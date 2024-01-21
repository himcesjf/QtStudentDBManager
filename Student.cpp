/*
 * Implementation of the Student class.
 * Manages the data and behavior of student entities, including serialization for network transmission.
 */


#include "Student.h"

Student::Student(int id, int version, const QString &school,
    const QString &firstName, const QString &middleName, const QString &lastName,
    int roll, const QString &className)
    : QObject(nullptr) // We manage deletion manually.
    , m_id(id)
    , m_version(version)
    , m_school (school)
    , m_firstName(firstName)
    , m_middleName(middleName)
    , m_lastName(lastName)
    , m_roll(roll)
    , m_className(className)
{
}

Student::~Student()
{
}

int Student::id() const
{
    return m_id;
}

void Student::setId(int id) {
    m_id = id;
}

int Student::version() const
{
    return m_version;
}

void Student::setVersion(int version)
{
    m_version = version;
}

QString Student::school() const
{
    return m_school;
}

void Student::setSchool(const QString &school)
{
    m_school = school;
}

QString Student::firstName() const
{
    return m_firstName;
}

void Student::setFirstName(const QString &firstName)
{
    m_firstName = firstName;
}

QString Student::middleName() const
{
    return m_middleName;
}

void Student::setMiddleName(const QString &middleName)
{
    m_middleName = middleName;
}

QString Student::lastName() const
{
    return m_lastName;
}

void Student::setLastName(const QString &lastName)
{
    m_lastName = lastName;
}

int Student::roll() const
{
    return m_roll;
}

void Student::setRoll(int roll)
{
    m_roll = roll;
}

QString Student::className() const
{
    return m_className;
}

void Student::setClassName(const QString &className)
{
    m_className = className;
}

void Student::updateFrom(const Student *other)
{
    if (!other) {
        return;
    }

    setId(other->id());
    setVersion(other->version());
    setSchool(other->school());
    setFirstName(other->firstName());
    setMiddleName(other->middleName());
    setLastName(other->lastName());
    setRoll(other->roll());
    setClassName(other->className());
}

void Student::serialize(QDataStream &out) const
{
    out << m_id << m_version << m_school << m_firstName << m_lastName << m_middleName << m_roll << m_className;
}

void Student::deserialize(QDataStream &in)
{
    in >> m_id >> m_version >> m_school >> m_firstName >> m_lastName >> m_middleName >> m_roll >> m_className;
}
