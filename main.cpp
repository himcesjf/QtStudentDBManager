#include "studentmodel.h"

#include <QDir>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSqlDatabase>
#include <QStandardPaths>

int main(int argc, char *argv[])
{
    QGuiApplication app {argc, argv};
    app.setApplicationDisplayName(QStringLiteral("Student Manager"));

    // Create application data dir.
    const QString &appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir;
    dir.mkdir(appDataDir);
    
    // Set up default database for application.
    QSqlDatabase db =  QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"));
    db.setDatabaseName(QDir::toNativeSeparators(appDataDir + QStringLiteral("/students.db")));
    qDebug() << "Database at:" << db.databaseName();

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
