#include "StudentModel.h"
#include <QDebug>

StudentModel::StudentModel(QObject *parent) : QAbstractListModel(parent) {
    // Load initial data from the database
    auto allStudents = dbOps.displayAllStudents();
    for (const auto& student : allStudents) {
        students.append(student);
    }
}

int StudentModel::rowCount(const QModelIndex &parent) const {
    return students.count();
}

QVariant StudentModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= students.count())
        return QVariant();

    const Student &student = students.at(index.row());
    switch (role) {
        case IdRole:
            return student.getId();
        case FirstNameRole:
            return student.getFirstName();
        case LastNameRole:
            return student.getLastName();
        case MiddleNameRole:
            return student.getMiddleName();
        case RollRole:
            return student.getRoll();
        case ClassRole:
            return student.getClass();
        case SubjectMarksRole:
            return QVariant::fromValue(student.getSubjectMarks());
        default:
            return QVariant();
    }
}

QHash<int, QByteArray> StudentModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "studentId";
    roles[FirstNameRole] = "firstName";
    roles[LastNameRole] = "lastName";
    roles[MiddleNameRole] = "middleName";
    roles[RollRole] = "roll";
    roles[ClassRole] = "class";
    roles[SubjectMarksRole] = "subjectMarks";
    return roles;
}


Student StudentModel::getStudentById(int id) const {
    for (const auto& student : students) {
        if (student.getId() == id) {
            return student;
        }
    }
    return Student(); // Return an empty Student object if not found
}

void StudentModel::addStudent(const QString& firstName, const QString& lastName) {
    Student newStudent;
    newStudent.setFirstName(firstName);
    newStudent.setLastName(lastName);
    dbOps.insertStudent(newStudent);
    // Refresh the model data - reloads the entire student list from the database
    beginResetModel();
    students = dbOps.displayAllStudents();
    endResetModel();
}

void StudentModel::updateStudent(int id, const QString& firstName, const QString& lastName) {
    // Find the student, update details, and call dbOps.updateStudent
    for (auto& student : students) {
        if (student.getId() == id) {
            student.setFirstName(firstName);
            student.setLastName(lastName);
            dbOps.updateStudent(student);
            break;
        }
    }
    // Refresh the model
    beginResetModel();
    students = dbOps.displayAllStudents();
    endResetModel();
}

void StudentModel::deleteStudent(int id) {
    dbOps.deleteStudent(id);
    // Refresh the model
    beginResetModel();
    students = dbOps.displayAllStudents();
    endResetModel();
}
