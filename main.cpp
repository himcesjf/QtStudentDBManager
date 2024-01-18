#include <QCoreApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    // Configure your PostgreSQL connection details
    const QString HOST = "localhost";
    const int PORT = 5432;
    const QString DB_NAME = "studentsdb";
    const QString USER = "myuser";
    const QString PASSWORD = "myuser";

    // Establishing connection to the database
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(HOST);
    db.setPort(PORT);
    db.setDatabaseName(DB_NAME);
    db.setUserName(USER);
    db.setPassword(PASSWORD);

    if (!db.open()) {
        qDebug() << "Error: connection to database failed";
        return -1;
    } else {
        qDebug() << "Database: connection ok";
    }

    QSqlQuery query;

    // Creating a table
    if (!query.exec("CREATE TABLE IF NOT EXISTS greetings (id SERIAL PRIMARY KEY, text VARCHAR(255))")) {
        qDebug() << "Failed to create table:" << query.lastError();
        return -1;
    }

    // Inserting data
    if (!query.exec("INSERT INTO greetings (text) VALUES ('Hello'), ('World')")) {
        qDebug() << "Failed to insert data:" << query.lastError();
        return -1;
    }

    // Querying data
    if (!query.exec("SELECT * FROM greetings")) {
        qDebug() << "Failed to query data:" << query.lastError();
        return -1;
    }

    // Printing data
    while (query.next()) {
        QString id = query.value(0).toString();
        QString text = query.value(1).toString();
        qDebug() << id << ": " << text;
    }

    return app.exec();
}
