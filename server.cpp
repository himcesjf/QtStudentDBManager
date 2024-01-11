#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDataStream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");  // Set the hostname
    db.setDatabaseName("mydb");  // Set the database name
    db.setUserName("myuser");  // Set the username
    db.setPassword("myuser");  // Set the password

    if (!db.open()) {
        qWarning() << "Database connection failed:" << db.lastError();
        return -1;
    }

    // Retrieve data from the table
    QSqlQuery query;
    if (!query.exec("SELECT * FROM students")) {
        qWarning() << "Failed to query the database:" << query.lastError();
        db.close();
        return -1;
    }

    // Setting up the server
    QTcpServer server;
    if (!server.listen(QHostAddress::Any, 12345)) {
        qWarning() << "Server failed to start:" << server.errorString();
        db.close();
        return -1;
    }

    QObject::connect(&server, &QTcpServer::newConnection, [&]() {
        QTcpSocket *clientSocket = server.nextPendingConnection();

        QObject::connect(clientSocket, &QTcpSocket::disconnected, clientSocket, &QTcpSocket::deleteLater);

        QDataStream outStream(clientSocket);
        outStream.setVersion(QDataStream::Qt_5_15);  // Qt version

        // Serializing table data
        while (query.next()) {
            outStream << query.value("id").toInt()
                      << query.value("firstName").toString()
                      << query.value("lastName").toString()
                      << query.value("middleName").toString()
                      << query.value("roll").toInt()
                      << query.value("class").toString();
        }

        clientSocket->disconnectFromHost();
    });

    return a.exec();
}
