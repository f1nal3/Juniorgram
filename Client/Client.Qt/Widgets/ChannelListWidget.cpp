#include "ChannelListWidget.hpp"

#include "Buttons.hpp"

ChannelListWidget::ChannelListWidget(QWidget* parent) : QWidget(parent)
{
    _mainLayout   = std::make_unique<QVBoxLayout>(this);
    _buttonLayout = std::make_unique<QHBoxLayout>();

    _channelList         = std::make_shared<ListWidget>();
    _addChannelButton    = std::make_unique<FlatButton>(this, "Add");
    _createChannelButton = std::make_unique<FlatButton>(this, "Create");

    _openChannelList     = std::make_unique<QShortcut>(
        QKeySequence{ "Shift+C" },
        this,
        this,
        [this](){ showChannelListWindow(); }
    );

    setContentsMargins(0, 0, 0, 0);
    setMinimumWidth(st::channelListMinWidth);
    setMaximumWidth(st::channelListMaxWidth);
    _buttonLayout->addWidget(_addChannelButton.get());
    _buttonLayout->addWidget(_createChannelButton.get());
    _mainLayout->addLayout(_buttonLayout.get());
    _mainLayout->addWidget(_channelList.get());
    _channelList->setFrameShape(QFrame::NoFrame);

    _addChannelButton->setClickCallback([this]() { showChannelListWindow(); });
    _createChannelButton->setClickCallback([this]() { showChannelCreationWindow(); });

    _mainLayout->setContentsMargins(contentsMargins());
    setLayout(_mainLayout.get());
}

void ChannelListWidget::showChannelListWindow()
{
    if (!_listWidgetChannels)
    {
        _listWidgetChannels = std::make_unique<ChannelListPage>(_channelList);
    }
    else
    {
        _listWidgetChannels->requestChannels();
    }
    _listWidgetChannels->show();
}

std::shared_ptr<ListWidget> ChannelListWidget::getChannelList() { return _channelList; }

void ChannelListWidget::showChannelCreationWindow()
{
    _channelCreationPopup = std::make_unique<ChannelCreationPage>();
    _channelCreationPopup->show();
    _channelCreationPopup.release();
}
