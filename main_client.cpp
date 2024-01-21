/*
 * Entry point for the client application of QtStudentDBManager.
 * It sets up the client-side GUI, processes command-line arguments, and initializes the local database and model.
 */

#include "ClientDatabase.h"
#include "Settings.h"
#include "StudentModel.h"

#include <QCommandLineParser>
#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app {argc, argv};
    app.setApplicationName(QStringLiteral("QtStudentDBManager"));

    QCommandLineParser parser;
    parser.setApplicationDescription(QStringLiteral("Client Application"));
    parser.addHelpOption();

    const QCommandLineOption schoolOption(QStringLiteral("school"),
        QStringLiteral("Set the school name"), QStringLiteral("name"));
    parser.addOption(schoolOption);
    parser.process(app);

    if (!parser.isSet(schoolOption)) {
        qCritical() << "Error: School name must be provided. Use --school <name>.";
        parser.showHelp(1);
    }

    const QString& cliSchoolName = parser.value(schoolOption);
    qDebug() << "Client for school:" << cliSchoolName;
    app.setOrganizationName(cliSchoolName);

    // Init local database.
    if (!ClientDatabase::createTables()) {
        return 1;
    }

    qmlRegisterType<StudentModel>("com.student.manager", 1, 0, "StudentModel");

    // Set up QML engine and main window component.
    QQmlApplicationEngine engine {&app};
    engine.load(QUrl {QStringLiteral("qrc:///main.qml")});

    if (engine.rootObjects().isEmpty()) {
        qCritical() << "Application GUI failed to load.";
        return 1;
    }

    return app.exec();
}
