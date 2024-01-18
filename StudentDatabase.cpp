#include "StudentDatabase.h"
#include "Settings.h"
#include "Student.h"

#include <QDebug>

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase> //For SQLite database

StudentDatabase::StudentDatabase(QObject *parent)
    : QObject(parent)
{
    m_db = QSqlDatabase::addDatabase(QStringLiteral("QPSQL"), QStringLiteral("studentDatabase"));
 
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
            query.value("rollName").toInt(),
            query.value("class").toString());
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

        query.prepare("INSERT INTO students (firstName, middleName, lastName, roll, class) "
            "VALUES (:firstName, :middleName, :lastName, :roll, :class)");
        
        query.bindValue(":firstName", student->firstName());
        query.bindValue(":middleName", student->middleName());
        query.bindValue(":lastName", student->lastName());
        query.bindValue(":roll", student->roll());
        query.bindValue(":class", student->className());

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
            "lastName = :lastName, roll = :roll, class = :class WHERE id = :id");

        query.bindValue(":firstName", student->firstName());
        query.bindValue(":middleName", student->middleName());
        query.bindValue(":lastName", student->lastName());
        query.bindValue(":roll", student->roll());
        query.bindValue(":class", student->className());
            
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
    
    query.exec("CREATE TABLE IF NOT EXISTS students ("
        "id SERIAL PRIMARY KEY, " //For PostgreSQL
//        "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," //For SQlite
        "firstName TEXT, "
        "middleName TEXT, "
        "lastName TEXT, "
        "roll INTEGER, "
        "class TEXT)");
}
