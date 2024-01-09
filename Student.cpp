// Student.cpp
#include "Student.h"

Student::Student() {} //Constructor

//Setters
void Student::setFirstName(const QString &name) { firstName = name; }
void Student::setLastName(const QString &name) { lastName = name; }
void Student::setMiddleName(const QString &name) { middleName = name; }
void Student::setId(int newId) { id = newId; }
void Student::setRoll(int newRoll) { roll = newRoll; }
void Student::setClass(const QString &cls) { studentClass = cls; }
void Student::setSubjectMarks(const QMap<QString, double> &marks) { subjectMarks = marks; }

//Getters
QString Student::getFirstName() const { return firstName; }
QString Student::getLastName() const { return lastName; }
QString Student::getMiddleName() const { return middleName; }
int Student::getId() const { return id; }
int Student::getRoll() const { return roll; }
QString Student::getClass() const { return studentClass; }
QMap<QString, double> Student::getSubjectMarks() const { return subjectMarks; }
