#ifndef STUDENTMODEL_H
#define STUDENTMODEL_H

#include <QAbstractListModel>
#include "Student.h"
#include "DatabaseOperations.h"

class StudentModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum StudentRoles {
        IdRole = Qt::UserRole + 1,
        FirstNameRole,
        LastNameRole,
        MiddleNameRole,
        RollRole,
        ClassRole,
        SubjectMarksRole
    };

    StudentModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE Student getStudentById(int id) const;
    Q_INVOKABLE void addStudent(const QString& firstName, const QString& lastName);
    Q_INVOKABLE void updateStudent(int id, const QString& firstName, const QString& lastName);
    Q_INVOKABLE void deleteStudent(int id);

private:
    DatabaseOperations dbOps;
    QList<Student> students;
};

#endif // STUDENTMODEL_H
