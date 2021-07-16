#include "BioButton.hpp"

#include <QPainter>
#include <QtEvents>

#include "Application.hpp"
#include "PopupWidget.hpp"

BioButton::BioButton(QImage bioImage, bool inCaption, QWidget* parent) : AbstractButton(parent)
{
    if (inCaption)
    {
        setFixedWidth(Style::valueDPIScale(46));
        setFixedHeight(Style::valueDPIScale(30));
    }
    else
    {
    }

    // Setting up profile image
    image = std::move(bioImage);

    setClickCallback([=]() {
        // Creating popup and setting
        auto pop = new PopupWidget();

        /*
         * We need to know where to place popup menu on screen
         * If we just use mapToGlobal(localPoint) we'll get shift on X
         * like globalPoint.x = screenPosition.x + localPoint.x
         * so we need localPoint and globalPoint
         * localPoint - position on window
         * globalPoint - position on screen(with shift on X)
         */
        auto localPoint  = geometry().bottomLeft();
        auto globalPoint = mapToGlobal(localPoint);

        // Creating menu
        Menu* menu = new Menu;

        // Adding options
        menu->addAction("Username: Add format here", []() {});
        menu->addAction("Quit", []() { oApp->setAppState(App::AppState::LoginForm); });

        pop->setMenu(menu);

        // Now show the menu
        pop->popup(QPoint(globalPoint.x() - localPoint.x(), globalPoint.y() + 1));
    });
}

void BioButton::setImage(QImage newImage) { image = std::move(newImage); }

void BioButton::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    // No borders
    painter.setPen(Qt::NoPen);

    // Background color
    // painter.fillRect(0, 0, width(), height(), _hoverColor);

    // Profile image diameter 20 < 30 (height of button)
    int di = Style::valueDPIScale(20);

    // Right top offset
    const int hordi = (width() - di) / 2;
    const int verdi = (height() - di) / 2;

    // We need smooth images
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.drawImage(QRect(hordi, verdi, di, di), image);

    QWidget::paintEvent(event);
}
