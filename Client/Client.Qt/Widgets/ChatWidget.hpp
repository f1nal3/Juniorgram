#pragma once
#include <Models/Primitives.hpp>

#include "Widgets/ChannelBar.hpp"
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
     * @brief Constructor for chat widget
     * @param parent Parent widget
     */
    explicit ChatWidget(std::string_view channelName, QWidget* parent = nullptr);
    /**
     * @brief Method for chat widget
     * @param ID of channel. It's format uint64_t
     */
    void setChannelID(const std::uint64_t channelID) { _channelID = channelID; }
    void setChannelName(std::string_view channelName) 
    { 
        _channelName = channelName;
        _channelBar->setChannelName(QString::fromStdString(_channelName));
    }

    const std::string& getChannelName() const { return _channelName; }

protected:
    /// Handle resize
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void newMessage(const QString& messageText) const;
    void setReply(QString messageText, QString username, uint64_t messageId) const;
    void addMessages(const std::vector<Models::MessageInfo>& messages) const;
    void addReplies(const std::vector<Models::ReplyInfo>& replies)const ;

    void requestMessages() const;
    void updateLayout() const;

signals:
    /// On delete button click
    void removeChannel();

private:
    std::unique_ptr<ChannelBar>  _channelBar;
    std::unique_ptr<ReplyWidget> _replyWidget;
    std::unique_ptr<ChatHistory> _chatHistory;
    std::unique_ptr<TextEdit>    _textEdit;

    std::unique_ptr<QTimer> _requestTimer;
    std::uint64_t           _channelID{};
    std::string             _channelName;
};
