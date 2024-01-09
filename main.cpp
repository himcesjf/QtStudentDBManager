#include <QCoreApplication>
#include "DatabaseOperations.h"
#include "Student.h"
#include <QDebug>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    // Initialize and set up the database
    DatabaseOperations dbOps;
    dbOps.createTable();

    dbOps.deleteAllStudents();

    // Create a new student and insert it into the database
    Student student1;
    student1.setId(1);
    student1.setFirstName("Himanshu");
    student1.setLastName("Rajput");
    student1.setMiddleName("A");
    student1.setRoll(101);
    student1.setClass("10A");
    dbOps.insertStudent(student1); // Insert the student record into the database

    // Display all students
    dbOps.displayAllStudents();

    //Inserting record of student
    Student student2;
    student2.setId(2);
    student2.setFirstName("Hemant");
    student2.setLastName("Roy");
    student2.setMiddleName("B");
    student2.setRoll(102);
    student2.setClass("10B");
    dbOps.insertStudent(student2);

    dbOps.displayAllStudents();

    // Update the student's information
    student2.setFirstName("Hemang");
    dbOps.updateStudent(student2);

    // Display all students again to see the updated information
    dbOps.displayAllStudents();

    // Delete the student from the database
    //dbOps.deleteStudent(student.getId());
    dbOps.deleteStudent(1);

    // Display all students to confirm deletion
    dbOps.displayAllStudents();

    //return a.exec();
    return 0;
}
