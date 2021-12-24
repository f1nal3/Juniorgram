#pragma once

#include <memory>
#include <QWidget>
#include <QString>

#include "Widgets/InputFields.hpp"
#include "Style/StyleWidgets.hpp"

class AbstractMessageWidget
{

private:
    std::unique_ptr<FlatTextEdit> _fmtMessageText;

    uint64_t    _messageID;
    uint64_t    _userID;

    QString     _messageText;
    QString     _userName;

    const Style::MessageWidget& _st;


};