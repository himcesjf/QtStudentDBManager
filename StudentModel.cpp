/*
 * Implementation of the StudentModel class.
 * Manages the data model for the client application, interfacing with the server for data synchronization.
 */


#include "StudentModel.h"
#include "NetworkMessage.h"
#include "Settings.h"

#include <QTcpSocket>
#include <QTimer>

#include <algorithm>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

StudentModel::StudentModel(QObject *parent)
    : QAbstractTableModel(parent)
    , m_socket {nullptr}
    , m_storageConfirmed {false}
    , m_error {false}
{
    connectToServer();
}

StudentModel::~StudentModel()
{
     if (m_socket) {
        m_socket->disconnectFromHost();
        if (m_socket->state() == QAbstractSocket::UnconnectedState
            || m_socket->waitForDisconnected(1000)) {
            qDebug() << "Disconnected from server.";
        }
    }

    qDeleteAll(m_students);
    m_students.clear();
}

int StudentModel::rowCount(const QModelIndex & /*parent*/) const
{
    return m_students.size();
}

int StudentModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 7; // ID, First Name, Middle Name, Last Name, Roll, Class Name, School
}

QVariant StudentModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role);

    if (!index.isValid() || index.row() < 0 || index.row() >= m_students.length()) {
        return QVariant();
    }

    const Student *student = m_students.at(index.row());
    switch (index.column()) {
    case 0:
        return student->id();
    case 1:
        return student->firstName();
    case 2:
        return student->middleName();
    case 3:
        return student->lastName();
    case 4:
        return student->roll();
    case 5:
        return student->className();
    case 6:
        return student->schoolName();
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
            return "First Name";
        case 2:
            return "Middle Name";
        case 3:
            return "Last Name";
        case 4:
            return "Roll";
        case 5:
            return "Class";
        case 6:
            return "School";
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

    qDebug() << QString("Connecting to server %1:%2 ...").arg(settings.value("client/serverHost").toString()).arg(settings.value("client/serverPort").toUInt());

    QObject::connect(m_socket, &QTcpSocket::connected, this, [=]() {
        qDebug() << QString("Connected to server %1:%2.")
            .arg(m_socket->peerAddress().toString())
            .arg(m_socket->peerPort());
            
        // Send a welcome message
        QDataStream out(m_socket);
        out.setVersion(QDataStream::Qt_6_0);
        out << NetworkMessage(NetworkMessage::StatusMessage, "Hello server!");
    });


    QObject::connect(m_socket, &QTcpSocket::readyRead, this, &StudentModel::readFromServer);
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
        } else if (receivedMsg->type() == NetworkMessage::StorageConfirmation) {
            const int id = receivedMsg->payload().toInt();
            
            qDebug() << "Received storage confirmation with id:" << id;
            
            // Update id from server.
            if (m_lastStudentSent) {
                m_lastStudentSent->setId(id);
                
                const int row = m_students.indexOf(m_lastStudentSent);
                
                if (row >= 0) {
                    emit dataChanged(index(row, 0), index(row, columnCount() - 1));
                }
                
                m_lastStudentSent = nullptr;
            }
            
            m_storageConfirmed = true;
            resetError();
            emit storageSuccess();
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

void StudentModel::checkStorageConfirmed()
{
    if (!m_storageConfirmed) {
        m_error = true;
        m_errorString = QStringLiteral("Failed to save, please contact administrator");
        emit errorChanged();
        emit errorStringChanged();
    }
}

void StudentModel::sendToServer(Student *student)
{
    resetError();
    
    if (student && m_socket && m_socket->state() == QAbstractSocket::ConnectedState) {
        m_storageConfirmed = false;
        
        QDataStream out(m_socket);
        out.setVersion(QDataStream::Qt_6_0);
        
        QByteArray serializedStudent;
        QDataStream studentStream(&serializedStudent, QIODeviceBase::WriteOnly);
        studentStream.setVersion(QDataStream::Qt_6_0);
        studentStream << *student;
    
        qDebug() << "Sending student to server:" << student;
        out << NetworkMessage(NetworkMessage::StudentObject, serializedStudent);
        
        m_lastStudentSent = student;
        
        QTimer::singleShot(200, this, &StudentModel::checkStorageConfirmed);
    } else {
        qDebug() << "Unable to send to server.";
        m_error = true;
        m_errorString = QStringLiteral("Failed to save, please contact administrator");
        emit errorChanged();
        emit errorStringChanged();
    }
}

void StudentModel::updateStudent(int id, const QString &firstName,
        const QString &middleName, const QString &lastName, int roll, const QString &className, const QString &schoolName)
{
    if (id == -1) {
        Student *newStudent = new Student(nullptr, id, firstName, middleName, lastName, roll, className);
        insertStudent(newStudent);
        sendToServer(newStudent);
    } else {
        for (int i = 0; i < m_students.length(); ++i) {
            Student *existingStudent = m_students.at(i);
            
            if (existingStudent->id() == id) {
                existingStudent->setFirstName(firstName);
                existingStudent->setMiddleName(middleName);
                existingStudent->setLastName(lastName);
                existingStudent->setRoll(roll);
                existingStudent->setClassName(className);
                existingStudent->setSchoolName(schoolName);
                
                emit dataChanged(index(i, 0), index(i, columnCount() - 1));
                
                sendToServer(existingStudent);
                
                break;
            }
        }
    }
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
