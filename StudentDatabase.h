/*
 * Header file for the StudentDatabase class.
 * Declares functionalities for interacting with the server-side database to manage student data.
 */


#pragma once

#include <QObject>
#include <QSqlDatabase>

class Student;


class StudentDatabase : public QObject {
    Q_OBJECT
    
public:
    explicit StudentDatabase(QObject *parent = nullptr);
    ~StudentDatabase();
    
public Q_SLOTS:
    void allStudentsRequested() const;
    void sendSchoolStudents(const QString &schoolName);
    void enterStudent(Student *student);
    
Q_SIGNALS:
    void allStudents(QVector<Student*> students) const;
    void newStudentAdded(int id) const;
    
private:
    void createTable();
    
    QSqlDatabase m_db;
};
