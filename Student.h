#ifndef STUDENT_H
#define STUDENT_H

#include <QObject>
#include <QDataStream>

class Student : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName)
    Q_PROPERTY(QString lastName READ lastName WRITE setLastName)

public:
    explicit Student(QObject *parent = nullptr, const QString &firstName = QString(), const QString &lastName = QString());

    QString firstName() const;
    void setFirstName(const QString &firstName);

    QString lastName() const;
    void setLastName(const QString &lastName);

    // Serialization
    void serialize(QDataStream &out) const;
    void deserialize(QDataStream &in);

private:
    QString m_firstName;
    QString m_lastName;
};

inline QDataStream &operator<<(QDataStream &out, const Student &student) {
    student.serialize(out);
    return out;
}

inline QDataStream &operator>>(QDataStream &in, Student &student) {
    student.deserialize(in);
    return in;
}

#endif // STUDENT_H
