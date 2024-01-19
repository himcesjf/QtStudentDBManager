#include "Settings.h"
#include "StudentModel.h"
#include "Student.h"

#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSettings>
#include <QTcpSocket>
#include <QCommandLineParser>
#include <QDir>
#include <QStandardPaths>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

int main(int argc, char *argv[])
{
    QGuiApplication app {argc, argv};

    QCommandLineParser parser;
    parser.setApplicationDescription("Client Application");
    parser.addHelpOption();

    // Open client with school option as a required argument
    QCommandLineOption schoolOption("school", "Set the school name", "name");
    parser.addOption(schoolOption);
    parser.process(app);

    if (!parser.isSet(schoolOption)) {
        qCritical() << "Error: School name must be provided. Use --school <name>.";
        parser.showHelp(1); //https://doc.qt.io/qt-6/qcommandlineparser.html#showHelp
    }


    const QString &appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir;
    dir.mkdir(appDataDir);

    // Set up local database on client side
    QString connectionName = "localDb";
    QSqlDatabase db =  QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), connectionName);
    db.setDatabaseName(QDir::toNativeSeparators(appDataDir + QStringLiteral("/students.db")));
    qDebug() << "Database at:" << db.databaseName();
    qDebug() << "Database connection name:" << connectionName;


    if (!db.open()) {
    qDebug() << "Error: Failed to open database. Error message: " << db.lastError().text();
    }

    QSqlQuery query(db);
    query.prepare("CREATE TABLE IF NOT EXISTS students ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," //For SQlite
        "firstName TEXT, "
        "middleName TEXT, "
        "lastName TEXT, "
        "roll INTEGER, "
        "class TEXT, "
        "school TEXT)");

    if (!query.exec()) {
        qDebug() << "Error: Failed to create the 'students' table on client-side. Error message: " << query.lastError().text();
    }




    qmlRegisterType<StudentModel>("com.student.manager", 1, 0, "StudentModel");
    QQmlApplicationEngine engine {&app};
    engine.load(QUrl {QStringLiteral("qrc:///main.qml")});

    if (engine.rootObjects().isEmpty()) {
        qCritical() << "Application GUI failed to load.";
        return 1;
    }

    return app.exec();
}
