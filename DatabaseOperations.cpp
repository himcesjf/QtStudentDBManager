// DatabaseOperations.cpp
#include "DatabaseOperations.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

// Constructor: Initializes the database connection using SQLite and opens 'students.db'.
DatabaseOperations::DatabaseOperations() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("students.db");
    db.open();
}

// Destructor: Closes the database connection when the object is destroyed.
DatabaseOperations::~DatabaseOperations() {
    if (db.isOpen()) {
        db.close();
        qDebug() << "Database connection closed.";
    }
}

// createTable: Creates a new table named 'student_table' in the database if it doesn't exist.
void DatabaseOperations::createTable() {
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS student_table ("
               "id INTEGER PRIMARY KEY, "
               "firstName TEXT, "
               "lastName TEXT, "
               "middleName TEXT, "
               "roll INTEGER, "
               "class TEXT)");
}

// insertStudent: Prepares and execute SQL query to insert a student's data into the database.
void DatabaseOperations::insertStudent(const Student &student) {
    //Exception handling
    try{
        QSqlQuery query;
        query.prepare("INSERT INTO student_table (firstName, lastName, middleName, roll, class) "
                    "VALUES (?, ?, ?, ?, ?)"); //https://doc.qt.io/qt-6/qsqlquery.html

        //Serialization
        query.addBindValue(QVariant(student.getFirstName()));
        query.addBindValue(QVariant(student.getLastName()));
        query.addBindValue(QVariant(student.getMiddleName()));
        query.addBindValue(QVariant(student.getRoll()));
        query.addBindValue(QVariant(student.getClass()));

        qDebug() << "\nInserting record for student ID" << student.getId();

        if(!query.exec()) {
            throw std::runtime_error(query.lastError().text().toStdString());
            }
        } catch (const std::runtime_error& e) {
            qDebug() << "Database error in insertStudent: " << e.what();
    }
}


// updateStudent: Updates student's record in the database using the student's ID.
void DatabaseOperations::updateStudent(const Student &student) {
    //Exception handling
    try{
        QSqlQuery query;
        query.prepare("UPDATE student_table SET firstName = ?, lastName = ? WHERE id = ?");

        //Serialization
        query.addBindValue(QVariant(student.getFirstName()));
        query.addBindValue(QVariant(student.getLastName()));
        query.addBindValue(QVariant(student.getId()));

        qDebug() << "\nUpdating records for student ID" << student.getId();

        if(!query.exec()) {
            throw std::runtime_error(query.lastError().text().toStdString());
            }
        } catch (const std::runtime_error& e) {
            qDebug() << "Database error in updateStudent: " << e.what();
    }
}


// displayAllStudents: Displays all student records from the database.
/*void DatabaseOperations::displayAllStudents() {
    QSqlQuery query("SELECT * FROM student_table");

    if (!query.next()) {
        qDebug() << "No Records";
        return;
    }

    do {
        QString firstName = query.value("firstName").toString();
        QString lastName = query.value("lastName").toString();
        QString middleName = query.value("middleName").toString();
        int roll = query.value("roll").toInt();
        QString studentClass = query.value("class").toString();
        int id = query.value("id").toInt();
        qDebug() << "ID:" << id << "Name:" << firstName << lastName << middleName << "Roll:" << roll << "Class:" << studentClass;
    } while (query.next());

    if (query.lastError().isValid()) {
        qDebug() << "Database error in displayAllStudents: " << query.lastError().text();
    }
} */


QList<Student> DatabaseOperations::displayAllStudents() {
    QList<Student> studentsList;
    QSqlQuery query("SELECT * FROM student_table");

    if (!query.next()) {
        qDebug() << "No Records";
        return studentsList;
    }

    do {
        Student student;
        student.setId(query.value("id").toInt());
        student.setFirstName(query.value("firstName").toString());
        student.setLastName(query.value("lastName").toString());
        student.setMiddleName(query.value("middleName").toString());
        student.setRoll(query.value("roll").toInt());
        student.setClass(query.value("class").toString());
        studentsList.append(student);
    } while (query.next());

    if (query.lastError().isValid()) {
        qDebug() << "Database error in displayAllStudents: " << query.lastError().text();
    }

    return studentsList;
}




// deleteStudent: Deletes a student's record from the database using the student's ID.
void DatabaseOperations::deleteStudent(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM student_table WHERE id = ?"); //The "?" is a placeholder for a parameter value that will be bound to the query later
    query.addBindValue(QVariant(id));
    qDebug() << "\nDeleting student record for student ID" << id;
    query.exec();
}

//deleteAllStudents: Deletes all student records
void DatabaseOperations::deleteAllStudents() {
    QSqlQuery query;
    query.exec("DELETE FROM student_table");
    qDebug() << "All student records deleted";
}

// Close the database connection explicitly.
void DatabaseOperations::closeDatabaseConnection() {
    if (db.isOpen()) {
        db.close();
        qDebug() << "Database connection closed.";
    }
}

