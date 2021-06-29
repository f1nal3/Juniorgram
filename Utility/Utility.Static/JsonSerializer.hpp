#pragma once
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QByteArray>
#include <QtWidgets/qfiledialog.h>
#include <QtCore/QStringList>


namespace Utility
{
class JsonSerializer
{
private:
    QJsonObject m_messageData;

public:
    JsonSerializer() = default;
    void pushData(QStringList& messagesList);
    void updateSerialize(QStringList& messagesList);
    ~JsonSerializer();
};
}  // namespace Utility
