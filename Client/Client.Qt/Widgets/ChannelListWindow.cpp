#include "ChannelListWindow.hpp"

#include <QFutureWatcher>

#include "Application.hpp"

ChannelListWindow::ChannelListWindow(std::shared_ptr<ListWidget>& anotherChannelListWidget, QWidget* parent)
    : QWidget(parent), _widgetChannelList(anotherChannelListWidget)
{
    setStyleSheet(
        QString("QWidget {"
                "background-color: #323232;"
                "}"));
    setWindowFlag(Qt::WindowMinimizeButtonHint, false);

    setFixedWidth(Style::valueDPIScale(300));
    setFixedHeight(Style::valueDPIScale(250));

    _vBoxLayout          = std::make_unique<QVBoxLayout>();
    _addChannelButton    = std::make_unique<FlatButton>(this, "Add");
    _updateChannelButton = std::make_unique<FlatButton>(this, "Update");
    _channelList         = std::make_unique<ListWidget>();

    _vBoxLayout->addWidget(_channelList.get());
    _vBoxLayout->addWidget(_addChannelButton.get());
    _vBoxLayout->addWidget(_updateChannelButton.get());

    _addChannelButton->setClickCallback([this]() { addChannelToMainChannelWidget(); });
    _updateChannelButton->setClickCallback([this]() { updateChannelListWindow(); });

    /// TODO: implement it w/o thread
    // connect(ReceiverManager::instance(), &ReceiverManager::onChannelListRequest, this, &ChannelListWindow::setChannels);

    setLayout(_vBoxLayout.get());
}

void ChannelListWindow::updateChannelList()
{
    std::thread([&]() {
        if (oApp->connectionManager()->isConnected())
        {
            std::mutex                   mtx;
            std::unique_lock<std::mutex> lck(mtx);
            mainWidgetStatus.wait(lck);

            int numberOfCoincidences = 0;

            for (int index = 0; index < _widgetChannelList->count(); ++index)
            {
                for (auto it = channels.rbegin(); it != channels.rend(); ++it)
                {
                    if (QString::fromStdString(it->channelName) == _widgetChannelList->item(index)->text())
                    {
                        channels.erase(std::remove(channels.begin(), channels.end(), *it), channels.end());
                    }
                }
            }
            for (auto it = channels.rbegin(); it != channels.rend(); ++it)
            {
                if (_channelList->count() == 0)
                {
                    _channelList->addItem(QString::fromStdString(it->channelName));
                }
                for (int i = 0; i < _channelList->count(); ++i)
                {
                    if (_channelList->item(i)->text() == QString::fromStdString(it->channelName))
                    {
                        numberOfCoincidences++;
                    }
                }
                if (numberOfCoincidences == 0)
                {
                    _channelList->addItem(QString::fromStdString(it->channelName));
                }
                numberOfCoincidences = 0;
            }
            lck.unlock();
        }
    }).detach();
}

void ChannelListWindow::addChannelToMainChannelWidget()
{
    if (_channelList->currentItem())
    {
        if (_widgetChannelList->count() != 0)
        {
            int numberOfCoincidences = 0;
            for (int i = 0; i < _widgetChannelList->count(); ++i)
            {
                if (_channelList->currentItem()->text() == _widgetChannelList->item(i)->text())
                {
                    numberOfCoincidences++;
                }
            }
            if (numberOfCoincidences == 0)
            {
                _widgetChannelList->addItem(_channelList->takeItem(_channelList->currentRow()));
            }
        }
        else
        {
            _widgetChannelList->addItem(_channelList->takeItem(_channelList->currentRow()));
        }
    }
    hide();
}

void ChannelListWindow::setChannels(std::vector<Network::ChannelInfo>&& channels_) { channels = channels_; }

void ChannelListWindow::updateChannelListWindow()
{
    if (oApp->connectionManager()->isConnected())
    {
        oApp->connectionManager()->askForChannelList();
        updateChannelList();
    }
}
