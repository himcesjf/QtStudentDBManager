#pragma once

#include <QObject>
#include <QDataStream>

class Student : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName)
    Q_PROPERTY(QString middleName READ middleName WRITE setMiddleName)
    Q_PROPERTY(QString lastName READ lastName WRITE setLastName)
    Q_PROPERTY(int roll READ roll WRITE setRoll)
    Q_PROPERTY(QString className READ className WRITE setClassName)

public:
    explicit Student(
                     QObject *parent = nullptr,
                     int id = 0,
                     const QString &firstName = QString(),
                     const QString &middleName = QString(),
                     const QString &lastName = QString(),
                     int roll = 0,
                     const QString &className = QString()
                     );


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

    // Serialization
    void serialize(QDataStream &out) const;
    void deserialize(QDataStream &in);

private:
    QString m_firstName;
    QString m_middleName;
    QString m_lastName;
    int m_roll;
    QString m_className;
    int m_id;
};

inline QDataStream &operator<<(QDataStream &out, const Student &student) {
    student.serialize(out);
    return out;
}

inline QDataStream &operator>>(QDataStream &in, Student &student) {
    student.deserialize(in);
    return in;
}
