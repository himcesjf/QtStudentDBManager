//main_client.cpp
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTcpSocket>
//#include <QTableView>
#include <QSettings>
#include "StudentModel.h"
#include "Student.h"

#include "Settings.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    /*
    // Load server settings
    QSettings settings("config.ini", QSettings::IniFormat);
    QString serverAddress = settings.value("Server/address", "localhost").toString();
    int serverPort = settings.value("Server/port", 12345).toInt();
    */

    Settings settings;
    QString serverAddress = settings.value("server/host").toString();
    int serverPort = settings.value("server/port").toInt();
    qDebug() << QString("Connecting to %1:%2")
            .arg(settings.value("server/host").toString())
            .arg(settings.value("server/port").toString());

    // Setup the model
    StudentModel model;

    //Pre-populate with sample data
    model.addStudent(new Student(nullptr, "John", "MiddleJohn", "Doe", 1, "Science"));
    model.addStudent(new Student(nullptr, "Jane", "MiddleJane", "Doe", 2, "Math"));


    // Setup TCP client
    QTcpSocket socket;
    socket.connectToHost(serverAddress, serverPort);

    if (socket.waitForConnected()) {
        if (socket.waitForReadyRead()) {
            QDataStream in(&socket);
            in.setVersion(QDataStream::Qt_6_0);

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

    // Register the StudentModel type with the QML system
    qmlRegisterType<StudentModel>("com.student.manager", 1, 0, "StudentModel");

    // Create QML engine and load GUI.
    //QQmlApplicationEngine engine {&app};
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("studentModel", &model);
    engine.load(QUrl {QStringLiteral("qrc:///main.qml")});

    if (engine.rootObjects().isEmpty()) {
        qCritical() << "Application GUI failed to load.";
        return 1;
    }

    return app.exec();
}
