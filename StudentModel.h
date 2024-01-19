/*
 * Header file for the StudentModel class.
 * Declares the structure and functionalities of the model used in the client's GUI for representing student data.
 */


#pragma once

#include "Student.h"

#include <QAbstractTableModel>
#include <QPointer>

class Settings;
class QTcpSocket;

class StudentModel : public QAbstractTableModel
{
    Q_OBJECT
    
    Q_PROPERTY(bool error READ error NOTIFY errorChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)

public:
    explicit StudentModel(QObject *parent = nullptr);
    ~StudentModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void updateStudent(int id, const QString &firstName,
        const QString &middleName, const QString &lastName, int roll, const QString &className, const QString &schoolName);
    
    Q_INVOKABLE void connectToServer();
    
    bool error() const;
    QString errorString() const;
    Q_INVOKABLE void resetError();

    
Q_SIGNALS:
    void storageSuccess() const;
    void errorChanged() const;
    void errorStringChanged() const;

private Q_SLOTS:
    void readFromServer();
    
    void sendToServer(Student *student);
    void checkStorageConfirmed();

private:
    void insertStudent(Student *student);
    void clearStudents();
    
    QVector<Student*> m_students;
    
    QTcpSocket *m_socket;
    
    QPointer<Student> m_lastStudentSent;
    bool m_storageConfirmed;
    
    bool m_error;
    QString m_errorString;
};
