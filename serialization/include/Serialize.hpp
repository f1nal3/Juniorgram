// TODO : add header guard
#pragma once
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QFileDialog>
#include <QStringList>

class Serialize
{
private:
    QJsonObject m_messageData;
public:
    Serialize();
    void pushData(QStringList &messagesList);
    void updateSerialize(QStringList &messagesList);
    ~Serialize();
};
