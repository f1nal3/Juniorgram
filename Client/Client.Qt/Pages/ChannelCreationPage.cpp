#include "ChannelCreationPage.hpp"

#include <Application.hpp>

#include "Widgets/Buttons.hpp"
#include "Widgets/InputFields.hpp"

ChannelCreationPage::ChannelCreationPage(QWidget* parent) : Page(parent)
{
    _createButton     = std::make_unique<FlatButton>(this, "Create");
    _channelNameInput = std::make_unique<FlatInput>(this, "Channel name");

    _createButton->setClickCallback([=]() {
        auto enteredText = _channelNameInput->text();
        // If entered channel name length bigger than six
        if (enteredText.length() >= 6)
        {
            oApp->connectionManager()->createChannel(enteredText.toStdString());
            close();
        }
    });

    setWindowFlag(Qt::WindowMinimizeButtonHint, false);
    setWindowFlag(Qt::WindowStaysOnTopHint);
    setWindowModality(Qt::ApplicationModal);

    setFixedWidth(Style::valueDPIScale(300));
    setFixedHeight(Style::valueDPIScale(250));

    setAttribute(Qt::WA_DeleteOnClose);
    updateLayout();
}

void ChannelCreationPage::updateLayout()
{
    const auto& size       = this->size();
    auto        blockWidth = size.width()   // page width
                      - st::defaultMargin   // left margin
                      - st::defaultMargin;  // right margin

    _channelNameInput->resize(blockWidth, _channelNameInput->minimumHeight());
    _createButton->resize(blockWidth, _createButton->minimumHeight());

    _channelNameInput->move(st::defaultMargin, st::defaultMargin);
    auto createButtonY = st::defaultMargin              // top margin
                         + _channelNameInput->height()  // input height
                         + st::defaultMargin;           // margin between button and input
    _createButton->move(st::defaultMargin, createButtonY);

    setFixedHeight(_createButton->y() + _createButton->height() + st::defaultMargin);
}
