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
    return 2; // First Name, Last Name
}

QVariant StudentModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    const Student *student = m_students.at(index.row());
    if (index.column() == 0)
        return student->firstName();
    else if (index.column() == 1)
        return student->lastName();

    return QVariant();
}

QVariant StudentModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();

    if (section == 0)
        return "First Name";
    else if (section == 1)
        return "Last Name";

    return QVariant();
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
