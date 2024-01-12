//Student.cpp
#include "Student.h"

Student::Student(QObject *parent, const QString &firstName, const QString &lastName)
    : QObject(parent), m_firstName(firstName), m_lastName(lastName)
{
}

QString Student::firstName() const {
    return m_firstName;
}

void Student::setFirstName(const QString &firstName) {
    m_firstName = firstName;
}

QString Student::lastName() const {
    return m_lastName;
}

void Student::setLastName(const QString &lastName) {
    m_lastName = lastName;
}

void Student::serialize(QDataStream &out) const {
    out << m_firstName << m_lastName;
}

void Student::deserialize(QDataStream &in) {
    in >> m_firstName >> m_lastName;
}
