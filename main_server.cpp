/*
 * Entry point for the server application of QtStudentDBManager.
 * It initializes the server components including ServerDatabase and StudentServer, and sets up signal-slot connections for handling client requests.
 */


#include "NetworkMessage.h"
#include "Student.h"
#include "ServerDatabase.h"
#include "StudentServer.h"
#include "Settings.h"

#include <QDebug>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication app {argc, argv};
    app.setApplicationName(QStringLiteral("QtStudentDBManager"));

    ServerDatabase db {&app};
    StudentServer server {&app};

    QObject::connect(&server, &StudentServer::requestStudents,
        &db, &ServerDatabase::studentsRequested);
    QObject::connect(&server, &StudentServer::studentReceived,
        &db, &ServerDatabase::enterStudent);
    QObject::connect(&db, &ServerDatabase::studentsToSend,
        &server, &StudentServer::sendStudents);

    return app.exec();
}
