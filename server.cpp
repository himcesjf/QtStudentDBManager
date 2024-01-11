#include <QTcpServer>
#include <QTcpSocket>
#include <QTextStream>
#include <QCoreApplication>

int main(int argc, char *argv[]) {
    // Initialize the Qt core application
    QCoreApplication a(argc, argv);

    // Create a TCP server
    QTcpServer server;

    // Make the server listen for incoming connections on any IP address, port 12345
    server.listen(QHostAddress::Any, 12345);

    // Connect the signal that indicates a new connection to a lambda function
    QObject::connect(&server, &QTcpServer::newConnection, [&]() {
        // When a new connection is established

        // Accept the connection and get a socket for communicating with the client
        QTcpSocket *clientSocket = server.nextPendingConnection();

        // Connect the signal indicating there's data ready to be read to another lambda function
        QObject::connect(clientSocket, &QTcpSocket::readyRead, [clientSocket]() {
            // When client data is ready to be read

            // Create a text stream associated with the client socket
            QTextStream stream(clientSocket);
            // Send a message to the client
            stream << "Hello from server\n";

            // Read a message from the client
            QString message = stream.readLine();
            // Print the received message to the debug console
            qDebug() << "Message from client:" << message;
        });
    });

    // Enter the main event loop
    return a.exec();
}
