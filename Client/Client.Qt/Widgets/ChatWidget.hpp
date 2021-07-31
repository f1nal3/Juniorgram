#pragma once
#include "Widgets/InputFields.hpp"
#include "Widgets/MessageWidget.hpp"
#include "Widgets/TextEdit.hpp"

class ChatWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChatWidget(QWidget* parent = nullptr);
    void setChannelID(const std::uint64_t channeID) { _channelID = channeID; }

private slots:
    void newMessage(const QString& messageText);
    void newMessage(const QString& messageText, const QString& userNameMessage);

private:
    std::unique_ptr<QVBoxLayout> _mainChatLayout;
    std::unique_ptr<ChatHistory> _chatHistory;
    std::unique_ptr<TextEdit>    _textEdit;
    std::uint64_t                _channelID;
};
