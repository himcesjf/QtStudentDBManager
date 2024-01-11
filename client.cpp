#include <QCoreApplication>
#include <QTcpSocket>
#include <QDataStream>
#include "studentmodel.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    StudentModel model;

    QTcpSocket socket;
    socket.connectToHost("localhost", 12345); // Connect to server at localhost and port 12345

    QObject::connect(&socket, &QTcpSocket::connected, [&]() {
        qDebug() << "Connected to server!";
    });

    QObject::connect(&socket, &QTcpSocket::readyRead, [&]() {
        QDataStream inStream(&socket);
        inStream.setVersion(QDataStream::Qt_5_15); // Qt version

        while (!inStream.atEnd()) {
            Student student;
            inStream >> student.id
                     >> student.firstName
                     >> student.lastName
                     >> student.middleName
                     >> student.roll
                     >> student.classRoom;

            model.addStudent(student);
        }
    });

    return a.exec();
}
