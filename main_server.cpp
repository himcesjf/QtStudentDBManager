//main_server.cpp
#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include "Student.h"

#include "Settings.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Settings settings;
    qDebug() << QString("Listening on %1:%2")
            .arg(settings.value("server/host").toString())
            .arg(settings.value("server/port").toString());

    // Create Student objects on the heap
    QVector<Student*> students;
    students.append(new Student(nullptr, "John", "Doe"));
    students.append(new Student(nullptr, "Jane", "Doe"));

    // Setup server
    QTcpServer server;
    server.listen(QHostAddress::Any, 12345);
    QObject::connect(&server, &QTcpServer::newConnection, [&]() {
        QTcpSocket *clientConnection = server.nextPendingConnection();
        QObject::connect(clientConnection, &QTcpSocket::disconnected,
                         clientConnection, &QTcpSocket::deleteLater);

        QDataStream out(clientConnection);
        out.setVersion(QDataStream::Qt_5_0);

        // Send each student
        for (Student *student : std::as_const(students)) {
            out << *student;
        }
        clientConnection->disconnectFromHost();
    });

    int result = a.exec();

    // Clean up
    qDeleteAll(students);
    students.clear();

    return result;
}
