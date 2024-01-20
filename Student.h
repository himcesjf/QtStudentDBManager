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
    Q_OBJECT
    
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName)
    Q_PROPERTY(QString middleName READ middleName WRITE setMiddleName)
    Q_PROPERTY(QString lastName READ lastName WRITE setLastName)
    Q_PROPERTY(int roll READ roll WRITE setRoll)
    Q_PROPERTY(QString className READ className WRITE setClassName)
    Q_PROPERTY(QString schoolName READ schoolName WRITE setSchoolName)

public:
    explicit Student(QObject *parent = nullptr,
        int id = -1,
        const QString &firstName = QString(),
        const QString &middleName = QString(),
        const QString &lastName = QString(),
        int roll = 0,
        const QString &className = QString(),
        const QString &schoolName = QString(),
        int version =1);
    ~Student();

    int id() const;
    void setId(int id);

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

    QString schoolName() const;
    void setSchoolName(const QString &schoolName);

    int version() const;
    void setVersion(int version);

    // Serialization
    void serialize(QDataStream &out) const;
    void deserialize(QDataStream &in);

private:
    QString m_firstName;
    QString m_middleName;
    QString m_lastName;
    int m_roll;
    QString m_className;
    QString m_schoolName;
    int m_id;
    int m_version;
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
            << ", First Name: " << student->firstName()
            << ", Middle Name: " << student->middleName()
            << ", Last Name: " << student->lastName()
            << ", Roll: " << student->roll()
            << ", Class Name: " << student->className()
            << ", School Name: " << student->schoolName()
            << ", Version: " << student->version() << ')';
    } else {
        debug.nospace() << "Student: nullptr";
    }

    return debug;
}
