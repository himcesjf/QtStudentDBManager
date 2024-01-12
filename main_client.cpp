//main_client.cpp
#include <QApplication>
#include <QTcpSocket>
#include <QTableView>
#include <QSettings>
#include "StudentModel.h"
#include "Student.h"

#include "Settings.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    /*
    // Load server settings
    QSettings settings("config.ini", QSettings::IniFormat);
    QString serverAddress = settings.value("Server/address", "localhost").toString();
    int serverPort = settings.value("Server/port",12345).toInt();
    */

    Settings settings;
    QString serverAddress = settings.value("server/host").toString();
    int serverPort = settings.value("server/port").toInt();
    qDebug() << QString("Connecting to %1:%2")
            .arg(settings.value("server/host").toString())
            .arg(settings.value("server/port").toString());


    // Setup the model
    StudentModel model;

    // Setup TCP client
    QTcpSocket socket;
    socket.connectToHost(serverAddress, serverPort);

    if (socket.waitForConnected()) {
        if (socket.waitForReadyRead()) {
            QDataStream in(&socket);
            in.setVersion(QDataStream::Qt_5_0);

            while (!in.atEnd()) {
                Student *student = new Student;
                in >> *student;
                model.addStudent(student);
            }
        }
    }

    // Setup GUI
    QTableView tableView;
    tableView.setModel(&model);
    tableView.show();

    return app.exec();
}
