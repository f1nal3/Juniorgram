#include "BioButton.hpp"

#include <QPainter>
#include <QWindow>
#include <QtEvents>

#include "Application.hpp"

BioButton::BioButton(QWidget* parent, bool) : CaptionButton(parent)
{
    _popup = std::make_unique<PopupWidget>(this);
    _popup->setFocusPolicy(Qt::NoFocus);
    _popup->setFocusProxy(this);

    setClickCallback([=]() {
        // Creating popup and setting
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

        _popup->setMenu(menu);

        // Now show the menu
        _popup->popup(QPoint(globalPoint.x() - localPoint.x(), globalPoint.y() + 1));
    });
}
