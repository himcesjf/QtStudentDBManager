/*
 * Header file for the StudentModel class.
 * Declares the structure and functionalities of the model used in the client's GUI for representing student data.
 */


#pragma once

#include "Student.h"

#include <QAbstractTableModel>
#include <QPointer>
#include <QQmlParserStatus>

class Settings;
class QTcpSocket;

class StudentModel : public QAbstractTableModel, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QString cliSchoolName READ getCliSchoolName CONSTANT)
    Q_PROPERTY(QString school READ getSchool WRITE setSchool NOTIFY schoolChanged)
    Q_PROPERTY(bool error READ error NOTIFY errorChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)

public:
    explicit StudentModel(QObject *parent = nullptr, const QString &schoolName = QString());
    ~StudentModel();

    QString getCliSchoolName() const;

    QString getSchool() const;
    void setSchool(const QString &school);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void updateStudent(int id, const QString &firstName,
        const QString &middleName, const QString &lastName, int roll, const QString &className/*, const QString &schoolName*/);
    
    Q_INVOKABLE void connectToServer();
    
    bool error() const;
    QString errorString() const;
    Q_INVOKABLE void resetError();

    void classBegin() override;
    void componentComplete() override;

    //Local database operations
    void insertStudentToDB(const Student &student);
    void updateStudentInDB(const Student &student);
    void deleteStudentFromDB(int studentId);
    void loadStudentsFromDB();

    
Q_SIGNALS:
    void schoolChanged();
    void storageSuccess() const;
    void errorChanged() const;
    void errorStringChanged() const;

private Q_SLOTS:
    void readFromServer();
    
    void sendToServer(Student *student);
    void checkStorageConfirmed();

private:
    QString m_school;
    QString m_cliSchoolName;
    void insertStudent(Student *student);
    void clearStudents();
    
    QVector<Student*> m_students;
    
    QTcpSocket *m_socket;
    
    QPointer<Student> m_lastStudentSent;
    bool m_storageConfirmed;
    
    bool m_error;
    QString m_errorString;
};
