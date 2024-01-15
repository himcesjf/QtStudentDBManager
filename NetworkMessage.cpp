#include "NetworkMessage.h"
#include "Student.h"

NetworkMessage::NetworkMessage(MessageType type, const QVariant &payload)
    : m_type {type}
    , m_payload {payload}
{
}

NetworkMessage::~NetworkMessage()
{
}

NetworkMessage::MessageType NetworkMessage::type() const
{
    return m_type;
}

QVariant NetworkMessage::payload() const 
{
    return m_payload;
}

void NetworkMessage::serialize(QDataStream &out) const
{
    out << static_cast<qint32>(m_type);
    out << m_payload;
}

void NetworkMessage::deserialize(QDataStream &in) 
{
    in >> m_type;
    in >> m_payload;
}
