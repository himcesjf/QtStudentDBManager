/*
 * Implementation of the StudentModel class.
 * Manages the data model for the client application, interfacing with the server for data synchronization.
 */


#include "StudentModel.h"
#include "ClientDatabase.h"
#include "NetworkMessage.h"
#include "Settings.h"

#include <QTcpSocket>
#include <QTimer>

#include <algorithm>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

StudentModel::StudentModel(QObject *parent)
    : QAbstractTableModel(parent)
    , m_socket {nullptr}
    , m_error {false}
{
}

StudentModel::~StudentModel()
{
    if (m_socket
        && (m_socket->state() == QAbstractSocket::UnconnectedState
        || m_socket->waitForDisconnected(1000))) {
        qDebug() << "Disconnected from server.";
    }

    qDeleteAll(m_students);
    m_students.clear();
}

void StudentModel::classBegin()
{
}

void StudentModel::componentComplete()
{
    connectToServer();
}

QString StudentModel::school() const {
    return m_school;
}

void StudentModel::setSchool(const QString &school) {
    if (m_school != school) {
        m_school = school;
        emit schoolChanged();
    }
}

int StudentModel::rowCount(const QModelIndex & /*parent*/) const
{
    return m_students.size();
}

int StudentModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 7; // ID, Version, First Name, Middle Name, Last Name, Roll, Class, Updated
}

QVariant StudentModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_students.length()) {
        return QVariant();
    }

    const Student *student = m_students.at(index.row());

    if (role == Qt::DecorationRole) {
        return m_updatedFromLocal.contains(student->id());
    }

    switch (index.column()) {
    case 0:
        return student->id();
    case 1:
        return student->version();
    case 2:
        return student->firstName();
    case 3:
        return student->middleName();
    case 4:
        return student->lastName();
    case 5:
        return student->roll();
    case 6:
        return student->className();
    default:
        return QVariant();
    }
}

QVariant StudentModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();

    switch (section) {
        case 0:
            return "ID";
        case 1:
            return "Version";
        case 2:
            return "First Name";
        case 3:
            return "Middle Name";
        case 4:
            return "Last Name";
        case 5:
            return "Roll";
        case 6:
            return "Class";
        default:
            return QVariant();
    }
}

void StudentModel::connectToServer()
{
    if (m_socket) {
        delete m_socket;
        clearStudents();
    }

    Settings &settings = Settings::instance();

    m_socket = new QTcpSocket {this};
    m_socket->connectToHost(QHostAddress(settings.value("client/serverHost").toString()),
        settings.value("client/serverPort").toUInt());

    qDebug() << QString("Connecting to server %1:%2 ...")
        .arg(settings.value("client/serverHost").toString())
        .arg(settings.value("client/serverPort").toUInt());

    QObject::connect(m_socket, &QTcpSocket::connected, this, [this]() {
        qDebug() << QString("Connected to server %1:%2.")
            .arg(m_socket->peerAddress().toString())
            .arg(m_socket->peerPort());

        // Send a greeting message.
        QDataStream out(m_socket);
        out.setVersion(QDataStream::Qt_6_0);
        out << NetworkMessage(NetworkMessage::StatusMessage, "Hello server!");

        // Request student data for school.
        qDebug() << "Requesting initial student data from server.";
        out << NetworkMessage(NetworkMessage::StudentsRequest, m_school);
    });


    QObject::connect(m_socket, &QTcpSocket::readyRead, this, &StudentModel::readFromServer);

    QObject::connect(m_socket, &QTcpSocket::disconnected, this, [this]() {
        qDebug() << "Disconnected from server.";
        m_socket->deleteLater();
        m_socket = nullptr;

        // After the initial fetch from the server, proceed to
        // integrate data from the local database.
        refreshFromLocalDatabase(true /* silent */);
    });
}

void StudentModel::readFromServer()
{
    QDataStream in(m_socket);
    in.setVersion(QDataStream::Qt_6_0);

    while (!in.atEnd()){
        NetworkMessage *receivedMsg = new NetworkMessage();
        in >> *receivedMsg;

        if (receivedMsg->type() == NetworkMessage::StatusMessage) {
            const QString &msgString = receivedMsg->payload().toString();
            qDebug() << "Received message from server:" << receivedMsg->payload().toString();
        } else {
            QDataStream studentStream(receivedMsg->payload().toByteArray());
            studentStream.setVersion(QDataStream::Qt_6_0);
            Student *student = new Student;
            studentStream >> *student;
            qDebug() << "Received student from server:" << student;
            insertStudent(student);
        }

        delete receivedMsg;
    }
}

void StudentModel::updateStudent(int id, int version, const QString &firstName,
    const QString &middleName, const QString &lastName, int roll, const QString &className)
{
    resetUpdated();
    resetError();

    Student *studentFromArgs = new Student(id, version, m_school,
        firstName, middleName, lastName, roll, className);

    if (id == -1) {
        if (ClientDatabase::add(studentFromArgs)) {
            insertStudent(studentFromArgs);
            emit storageSuccess();
        } else {
            m_error = true;
            m_errorString = QStringLiteral("Error inserting into database.");
            emit errorChanged();
            emit errorStringChanged();
        }
    } else {
        for (int i = 0; i < m_students.length(); ++i) {
            Student *existingStudent = m_students.at(i);

            if (existingStudent->id() == id) {
                if (ClientDatabase::update(studentFromArgs)) {
                    existingStudent->updateFrom(studentFromArgs);
                    emit dataChanged(index(i, 0), index(i, columnCount() - 1));
                    emit storageSuccess();
                } else {
                    refreshFromLocalDatabase(); // Sync to local db in case of update error (e.g. version conflict).

                    m_error = true;
                    m_errorString = QStringLiteral("Overwrite error! Green rows have been updated with new data from other clients.");
                    emit errorChanged();
                    emit errorStringChanged();
                }

                delete studentFromArgs;
                break;
            }
        }
    }
}

void StudentModel::refreshFromLocalDatabase(bool silent)
{
    qDebug() << "Integrating current data from local database.";

    const QVector<Student *> &students = ClientDatabase::students(m_school);

    for (Student *dbStudent : std::as_const(students)) {
        Student *existingStudent = nullptr;
        int dbVersion = -1;

        for (Student *s : std::as_const(m_students)) {
            if (s->id() == dbStudent->id()) {
                existingStudent = s;
                break;
            }
        }

        if (existingStudent) {
            // Another client updated this student. Sync and inform the user.
            if (dbStudent->version() > existingStudent->version()) {
                existingStudent->updateFrom(dbStudent);
                delete dbStudent;

                if (!silent) {
                    m_updatedFromLocal.append(dbStudent->id());
                }

                const int row = m_students.indexOf(existingStudent);
                emit dataChanged(index(row, 0), index(row, columnCount() - 1));
            }
        } else { // Another client added a new student we didn't have yet.'
            if (!silent) {
                m_updatedFromLocal.append(dbStudent->id());
            }

            insertStudent(dbStudent);
        }
    }
}

void StudentModel::resetUpdated()
{
    m_updatedFromLocal.clear();
    emit dataChanged(index(0, 0), index(m_students.length() - 1, 6));
}

void StudentModel::insertStudent(Student *student)
{
    beginInsertRows(QModelIndex(), m_students.size(), m_students.size());
    m_students.append(student);
    endInsertRows();
}

void StudentModel::clearStudents()
{
    beginRemoveRows(QModelIndex(), 0, m_students.size() - 1);
    qDeleteAll(m_students);
    m_students.clear();
    endRemoveRows();
}

bool StudentModel::error() const
{
    return m_error;
}

QString StudentModel::errorString() const
{
    return m_errorString;
}

void StudentModel::resetError()
{
    if (m_error) {
        m_error = false;
        m_errorString.clear();
        emit errorChanged();
        emit errorStringChanged();
    }
}
