#pragma once
#include <Network/Primitives.hpp>

#include "Widgets/InputFields.hpp"
#include "Widgets/MessageWidget.hpp"
#include "Widgets/TextEdit.hpp"

/** 
 *  @class ChatWidget
 *  @brief chatWidget stores and displays messages and replies.
 */
class ChatWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructor for ñhat widget
     * @param parent Parent widget
     */
    explicit ChatWidget(QWidget* parent = nullptr);
    /**
     * @brief Method for ñhat widget
     * @param ID of channel. It's format uint64_t
     */
    void setChannelID(const std::uint64_t channelID) { _channelID = channelID; }

private slots:
    void newMessage(const QString& messageText);
    void addReplyWidget(ReplyWidget* reply);
    void addMessages(const std::vector<Network::MessageInfo>& messages);
    void addReplies(const std::vector<Network::ReplyInfo>& replies);

    void requestMessages();
    void requestReplies();

private:
    ReplyWidget* _replyWidget;
    std::unique_ptr<QVBoxLayout> _mainChatLayout;
    std::unique_ptr<ChatHistory> _chatHistory;
    std::unique_ptr<TextEdit>    _textEdit;
    std::unique_ptr<QTimer>      _requestTimer;
    std::uint64_t                _channelID;
};
