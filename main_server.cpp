//main_server.cpp
#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include "Student.h"
#include "Settings.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app {argc, argv};

    Settings& settings = Settings::instance();

    // Create example students for testing
    QVector<Student*> exampleStudents;
    exampleStudents.append(new Student(&app, 1, "John", "S", "Doe", 101, "English"));
    exampleStudents.append(new Student(&app, 2, "Jane", "K", "Doe", 102, "Science"));

    // Setup server
    QTcpServer server;
    if (!server.listen(QHostAddress(settings.value("server/host").toString()),
                       settings.value("server/port").toUInt())) {
        qDebug() << "Server failed to start. Error:" << server.errorString();
        return -1;
    }

    qDebug() << QString("Listening on %1:%2.")
            .arg(server.serverAddress().toString())
            .arg(server.serverPort());

    QObject::connect(&server, &QTcpServer::newConnection, [&](){
        qDebug() << "New client connected.";

        QTcpSocket *clientConnection = server.nextPendingConnection();
        QObject::connect(clientConnection, &QTcpSocket::disconnected,
                         clientConnection, &QTcpSocket::deleteLater);

        //Immediately send example students upon new connection
        QDataStream out(clientConnection);
        out.setVersion(QDataStream::Qt_6_0);

        // Send each student
        for (const Student *student : std::as_const(exampleStudents))
        {
            out << *student;
            qDebug() << "Sending student:"
                        << student->id()
                        << student->firstName()
                        << student->middleName()
                        << student->lastName()
                        << student->roll()
                        << student->className();
        }

        QObject::connect(clientConnection, &QTcpSocket::readyRead, [clientConnection]() {
            QByteArray incomingData = clientConnection->readAll();
            QString message = QString::fromUtf8(incomingData);
            //###Needs Improvement
            if (message == "Hello Server!" || message == "Hello from client!") {
                qDebug() << "Message received from client:" << incomingData;
            } else {
                // Deserialize and process the student data
                QDataStream in(&incomingData, QIODevice::ReadOnly);
                in.setVersion(QDataStream::Qt_6_0);

                while (!in.atEnd()){
                    Student *receivedStudent = new Student();
                    in >> *receivedStudent;

                    // Log the received student
                    qDebug() << "Received student data from GUI client:"
                            << receivedStudent->id()
                            << receivedStudent->firstName()
                            << receivedStudent->middleName()
                            << receivedStudent->lastName()
                            << receivedStudent->roll()
                            << receivedStudent->className();
                    // Clean up
                    delete receivedStudent;
                }
            }
        });
        //clientConnection->disconnectFromHost();
    });

    int result = app.exec();

    return result;
}
