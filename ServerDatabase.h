/*
 * Header file for the ServerDatabase class.
 * Declares functionalities for interacting with the server-side database to manage student data.
 */


#pragma once

#include <QObject>
#include <QSqlDatabase>
#include "Student.h"

//class Student;


class ServerDatabase : public QObject {
    Q_OBJECT

public:
    explicit ServerDatabase(QObject *parent = nullptr);
    ~ServerDatabase();

public Q_SLOTS:
    void studentsRequested(const QString &school) const;
    void enterStudent(Student *student);

Q_SIGNALS:
    void studentsToSend(QVector<Student *> students) const;
    void newStudentAdded(int id) const;

private:
    void createTable();

    QSqlDatabase m_db;
};
