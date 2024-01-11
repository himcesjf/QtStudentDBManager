#include <QTcpSocket>
#include <QTextStream>
#include <QCoreApplication>
#include <QTimer>

int main(int argc, char *argv[]) {
    // Initialize the Qt core application
    QCoreApplication a(argc, argv);

    // Create a TCP socket for the client
    QTcpSocket socket;
    // Connect the socket to a server listening on 'localhost' and port 12345
    socket.connectToHost("localhost", 12345);

    // Connect a lambda function to the socket's 'connected' signal
    QObject::connect(&socket, &QTcpSocket::connected, [&]() {
        // This code is executed once the socket successfully connects to the server

        // Create a text stream associated with the socket
        QTextStream stream(&socket);
        // Send a message through the socket to the server
        stream << "Hello from client\n";
    });

    // Connect a lambda function to the socket's 'readyRead' signal
    QObject::connect(&socket, &QTcpSocket::readyRead, [&]() {
        // This code is executed when there is data from the server ready to be read

        // Create a text stream to read data from the socket
        QTextStream stream(&socket);
        // Read a line of text sent from the server and output it to the debug console
        qDebug() << "Message from server:" << stream.readLine();
        // Quit the application after receiving the message
        a.quit();
    });

    // Enter the main event loop
    return a.exec();
}
