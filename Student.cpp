#include "Student.h"

Student::Student(QObject *parent, const QString &firstName, const QString &middleName, const QString &lastName, int roll, const QString &className)
    : QObject(parent), m_firstName(firstName), m_middleName(middleName), m_lastName(lastName), m_roll(roll), m_className(className)
{
}

QString Student::firstName() const {
    return m_firstName;
}

void Student::setFirstName(const QString &firstName) {
    m_firstName = firstName;
}


QString Student::middleName() const {
    return m_middleName;
}

void Student::setMiddleName(const QString &middleName) {
    m_middleName = middleName;
}


QString Student::lastName() const {
    return m_lastName;
}

void Student::setLastName(const QString &lastName) {
    m_lastName = lastName;
}


int Student::roll() const {
    return m_roll;
}

void Student::setRoll(int roll) {
    m_roll = roll;
}

QString Student::className() const {
    return m_className;
}

void Student::setClassName(const QString &className) {
    m_lastName = className;
}



void Student::serialize(QDataStream &out) const {
    out << m_firstName << m_lastName << m_middleName << m_roll << m_className;
}

void Student::deserialize(QDataStream &in) {
    in >> m_firstName >> m_lastName >> m_middleName >> m_roll >> m_className;
}
