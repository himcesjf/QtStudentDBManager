// DatabaseOperations.h
#ifndef DATABASEOPERATIONS_H
#define DATABASEOPERATIONS_H

#include <QSqlDatabase>
#include "Student.h"

// DatabaseOperations class handles all database related operations
class DatabaseOperations {
    public:
        DatabaseOperations();
        ~DatabaseOperations(); // Destructor to close the database connection

        void createTable();
        void insertStudent(const Student &student);
        void updateStudent(const Student &student);
        //void displayAllStudents();
        QList<Student> displayAllStudents();
        void deleteStudent(int id);
        void deleteAllStudents();
        void closeDatabaseConnection();


    private:
        QSqlDatabase db;
};

#endif // DATABASEOPERATIONS_H
