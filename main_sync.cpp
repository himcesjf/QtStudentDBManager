/*
 * Entry point for the sync client application of QtStudentDBManager.
 */

#include "ClientDatabase.h"
#include "NetworkMessage.h"
#include "Settings.h"

#include <QCoreApplication>
#include <QSqlQuery>
#include <QTcpSocket>

int main(int argc, char *argv[])
{
    QCoreApplication app {argc, argv};
    app.setApplicationName(QStringLiteral("QtStudentDBManager"));

    Settings &settings = Settings::instance();

    // Init local database.
    if (!ClientDatabase::createTables()) {
        return 1;
    }

    QTcpSocket *socket {new QTcpSocket(&app)};

    socket->connectToHost(QHostAddress(settings.value("client/serverHost").toString()),
        settings.value("client/serverPort").toUInt());

    qDebug() << QString("Connecting to server %1:%2 ...")
        .arg(settings.value("client/serverHost").toString())
        .arg(settings.value("client/serverPort").toUInt());

    QObject::connect(socket, &QTcpSocket::connected, &app, [&]() {
        qDebug() << QString("Connected to server %1:%2.")
        .arg(socket->peerAddress().toString())
        .arg(socket->peerPort());

        // Send a greeting message.
        QDataStream out(socket);
        out.setVersion(QDataStream::Qt_6_0);
        out << NetworkMessage(NetworkMessage::StatusMessage, "Hello server!");

        qDebug() << "Beginning sync table replay transmission.";

        QSqlQuery syncQuery("SELECT * FROM sync");

        if (!syncQuery.next()) {
            qDebug() << "Sync table is empty.";
        } else {
            int lastId = -1;

            do {
                qDebug() << "Replaying record.";
                out << NetworkMessage(NetworkMessage::StudentRecord,
                    syncQuery.value("student").toByteArray());
                lastId = syncQuery.value("id").toInt();
            } while (syncQuery.next());

            qDebug() << "Sync to server complete. Id of last record transmitted:" << lastId;

            QSqlQuery cleanupQuery;
            cleanupQuery.prepare("DELETE FROM sync WHERE id <= :lastId");
            cleanupQuery.bindValue(":lastId", lastId);
            cleanupQuery.exec();

            if (cleanupQuery.exec()) {
                qDebug() << "Removed synchronized student records from sync table.";
            } else {
                qDebug() << "Error: Could not remove synchronized student records from sync table.";
            }
        }

        // Disconnect.
        socket->disconnectFromHost();
        socket->waitForDisconnected(1000);

        qDebug() << "Disconnected from server.";

        app.quit();
    });

    return app.exec();
}
