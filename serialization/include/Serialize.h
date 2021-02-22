#pragma once
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QByteArray>
#include <QtWidgets/qfiledialog.h>
#include <QtCore/QStringList>

class Serialize
{
private:
    QJsonObject m_messageData;
public:
    Serialize() = default;
    void pushData(QStringList &messagesList);
    void updateSerialize(QStringList &messagesList);
    ~Serialize();
};
