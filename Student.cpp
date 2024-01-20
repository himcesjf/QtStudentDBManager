/*
 * Implementation of the Student class.
 * Manages the data and behavior of student entities, including serialization for network transmission.
 */


#include "Student.h"

Student::Student(QObject *parent, int id, const QString &firstName, const QString &middleName, 
    const QString &lastName, int roll, const QString &className, const QString &schoolName, int version)
    : QObject(parent)
    , m_id(id)
    , m_firstName(firstName)
    , m_middleName(middleName)
    , m_lastName(lastName)
    , m_roll(roll)
    , m_className(className)
    , m_schoolName(schoolName)
    , m_version(version)
{
}

Student::~Student()
{
}

int Student::id() const {
    return m_id;
}
void Student::setId(int id) {
    m_id = id;
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
    m_className = className;
}

QString Student::schoolName() const {
    return m_schoolName;
}
void Student::setSchoolName(const QString &schoolName) {
    m_schoolName = schoolName;
}

int Student::version() const {
    return m_version;
}
void Student::setVersion(int version) {
    m_version = version;
}

void Student::serialize(QDataStream &out) const {
    out << m_id << m_firstName << m_lastName << m_middleName << m_roll << m_className << m_schoolName << m_version;
}

void Student::deserialize(QDataStream &in) {
    in >> m_id >> m_firstName >> m_lastName >> m_middleName >> m_roll >> m_className << m_schoolName << m_version;
}
