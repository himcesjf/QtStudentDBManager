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

    // Create Student objects on the heap
    QVector<Student*> students;
    students.append(new Student(&app, "John", "Doe"));
    students.append(new Student(&app, "Jane", "Doe"));

    // Setup server
    QTcpServer server;
    server.listen(QHostAddress(settings.value("server/host").toString()),
        settings.value("server/port").toUInt());

    qDebug() << QString("Listening on %1:%2.")
            .arg(server.serverAddress().toString())
            .arg(server.serverPort());

    QObject::connect(&server, &QTcpServer::newConnection, [&]() {
        qDebug() << "New client connected.";

        QTcpSocket *clientConnection = server.nextPendingConnection();
        QObject::connect(clientConnection, &QTcpSocket::disconnected,
                         clientConnection, &QTcpSocket::deleteLater);

        QDataStream out(clientConnection);
        out.setVersion(QDataStream::Qt_6_0);

        // Send each student
        for (Student *student : std::as_const(students)) {
            qDebug() << "Sending student:" << student->firstName() << student->lastName();
            out << *student;
        }

        clientConnection->disconnectFromHost();
    });

    int result = app.exec();

    return result;
}
