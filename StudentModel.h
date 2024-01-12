//StudentModel.h
#ifndef STUDENTMODEL_H
#define STUDENTMODEL_H

#include <QAbstractTableModel>
#include "Student.h"

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
    QVector<Student*> m_students;
};

#endif // STUDENTMODEL_H
