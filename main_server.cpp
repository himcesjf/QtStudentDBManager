#include "NetworkMessage.h"
#include "Student.h"
#include "StudentDatabase.h"
#include "StudentServer.h"
#include "Settings.h"

#include <QDebug>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication app {argc, argv};

    StudentDatabase db {&app};
    StudentServer server {&app};
    
    QObject::connect(&server, &StudentServer::requestAllStudents,
        &db, &StudentDatabase::allStudentsRequested);
    QObject::connect(&server, &StudentServer::studentReceived,
        &db, &StudentDatabase::enterStudent);
    QObject::connect(&db, &StudentDatabase::allStudents,
        &server, &StudentServer::sendStudents);
    QObject::connect(&db, &StudentDatabase::newStudentAdded,
        &server, &StudentServer::newStudentAdded);
        
    return app.exec();
}
