#pragma once

#include <QAbstractTableModel>
#include "Student.h"

class Settings;
class QTcpSocket;

class StudentModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit StudentModel(QObject *parent = nullptr);
    ~StudentModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    void addStudent(Student *student);
    void clearStudents();

    Q_INVOKABLE void connectToServer();

public slots:
    Q_INVOKABLE void sendDataToServer();

private Q_SLOTS:
    void loadFromServer();

private:
    QTcpSocket *m_socket;
    QVector<Student*> m_students;
};
