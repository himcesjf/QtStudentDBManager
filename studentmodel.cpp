#include "studentmodel.h"

#include <QSqlQuery>

StudentModel::StudentModel(QObject *parent)
    : QSqlTableModel(parent)
{
    if (!database().open()) {
        qCritical() << "Error opening database.";
    }

    createTable();

    setTable(QStringLiteral("students"));
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    select();

    setHeaderData(0, Qt::Horizontal, QStringLiteral("Id"));
    setHeaderData(1, Qt::Horizontal, QStringLiteral("First name"));
    setHeaderData(2, Qt::Horizontal, QStringLiteral("Last name"));
    setHeaderData(3, Qt::Horizontal, QStringLiteral("Middle name"));
    setHeaderData(4, Qt::Horizontal, QStringLiteral("Roll"));
    setHeaderData(5, Qt::Horizontal, QStringLiteral("Class"));

    seedInitialStudents();
}

StudentModel::~StudentModel()
{
    database().close();
}

QHash<int, QByteArray> StudentModel::roleNames() const
{
    QHash<int, QByteArray> roles {QAbstractItemModel::roleNames()}; 

    for (uint i = 0; i < 7; ++i) {
        const QVariant &header = headerData(i, Qt::Horizontal);
        const QString &roleName = header.toString().simplified().remove(' ').toLower();
        roles[Qt::UserRole + 1 + i] = roleName.toLatin1();
    }

    return roles;
}

void StudentModel::createTable()
{
    QSqlQuery query;
    query.exec(
        "CREATE TABLE IF NOT EXISTS students ("
        "id INTEGER PRIMARY KEY, "
        "firstName TEXT, "
        "lastName TEXT, "
        "middleName TEXT, "
        "roll INTEGER, "
        "class TEXT)"
    );
}

void StudentModel::seedInitialStudents()
{
    // Don't seed if the db already holds students.
    if (rowCount()) {
        return;
    }

    QSqlQuery query;
    query.prepare(
        "INSERT INTO students (firstName, lastName, middleName, roll, class) "
        "VALUES (?, ?, ?, ?, ?)"
    );
    query.addBindValue(QLatin1String("Foo"));
    query.addBindValue(QLatin1String("Baz"));
    query.addBindValue(QLatin1String("Bar"));
    query.addBindValue(101);
    query.addBindValue(QLatin1String("Science"));
    query.exec();

    select();

    qDebug() << "Table seeded.";
}