#include "StudentModel.h"
#include <algorithm>

StudentModel::StudentModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

StudentModel::~StudentModel()
{
    qDeleteAll(m_students);
    m_students.clear();
}

int StudentModel::rowCount(const QModelIndex & /*parent*/) const
{
    return m_students.size();
}

int StudentModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 5; // First Name, Middle Name, Last Name, Roll, Class Name
}

QVariant StudentModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const Student *student = m_students.at(index.row());
    switch (index.column()) {
    case 0:
        return student->firstName();
    case 1:
        return student->middleName();
    case 2:
        return student->lastName();
    case 3:
        return student->roll();
    case 4:
        return student->className();
    default:
        return QVariant();
    }
}

QVariant StudentModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();

    switch (section) {
        case 0:
            return "First Name";
        case 1:
            return "Middle Name";
        case 2:
            return "Last Name";
        case 3:
            return "Roll";
        case 4:
            return "Class";
        default:
            return QVariant();
    }
}


void StudentModel::addStudent(Student *student)
{
    beginInsertRows(QModelIndex(), m_students.size(), m_students.size());
    m_students.append(student);
    endInsertRows();
}

void StudentModel::clearStudents()
{
    beginRemoveRows(QModelIndex(), 0, m_students.size() - 1);
    qDeleteAll(m_students);
    m_students.clear();
    endRemoveRows();
}
