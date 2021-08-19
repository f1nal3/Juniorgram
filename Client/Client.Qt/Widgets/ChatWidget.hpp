#pragma once
#include <Network/Primitives.hpp>

#include "Widgets/InputFields.hpp"
#include "Widgets/MessageWidget.hpp"
#include "Widgets/TextEdit.hpp"

class ChatWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChatWidget(QWidget* parent = nullptr);
    void setChannelID(const std::uint64_t channelID) { _channelID = channelID; }

private slots:
    void newMessage(const QString& messageText);
    void addMessages(const std::vector<Network::MessageInfo>& messages);

private:
    std::unique_ptr<QVBoxLayout> _mainChatLayout;
    std::unique_ptr<ChatHistory> _chatHistory;
    std::unique_ptr<TextEdit>    _textEdit;
    std::unique_ptr<QTimer>      _requestTimer;
    std::uint64_t                _channelID;
};
