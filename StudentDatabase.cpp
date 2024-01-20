/*
 * Implementation of the StudentDatabase class.
 * Handles all database operations on the server side, including querying and updating student records.
 */


#include "StudentDatabase.h"
#include "Settings.h"
#include "Student.h"

#include <QDebug>

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>

StudentDatabase::StudentDatabase(QObject *parent)
    : QObject(parent)
{
    //Set up remote database on server side
    QString connectionName = "serverDb"; // Name for the PostgreSQL connection
    m_db = QSqlDatabase::addDatabase(QStringLiteral("QPSQL"), connectionName);
 
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

StudentDatabase::~StudentDatabase()
{
    if (m_db.isOpen()) {
        m_db.close();
        qDebug() << "Database connection closed.";
    }
}

void StudentDatabase::sendSchoolStudents(const QString &schoolName)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM students WHERE school = :schoolName");
    query.bindValue(":schoolName", schoolName);

    if (!query.exec()) {
        qCritical() << "Send school students query failed." << query.lastError().text();
        return;
    }

    QVector<Student*> students;

    do {
            Student *student = new Student(nullptr,
                query.value("id").toInt(),
                query.value("firstName").toString(),
                query.value("middleName").toString(),
                query.value("lastName").toString(),
                query.value("roll").toInt(),
                query.value("class").toString(),
                query.value("school").toString());
            students.append(student);
        } while (query.next());

    emit allStudents(students);
}


void StudentDatabase::allStudentsRequested() const
{
    QSqlQuery query("SELECT * FROM students", m_db);
    
    if (!query.next()) {
        qDebug() << "No students in database.";
        return;
    }
    
    QVector<Student*> students;
    
    do {
        Student *student = new Student(nullptr,
            query.value("id").toInt(),
            query.value("firstName").toString(),
            query.value("middleName").toString(),
            query.value("lastName").toString(),
            query.value("roll").toInt(),
            query.value("class").toString(),
            query.value("school").toString());
        students.append(student);
    } while (query.next());
    
    emit allStudents(students);
}

void StudentDatabase::enterStudent(Student *student)
{
    qDebug() << "enter";
    
    if (!student) {
        return;
    }
    
    QSqlQuery query(m_db);
    
    if (student->id() == -1) {
        qDebug() << "New student for database:" << student;

        query.prepare("INSERT INTO students (firstName, middleName, lastName, roll, class, school) "
            "VALUES (:firstName, :middleName, :lastName, :roll, :class, :school)");
        
        query.bindValue(":firstName", student->firstName());
        query.bindValue(":middleName", student->middleName());
        query.bindValue(":lastName", student->lastName());
        query.bindValue(":roll", student->roll());
        query.bindValue(":class", student->className());
        query.bindValue(":school", student->schoolName());

        const bool success = query.exec();

        if (success) {
            const int id = query.lastInsertId().toInt();
            qDebug() << "Newly inserted id:" << id;
            emit newStudentAdded(id);
        } else {
            qCritical() << "Insert failed:" << query.lastError().text();
            qCritical() << "Failed Query:" << query.lastQuery();
        }
    } else {
        qDebug() << "Updating database record for:" << student;

        query.prepare("UPDATE students SET firstName = :firstName, middleName = :middleName, "
            "lastName = :lastName, roll = :roll, class = :class, school = :school WHERE id = :id");

        query.bindValue(":firstName", student->firstName());
        query.bindValue(":middleName", student->middleName());
        query.bindValue(":lastName", student->lastName());
        query.bindValue(":roll", student->roll());
        query.bindValue(":class", student->className());
        query.bindValue(":school", student->schoolName());
        query.bindValue(":id", student->id());
            
        query.exec();
        
        emit newStudentAdded(student->id());
    }
    
    delete student;
}

void StudentDatabase::createTable()
{
    if (!m_db.isOpen()) {
        return;
    }
 
    QSqlQuery query(m_db);
    if(!query.exec("CREATE TABLE IF NOT EXISTS students ("
        "id SERIAL PRIMARY KEY, " //For PostgreSQL
//        "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," //For SQlite
        "firstName TEXT, "
        "middleName TEXT, "
        "lastName TEXT, "
        "roll INTEGER, "
        "class TEXT, "
        "school TEXT)")) {qCritical() << "Error creating table: " << query.lastError().text(); return;}

        qDebug() <<"Table 'students' created or already exists on server side.";
}
