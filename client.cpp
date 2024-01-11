#include <QTcpSocket>
#include <QTextStream>
#include <QCoreApplication>
#include <QTimer>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    QTcpSocket socket;
    socket.connectToHost("localhost", 12345);

    QObject::connect(&socket, &QTcpSocket::connected, [&]() {
        QTextStream stream(&socket);
        stream << "Hello from client\n";
    });

    QObject::connect(&socket, &QTcpSocket::readyRead, [&]() {
        QTextStream stream(&socket);
        qDebug() << "Message from server:" << stream.readLine();
        a.quit();
    });

    return a.exec();
}
