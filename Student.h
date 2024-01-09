// Student.h
#ifndef STUDENT_H
#define STUDENT_H

#include <QString>
#include <QMap>

class Student {
public:
    Student(); //Constrictor

    //Setters
    void setFirstName(const QString &name);
    void setLastName(const QString &name);
    void setMiddleName(const QString &name);
    void setId(int newId);
    void setRoll(int newRoll);
    void setClass(const QString &cls);
    void setSubjectMarks(const QMap<QString, double> &marks);

    //Getters
    QString getFirstName() const;
    QString getLastName() const;
    QString getMiddleName() const;
    int getId() const;
    int getRoll() const;
    QString getClass() const;
    QMap<QString, double> getSubjectMarks() const;

private:
    QString firstName;
    QString lastName;
    QString middleName;
    int id;
    int roll;
    QString studentClass;
    QMap<QString, double> subjectMarks;
};

#endif // STUDENT_H
