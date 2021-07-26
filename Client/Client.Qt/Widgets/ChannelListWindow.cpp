#include "ChannelListWindow.hpp"

#include <QFutureWatcher>

#include "ConnectionManager.hpp"
#include "Style/Style.hpp"

ChannelListWindow::ChannelListWindow(std::shared_ptr<ListWidget>& anotherChannelListWidget, QWidget* parent)
    : QWidget(parent), _channelListMainWindow(anotherChannelListWidget)
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
    ConnectionManager::getClient().askForChannelList();
    updateChannelList();

    setLayout(_vBoxLayout.get());
}

void ChannelListWindow::updateChannelList()
{
    std::thread([&]() {
        if (ConnectionManager::isConnected())
        {
            std::mutex                   mtx;
            std::unique_lock<std::mutex> lck(mtx);
            mainWidgetStatus.wait(lck);

            int numberOfCoincidences = 0;

            for (int i = 0; i < _channelListMainWindow->count(); ++i)
            {
                for (auto it = _channelNames.rbegin(); it != _channelNames.rend(); ++it)
                {
                    if (QString::fromStdString(*it) == _channelListMainWindow->item(i)->text())
                    {
                        _channelNames.erase(std::remove(_channelNames.begin(), _channelNames.end(), *it), _channelNames.end());
                    }
                }
            }
            for (auto it = _channelNames.rbegin(); it != _channelNames.rend(); ++it)
            {
                if (_channelList->count() == 0)
                {
                    _channelList->addItem(QString::fromStdString(*it));
                }
                for (int i = 0; i < _channelList->count(); ++i)
                {
                    if (_channelList->item(i)->text() == QString::fromStdString(*it))
                    {
                        numberOfCoincidences++;
                    }
                }
                if (numberOfCoincidences == 0)
                {
                    _channelList->addItem(QString::fromStdString(*it));
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
        if (_channelListMainWindow->count() != 0)
        {
            int numberOfCoincidences = 0;
            for (int i = 0; i < _channelListMainWindow->count(); ++i)
            {
                if (_channelList->currentItem()->text() == _channelListMainWindow->item(i)->text())
                {
                    numberOfCoincidences++;
                }
            }
            if (numberOfCoincidences == 0)
            {
                _channelListMainWindow->addItem(_channelList->takeItem(_channelList->currentRow()));
            }
        }
        else
        {
            _channelListMainWindow->addItem(_channelList->takeItem(_channelList->currentRow()));
        }
    }
    this->hide();
}

void ChannelListWindow::addChannelInfo(const std::string& nameOfChannels) { _channelNames.push_back(nameOfChannels); }

void ChannelListWindow::updateChannelListWindow()
{
    if (ConnectionManager::isConnected())
    {
        ConnectionManager::getClient().askForChannelList();
        updateChannelList();
    }
}
