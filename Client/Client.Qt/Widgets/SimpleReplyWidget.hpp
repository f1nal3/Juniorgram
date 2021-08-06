#pragma once

#include <QWidget>
#include <QPainter>
#include <memory>

#include "Widgets/InputFields.hpp"
#include "Widgets/ReactionLayout.hpp"

class SimpleReplyWidget : public QWidget
{
public:
    SimpleReplyWidget(QWidget* history, QString message, uint64_t messageId, QString username,
                      const Style::MessageWidget& st = st::defaultMessageWidget);

public:
    int expectedHeight() const;


protected:
    void paintEvent(QPaintEvent* e) override;
    //void resizeEvent(QResizeEvent* e) override;

private:
    std::unique_ptr<FlatTextEdit>   _fmtMessageText;
    uint64_t                        _userId;
    uint64_t                        _messageId;
    QString                         _messageText;
    QString                         _username;
    const Style::MessageWidget&     _st;
};
