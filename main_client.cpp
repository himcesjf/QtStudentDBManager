#include "Settings.h"
#include "StudentModel.h"
#include "Student.h"

#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSettings>
#include <QTcpSocket>

int main(int argc, char *argv[])
{
    QGuiApplication app {argc, argv};
    QString schoolName = argc > 1 ? argv[1] : "DefaultSchool"; //Modifying the Client to Start with a School Name

    QQmlApplicationEngine engine {&app};

    qmlRegisterType<StudentModel>("com.student.manager", 1, 0, "StudentModel");

    engine.load(QUrl {QStringLiteral("qrc:///main.qml")});

    if (engine.rootObjects().isEmpty()) {
        qCritical() << "Application GUI failed to load.";
        return 1;
    }

    return app.exec();
}
