/*
 * Header file for the NetworkMessage class.
 * Declares the structure and functionalities for network message handling in the application.
 */


#pragma once

#include <QDataStream>
#include <QString>
#include <QVariant>

class Student;

class NetworkMessage 
{
public:
    enum MessageType {
        StatusMessage,
        StorageConfirmation,
        StudentObject,
        SchoolRequest
    };

    NetworkMessage(MessageType type = StatusMessage,
        const QVariant &payload = QVariant());
    ~NetworkMessage();

    MessageType type() const;
    QVariant payload() const;

    // Serialization
    void serialize(QDataStream &out) const;
    void deserialize(QDataStream &in);

private:
    MessageType m_type;
    QVariant m_payload;
};

inline QDataStream &operator<<(QDataStream &out, const NetworkMessage &message)
{
    message.serialize(out);
    return out;
}

inline QDataStream &operator>>(QDataStream &in, NetworkMessage &message)
{
    message.deserialize(in);
    return in;
}
