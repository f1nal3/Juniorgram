#include "JsonSerializer.hpp"

void JsonSerializer::updateSerialize(QStringList &messagesList)
{
    QFile jsonFile("text.json");
    if(!jsonFile.open(QIODevice::ReadOnly))
    {
           return;
    }
    QByteArray saveData = jsonFile.readAll();
    QJsonDocument jsonDocument(QJsonDocument::fromJson(saveData));
    m_messageData = jsonDocument.object();
    QJsonArray jsonArray = m_messageData["texts"].toArray();
    foreach(QJsonValue value, jsonArray)
    {
        QJsonObject obj = value.toObject();
        messagesList.append(obj["Message"].toString());
    }
}

void JsonSerializer::pushData(QStringList &messagesList)
{
    QJsonObject json;
    json["Message"] = messagesList.back();
    QJsonArray jsonArray = m_messageData["texts"].toArray();
    jsonArray.append(json);
    m_messageData["texts"] = jsonArray;
}

JsonSerializer::~JsonSerializer()
{
    QFile jsonFile("text.json");
    if(!jsonFile.open(QIODevice::WriteOnly))
         {
            return;
         }
    jsonFile.write(QJsonDocument(m_messageData).toJson(QJsonDocument::Indented));
    jsonFile.close();
}
