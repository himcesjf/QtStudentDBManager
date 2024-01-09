// DatabaseOperations.h
#ifndef DATABASEOPERATIONS_H
#define DATABASEOPERATIONS_H

#include <QSqlDatabase>
#include "Student.h"

// DatabaseOperations class handles all database related operations
class DatabaseOperations {
public:
    DatabaseOperations();
    void createTable();
    void insertStudent(const Student &student);
    void updateStudent(const Student &student);
    void displayAllStudents();
    void deleteStudent(int id);
    void deleteAllStudents();
private:
    QSqlDatabase db;
};

#endif // DATABASEOPERATIONS_H
