#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTcpSocket>
#include <QTableView>
#include <QSettings>
#include "StudentModel.h"
#include "Student.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Load server settings
    QSettings settings("config.ini", QSettings::IniFormat);
    QString serverAddress = settings.value("Server/address", "localhost").toString();
    int serverPort = settings.value("Server/port", 12345).toInt();

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

    /*
    // Setup GUI
    QTableView tableView;
    tableView.setModel(&model);
    tableView.show();
    */

     // Register QML types.
    qmlRegisterType<StudentModel>("com.student.manager", 1, 0, "StudentModel");

    // Create QML engine and load GUI.
    QQmlApplicationEngine engine {&app};
    engine.load(QUrl {QStringLiteral("qrc:///main.qml")});

    if (engine.rootObjects().isEmpty()) {
        qCritical() << "Application GUI failed to load.";
        return 1;
    }


    return app.exec();
}
