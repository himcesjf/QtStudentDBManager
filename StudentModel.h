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

    Q_PROPERTY(QString school READ school WRITE setSchool NOTIFY schoolChanged)
    Q_PROPERTY(bool error READ error NOTIFY errorChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)

public:
    explicit StudentModel(QObject *parent = nullptr);
    ~StudentModel();

    void classBegin() override;
    void componentComplete() override;

    QString school() const;
    void setSchool(const QString &school);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void updateStudent(int id, int version, const QString &firstName,
        const QString &middleName, const QString &lastName, int roll, const QString &className);

    Q_INVOKABLE void connectToServer();

    bool error() const;
    QString errorString() const;
    Q_INVOKABLE void resetError();
    Q_INVOKABLE void resetUpdated();

Q_SIGNALS:
    void schoolChanged() const;
    void storageSuccess() const;
    void errorChanged() const;
    void errorStringChanged() const;

private Q_SLOTS:
    void readFromServer();

private:
    void refreshFromLocalDatabase(bool silent = false);

    void insertStudent(Student *student);
    void clearStudents();

    QString m_school;

    QVector<Student*> m_students;
    QVector<int> m_updatedFromLocal;

    QTcpSocket *m_socket;

    bool m_error;
    QString m_errorString;
};
