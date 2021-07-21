#include "BioButton.hpp"

#include <QPainter>
#include <QtEvents>

#include "Application.hpp"
#include "PopupWidget.hpp"

BioButton::BioButton(QWidget* parent, bool) : CaptionButton(parent)
{
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
