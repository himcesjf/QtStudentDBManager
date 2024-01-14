#include "StudentModel.h"
#include "Settings.h"

#include <QTcpSocket>

#include <algorithm>

StudentModel::StudentModel(QObject *parent)
    : QAbstractTableModel(parent)
    , m_socket {nullptr}
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

void StudentModel::connectToServer()
{
    if (m_socket) {
        delete m_socket;
        clearStudents();
    }

    Settings &settings = Settings::instance();

    m_socket = new QTcpSocket {this};
    m_socket->connectToHost(QHostAddress(settings.value("server/host").toString()),
        settings.value("server/port").toUInt());

    qDebug() << QString("Connecting to server %1:%2 ...").arg(settings.value("server/host").toString()).arg(settings.value("server/port").toUInt());

    QObject::connect(m_socket, &QTcpSocket::connected, this, [=]() {
        qDebug() << QString("Connected to server %1:%2.")
            .arg(m_socket->peerAddress().toString())
            .arg(m_socket->peerPort());
    });

    QObject::connect(m_socket, &QTcpSocket::readyRead, this, &StudentModel::loadFromServer);
}


void StudentModel::loadFromServer()
{
    qDebug() << "Loading data from server ...";

    QDataStream in(m_socket);
    in.setVersion(QDataStream::Qt_6_0);

     while (!in.atEnd()) {
         Student *student = new Student;
         in >> *student;
         qDebug() << "Received student:" << student->id() << student->firstName() << student->middleName() << student->lastName() << student->roll() << student->className();
        addStudent(student);
    }
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
