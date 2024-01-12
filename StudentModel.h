#pragma once

#include <QAbstractTableModel>
#include "Student.h"

class Settings;

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

    void addStudent(Student *student);
    void clearStudents();

private:
    void loadFromServer();

    QVector<Student*> m_students;
};
