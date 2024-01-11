#include <QTcpServer>
#include <QTcpSocket>
#include <QTextStream>
#include <QCoreApplication>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    QTcpServer server;
    server.listen(QHostAddress::Any, 12345);

    QObject::connect(&server, &QTcpServer::newConnection, [&]() {
        QTcpSocket *clientSocket = server.nextPendingConnection();

        QObject::connect(clientSocket, &QTcpSocket::readyRead, [clientSocket]() {
            QTextStream stream(clientSocket);
            QString message = stream.readLine();
            qDebug() << "Message from client:" << message;

            stream << "Hello from server\n";
        });
    });

    return a.exec();
}
