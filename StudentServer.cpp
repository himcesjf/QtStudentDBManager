/*
 * Implementation of the StudentServer class.
 * Manages network communications and request handling for the server application.
 */


#include "StudentServer.h"
#include "NetworkMessage.h"
#include "Settings.h"
#include "Student.h"

#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>

StudentServer::StudentServer(QObject *parent)
    : QObject(parent)
    , m_tcpServer {new QTcpServer(this)}
{
    Settings& settings = Settings::instance();
    
    if (!m_tcpServer->listen(QHostAddress(settings.value("server/host").toString()),
        settings.value("server/port").toUInt())) {
        qDebug() << "Server failed to start. Error:" << m_tcpServer->errorString();
        return;
    }

    qDebug() << QString("Listening on %1:%2.")
            .arg(m_tcpServer->serverAddress().toString())
            .arg(m_tcpServer->serverPort());
            
    QObject::connect(m_tcpServer, &QTcpServer::newConnection, this, &StudentServer::newClientConnected);
}

StudentServer::~StudentServer()
{
}

void StudentServer::newStudentAdded(int id)
{
    if (!m_currentConnection) {
        return;
    }
    
    // Confirm reception of student
    QDataStream out(m_currentConnection);
    out.setVersion(QDataStream::Qt_6_0);
    qDebug() << "Sending storage confirmation and new id" << id;
    out << NetworkMessage(NetworkMessage::StatusMessage, QStringLiteral("OK"));
    out << NetworkMessage(NetworkMessage::StorageConfirmation, id);
}

void StudentServer::sendStudents(const QVector<Student*> &students)
{
    if (!m_currentConnection) {
        return;
    }
    
    QDataStream out(m_currentConnection);
    out.setVersion(QDataStream::Qt_6_0);
    
    for (const Student *student : students) {
        QByteArray serializedStudent;
        QDataStream studentStream(&serializedStudent, QIODeviceBase::WriteOnly);
        studentStream.setVersion(QDataStream::Qt_6_0);
        studentStream << *student;
    
        qDebug() << "Sending student to server:" << student;
        out << NetworkMessage(NetworkMessage::StudentObject, serializedStudent);
    }
    
    qDeleteAll(students);
}

void StudentServer::newClientConnected()
{
    qDebug() << "New client connected.";
    
    m_currentConnection = m_tcpServer->nextPendingConnection();
    
    QObject::connect(m_currentConnection, &QTcpSocket::readyRead,
        this, &StudentServer::readFromClient);
    QObject::connect(m_currentConnection, &QTcpSocket::disconnected,
        m_currentConnection, &QTcpSocket::deleteLater);
    
    QDataStream out(m_currentConnection);
    out.setVersion(QDataStream::Qt_6_0);
    out << NetworkMessage(NetworkMessage::StatusMessage, "Hello client!");
    
    emit requestAllStudents();
}

void StudentServer::readFromClient() {
    QByteArray incomingData = m_currentConnection->readAll();
    QDataStream in(&incomingData, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_6_0);

    while (!in.atEnd()) {
        NetworkMessage *receivedMsg = new NetworkMessage();
        in >> *receivedMsg;

        if (receivedMsg->type() == NetworkMessage::StatusMessage) {
            qDebug() << "Received message from client:" << receivedMsg->payload().toString();
        } else if (receivedMsg->type() == NetworkMessage::SchoolRequest) {
            QString schoolName = receivedMsg->payload().toString();
            qDebug() << "Received School Request from client: " << receivedMsg->payload().toString();
            emit requestSchoolStudents(schoolName);
        } else {
            QDataStream studentStream(receivedMsg->payload().toByteArray());
            studentStream.setVersion(QDataStream::Qt_6_0);
            Student *student = new Student;
            studentStream >> *student;
            qDebug() << "Received student from client:" << student;
            emit studentReceived(student);
        }

        delete receivedMsg;
    }
}

