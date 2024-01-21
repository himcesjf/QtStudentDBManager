#include "ClientDatabase.h"
#include "Student.h"

#include <QCoreApplication>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>

ClientDatabase::ClientDatabase(QObject *parent)
    : QObject(parent)
{

    // Manually combine GenericDataLocation + app name to share a single sqlite
    // database among client instances. The write locking within sqlite itself
    // makes this safe.
    const QString &appDataDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)
        + QDir::separator()
        + QCoreApplication::instance()->applicationName();

    QDir dir(appDataDir);

    if (!dir.exists()) {
        const bool created = dir.mkpath(".");
        if (!created) {
            qCritical() << "Error: Application data directory could not be created.";
        }
    }

    // Set up application default database.
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName(QDir::toNativeSeparators(appDataDir
        + QDir::separator()
        + QStringLiteral("students.db")));

    qDebug() << "Database file at:" << db.databaseName();

    if (!db.open()) {
        qCritical() << "Error: Failed to open database:" << db.lastError().text();
    }
}

bool ClientDatabase::createTables()
{
    instance(); // Lazy-init db.

    if (!QSqlDatabase::database().isOpen()) {
        return false;
    }

    QSqlQuery studentTableQuery;
    studentTableQuery.prepare("CREATE TABLE IF NOT EXISTS students ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
        "version INTEGER DEFAULT 1, "
        "school TEXT, "
        "firstName TEXT, "
        "middleName TEXT, "
        "lastName TEXT, "
        "roll INTEGER DEFAULT 0, "
        "class TEXT)");

    QSqlQuery syncTableQuery;
    syncTableQuery.prepare("CREATE TABLE IF NOT EXISTS sync ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
        "student BLOB)");

    const bool success = studentTableQuery.exec() && syncTableQuery.exec();

    if (!success) {
        qCritical() << "Error: Failed to create local database tables.";
    }

    return success;
}

QVector<Student *> ClientDatabase::students(const QString &school)
{
    instance(); // Lazy-init db.

    QSqlQuery query;
    query.prepare("SELECT * FROM students WHERE school = :school");
    query.bindValue(":school", school);
    query.exec();

    QVector<Student*> students;

    if (!query.next()) {
        qDebug() << "No students in local database.";
        return students;
    }

    do {
        Student *student = new Student(query.value("id").toInt(),
            query.value("version").toInt(),
            query.value("school").toString(),
            query.value("firstName").toString(),
            query.value("middleName").toString(),
            query.value("lastName").toString(),
            query.value("roll").toInt(),
            query.value("class").toString());
        students.append(student);
    } while (query.next());

    return students;
}

bool ClientDatabase::add(Student *student)
{
    if (!student) {
        return -1;
    }

    instance(); // Lazy-init db.

    qDebug() << "New student for database:" << student;

    QSqlQuery query;
    query.prepare("INSERT INTO students (version, school, firstName, middleName, lastName, roll, class) "
        "VALUES (:version, :school, :firstName, :middleName, :lastName, :roll, :class)");

    query.bindValue(":version", student->version());
    query.bindValue(":firstName", student->firstName());
    query.bindValue(":middleName", student->middleName());
    query.bindValue(":lastName", student->lastName());
    query.bindValue(":roll", student->roll());
    query.bindValue(":class", student->className());
    query.bindValue(":school", student->school());

    const bool success = query.exec();

    if (success) {
        const int id = query.lastInsertId().toInt();
        qDebug() << "Newly inserted id:" << id;
        student->setId(id);

        logToSync(student);

        return true;
    } else {
        qDebug() << "Insert failed:" << query.lastError().text();
    }

    return false;
}

bool ClientDatabase::update(Student *student)
{
    if (!student) {
        return false;
    }

    instance(); // Lazy-init db.

    qDebug() << "Updating database record for:" << student;

    const int currentVersion = student->version();
    int dbVersion = -1;

    QSqlQuery versionQuery;
    versionQuery.prepare("SELECT version FROM students WHERE id = :id");
    versionQuery.bindValue(":id", student->id());
    versionQuery.exec();

    if (versionQuery.next()) {
        dbVersion = versionQuery.value(0).toInt();
    } else {
        qDebug() << "This student does not exist in the local database yet (it was likely retrieved from the server). Proceeding to add it.";
        add(student);
        return true;
    }

    qDebug() << "Version number in database:" << dbVersion;

    if (dbVersion > currentVersion) {
        qDebug() << "Version number in database is higher. Aborting update.";
        return false;
    } else {
        qDebug() << "Updating to version:" << (currentVersion + 1);
        student->setVersion(currentVersion + 1);
    }

    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE students SET version = :version, firstName = :firstName, middleName = :middleName, "
        "lastName = :lastName, roll = :roll, class = :class WHERE id = :id");

    updateQuery.bindValue(":version", student->version());
    updateQuery.bindValue(":firstName", student->firstName());
    updateQuery.bindValue(":middleName", student->middleName());
    updateQuery.bindValue(":lastName", student->lastName());
    updateQuery.bindValue(":roll", student->roll());
    updateQuery.bindValue(":class", student->className());
    updateQuery.bindValue(":id", student->id());
    updateQuery.exec();

    logToSync(student);

    return true;
}

void ClientDatabase::logToSync(const Student *student)
{
    qDebug() << "Adding to sync table:" << student;

    QByteArray serializedStudent;
    QDataStream studentStream(&serializedStudent, QIODeviceBase::WriteOnly);
    studentStream.setVersion(QDataStream::Qt_6_0);
    studentStream << *student;

    QSqlQuery query;
    query.prepare("INSERT INTO sync (student) "
        "VALUES (:student)");
    query.bindValue(":student", serializedStudent);

    query.exec();
}
