#include "StudentModel.h"
#include "Settings.h"

#include <QTcpSocket>

#include <algorithm>

StudentModel::StudentModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    loadFromServer();
}

StudentModel::~StudentModel()
{
    qDeleteAll(m_students);
    m_students.clear();
}

int StudentModel::rowCount(const QModelIndex & /*parent*/) const
{
    return m_students.size();
}

int StudentModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 6; // ID, First Name, Middle Name, Last Name, Roll, Class Name
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
        default:
            return QVariant();
    }
}

void StudentModel::loadFromServer()
{
    qDebug() << "Loading data from server ...";

    Settings &settings = Settings::instance();

    QTcpSocket *socket = new QTcpSocket {this};
    socket->connectToHost(QHostAddress(settings.value("server/host").toString()),
        settings.value("server/port").toUInt());

    QObject::connect(socket, &QTcpSocket::connected, this, [=]() {
        qDebug() << QString("Connected to %1:%2.")
            .arg(socket->peerAddress().toString())
            .arg(socket->peerPort());
    });

    QObject::connect(socket, &QTcpSocket::readyRead, this, [=]() {
            QDataStream in(socket);
            in.setVersion(QDataStream::Qt_6_0);

            while (!in.atEnd()) {
                Student *student = new Student;
                in >> *student;
                qDebug() << "Received student:" << student->id() << student->firstName() << student->middleName() << student->lastName() << student->roll() << student->className();
                addStudent(student);
            }
    });

    QObject::connect(socket, &QTcpSocket::disconnected, this, [=]() {
        socket->deleteLater();
    });
}

void StudentModel::addStudent(Student *student)
{
    beginInsertRows(QModelIndex(), m_students.size(), m_students.size());
    qDebug() << "Inserting student:" << student->id() << student->firstName() << student->middleName() << student->lastName() << student->roll() << student->className();
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
