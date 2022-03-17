#include "ChannelBar.hpp"

#include <QPainter>
ChannelBar::ChannelBar(QWidget* parent, QString channelName /* const Network::ChannelInfo& info*/)
    : QWidget(parent), _channelName(channelName)
{
    _leaveChannel  = std::make_unique<FlatButton>(this, "Leave");
    _deleteChannel = std::make_unique<FlatButton>(this, "Delete");
    _leaveChannel->setClickCallback([this]() {
        leaveChannel();
        emit leaveChannelClick();
    });
    _deleteChannel->setClickCallback([this]() {
        leaveChannel();
        deleteChannel();
        emit leaveChannelClick();
    });
    setMinimumHeight(_leaveChannel->minimumHeight()  // minimal button height
                     + st::defaultMargin             // top margin
                     + st::defaultMargin);           // bottom margin
}

void ChannelBar::updateLayout()
{
    const auto& size = this->size();
    _deleteChannel->move(st::defaultMargin, st::defaultMargin);
    _leaveChannel->move(size.width()                   // from right
                            - st::defaultMargin        // right margin
                            - _leaveChannel->width(),  // button width
                        st::defaultMargin);            // top margin
}

void ChannelBar::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    /// Until we don't have link like button we use this
    p.setFont(st::boldFont);
    auto nameWidth = width() -                                              // widget width
                     (st::defaultMargin * 4                                 // left + right + between margins
                      + _deleteChannel->width() + _leaveChannel->width());  // buttons width
    auto drawText = st::boldFont->elided(_channelName, nameWidth);
    auto textX    = st::defaultMargin * 2 + _deleteChannel->width();  // left + between margin
    p.setPen(Qt::white);
    p.drawText(QRect(textX, st::defaultMargin, nameWidth, st::boldFont->height), drawText, Style::al_center);
}

void ChannelBar::resizeEvent(QResizeEvent*) { updateLayout(); }

void ChannelBar::leaveChannel()
{
    if (oApp->connectionManager()->isConnected())
    {
        oApp->connectionManager()->leaveChannel(_channelName.toStdString());
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log("Server isn't connected", Base::Logger::LogLevel::ERR);
    }
}

void ChannelBar::deleteChannel()
{
    if (oApp->connectionManager()->isConnected())
    {
        oApp->connectionManager()->deleteChannel(_channelName.toStdString());
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log("Server isn't connected", Base::Logger::LogLevel::ERR);
    }
}
