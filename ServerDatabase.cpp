/*
 * Implementation of the ServerDatabase class.
 * Handles all database operations on the server side, including querying and updating student records.
 */


#include "ServerDatabase.h"
#include "Settings.h"
#include "Student.h"

#include <QDebug>

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>

ServerDatabase::ServerDatabase(QObject *parent)
    : QObject(parent)
{
    m_db = QSqlDatabase::addDatabase(QStringLiteral("QPSQL"),
        QStringLiteral("studentsdb"));

    Settings &settings = Settings::instance();

    qDebug() << QString("Connecting to database %1 at %2:%3 as user %4.")
        .arg(settings.value("server/dbName").toString())
        .arg(settings.value("server/dbHost").toString())
        .arg(settings.value("server/dbPort").toUInt())
        .arg(settings.value("server/dbUser").toString());

    m_db.setPort(settings.value("server/dbPort").toUInt());
    m_db.setHostName(settings.value("server/dbHost").toString());
    m_db.setUserName(settings.value("server/dbUser").toString());
    m_db.setPassword(settings.value("server/dbPassword").toString());
    m_db.setDatabaseName(settings.value("server/dbName").toString());

    if (m_db.open()) {
        qDebug() << "Database connection open.";

        createTable();
    } else {
        qCritical() << "Error opening database!";
    }
}

ServerDatabase::~ServerDatabase()
{
    if (m_db.isOpen()) {
        m_db.close();
        qDebug() << "Database connection closed.";
    }
}

void ServerDatabase::createTable()
{
    if (!m_db.isOpen()) {
        return;
    }

    QSqlQuery query(m_db);
    if (!query.exec("CREATE TABLE IF NOT EXISTS students ("
        "id SERIAL PRIMARY KEY, "
        "version INTEGER NOT NULL, "
        "school TEXT, "
        "firstName TEXT, "
        "middleName TEXT, "
        "lastName TEXT, "
        "roll INTEGER DEFAULT 0, "
        "class TEXT)")) {
        qCritical() << "Error creating table: " << query.lastError().text();
        return;
    }
}

void ServerDatabase::studentsRequested(const QString &school) const
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM students WHERE school = :school");
    query.bindValue(":school", school);

    if (!query.exec()) {
        qCritical() << "Send students query failed." << query.lastError().text();
        return;
    }

    if (!query.next()) {
        qDebug() << "No students in database.";
        return;
    }

    QVector<Student*> students;

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

    emit studentsToSend(students);
}

void ServerDatabase::enterStudent(Student *student)
{
    if (!student) {
        return;
    }

    qDebug() << "Entering into database:" << student;

    QSqlQuery query(m_db);

    // Consolidate using an UPSERT query.
    query.prepare("INSERT INTO students (id, version, school, firstName, middleName, lastName, roll, class) "
        "VALUES (:id, :version, :school, :firstName, :middleName, :lastName, :roll, :class) "
        "ON CONFLICT (id) "
        "DO "
        "UPDATE SET version = EXCLUDED.version, "
        "           school = EXCLUDED.school, "
        "           firstName = EXCLUDED.firstName, "
        "           middleName = EXCLUDED.middleName, "
        "           lastName = EXCLUDED.lastName, "
        "           roll = EXCLUDED.roll, "
        "           class = EXCLUDED.class");

    query.bindValue(":id", student->id());
    query.bindValue(":version", student->version());
    query.bindValue(":school", student->school());
    query.bindValue(":firstName", student->firstName());
    query.bindValue(":middleName", student->middleName());
    query.bindValue(":lastName", student->lastName());
    query.bindValue(":roll", student->roll());
    query.bindValue(":class", student->className());

    if (!query.exec()) {
        qDebug() << "Error: Entering student record into database failed:" << query.lastError().text();
        return;
    }

    delete student;
}
