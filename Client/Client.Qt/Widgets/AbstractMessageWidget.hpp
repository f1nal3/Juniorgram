#pragma once

#include <memory>
#include <QWidget>
#include <QString>

#include "Widgets/InputFields.hpp"
#include "Style/StyleWidgets.hpp"

class AbstractMessageWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief constructor for displaying a message from a user on the screen.
     * @param history parent widget
     * @param message Message from user.
     * @param messageId of message.
     * @param username User nickname.
     * @param userId of user.
     * @param st Set of widget's Style properties
     */
    AbstractMessageWidget(QWidget* history, QString message, uint64_t messageID, QString username, uint64_t userID,
                          const Style::MessageWidget& st = st::defaultMessageWidget);
    
    /// Possible height of message widget
    virtual int expectedHeight() const = 0;

signals:
    /// Signal for change reply message geometry
    void geometryChanged(int);

protected:
    // Getters and setters for derived classes
    uint64_t getUserID()                   const { return _userID; };
    virtual uint64_t getMessageID()        const { return _messageID; }
    const QString&  getMessageText()       const { return _messageText; }
    const QString&  getUserName()          const { return _userName; }

    const Style::MessageWidget& getStyle() const { return _st; }

    void setUserID(uint64_t uID)          { _userID = uID; }
    void setMessageID(uint64_t msgID)     { _messageID = msgID; }
    void setMessageText(QString msg)      { _messageText = std::move(msg); }
    void setUserName(QString usrName)     { _userName = std::move(usrName); }

    FlatTextEdit* getFmtMessageText() const { return _fmtMessageText.get(); }

private:
    std::unique_ptr<FlatTextEdit> _fmtMessageText;

    uint64_t    _messageID;
    uint64_t    _userID;

    QString     _messageText;
    QString     _userName;

    const Style::MessageWidget& _st;
};
