/*
 * Header file for the Student class.
 * Declares the structure and functionalities of the Student objects used in the application.
 */


#pragma once

#include <QObject>
#include <QDataStream>
#include <QDebug>

class Student : public QObject
{
    Q_GADGET

    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(int version READ version WRITE setVersion)
    Q_PROPERTY(QString school READ school WRITE setSchool)
    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName)
    Q_PROPERTY(QString middleName READ middleName WRITE setMiddleName)
    Q_PROPERTY(QString lastName READ lastName WRITE setLastName)
    Q_PROPERTY(int roll READ roll WRITE setRoll)
    Q_PROPERTY(QString className READ className WRITE setClassName)

public:
    explicit Student(int id = -1,
        int version = 1,
        const QString &school = QString(),
        const QString &firstName = QString(),
        const QString &middleName = QString(),
        const QString &lastName = QString(),
        int roll = 0,
        const QString &className = QString());
    ~Student();

    int id() const;
    void setId(int id);

    int version() const;
    void setVersion(int version);

    QString school() const;
    void setSchool(const QString &school);

    QString firstName() const;
    void setFirstName(const QString &firstName);

    QString middleName() const;
    void setMiddleName(const QString &middleName);

    QString lastName() const;
    void setLastName(const QString &lastName);

    int roll() const;
    void setRoll(int roll);

    QString className() const;
    void setClassName(const QString &className);

    void updateFrom(const Student *other);

    // Serialization
    void serialize(QDataStream &out) const;
    void deserialize(QDataStream &in);

private:
    int m_id;
    int m_version;
    QString m_school;
    QString m_firstName;
    QString m_middleName;
    QString m_lastName;
    int m_roll;
    QString m_className;
};


inline QDataStream &operator<<(QDataStream &out, const Student &student)
{
    student.serialize(out);
    return out;
}

inline QDataStream &operator>>(QDataStream &in, Student &student)
{
    student.deserialize(in);
    return in;
}


inline QDebug operator<<(QDebug debug, const Student *student)
{
    QDebugStateSaver saver(debug);

    if (student) {
        debug.nospace() << "Student(ID: " << student->id()
            << ", Version: " << student->version()
            << ", School: " << student->school()
            << ", First Name: " << student->firstName()
            << ", Middle Name: " << student->middleName()
            << ", Last Name: " << student->lastName()
            << ", Roll: " << student->roll()
            << ", Class: " << student->className() << ')';
    } else {
        debug.nospace() << "Student: nullptr";
    }

    return debug;
}
