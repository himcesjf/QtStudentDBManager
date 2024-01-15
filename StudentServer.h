#pragma once

#include <QObject>
#include <QPointer>

class Student;

class QTcpServer;
class QTcpSocket;

class StudentServer : public QObject {
    Q_OBJECT
    
public:
    explicit StudentServer(QObject *parent = nullptr);
    ~StudentServer();
    
public Q_SLOTS:
    void newStudentAdded(int id);
    void sendStudents(const QVector<Student*> &students);
    
Q_SIGNALS:
    void requestAllStudents();
    void studentReceived(Student *student) const;
    
private Q_SLOTS:
    void newClientConnected();
    void readFromClient();
    
private:
    QTcpServer *m_tcpServer;
    QPointer<QTcpSocket> m_currentConnection;
};
