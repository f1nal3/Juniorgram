#include "BioButton.hpp"

#include <utility>

#include "App.hpp"
#include "PopupWidget.hpp"

BioButton::BioButton(QImage bioImage, bool inCaption, QWidget* parent) : QWidget(parent)
{
    /*
     * This part set button sizes.
     * P.S. We didn't have and don't have a designer
     * so this was the take from Teams desktop :)
     */
    if (inCaption)
    {
        setFixedWidth(Style::valueDPIScale(46));
        setFixedHeight(Style::valueDPIScale(30));
    }
    else
    {
    }

    // Setting up hover animation
    // P.S. why??
    // P.S.S why not
    fadeinAnim = new QPropertyAnimation(this, "hoverColor");
    fadeinAnim->setDuration(150);
    fadeinAnim->setEasingCurve(QEasingCurve::InCubic);
    fadeinAnim->setStartValue(QColor(255, 255, 255, 0));
    fadeinAnim->setEndValue(QColor(255, 255, 255, 76));

    // Setting up background color
    hoverColor = QColor(255, 255, 255, 0);

    // Setting up profile image
    image = std::move(bioImage);

    // Now it's all up to eventFilter
    installEventFilter(this);

    // For HoverXXX messages
    setAttribute(Qt::WA_Hover);

    // setMouseTracking(true);
}

void BioButton::setImage(QImage newImage) { image = std::move(newImage); }

bool BioButton::eventFilter(QObject* object, QEvent* event)
{
    if (object == this)
    {
        // If mouse is enter then start the hover animation
        if (event->type() == QEvent::HoverEnter)
        {
            fadeinAnim->start();
            return true;
        }
        // If leaves return background color
        // P.S. there is no animation :(
        else if (event->type() == QEvent::HoverLeave)
        {
            fadeinAnim->stop();
            hoverColor = fadeinAnim->startValue().value<QColor>();
            update();
            return true;
        }
        // On click
        else if (event->type() == QEvent::MouseButtonRelease)
        {
            fadeinAnim->stop();
            hoverColor = fadeinAnim->startValue().value<QColor>();
            // Getting mouse event info
            auto ev = dynamic_cast<QMouseEvent*>(event);

            /*
             * Bottom left point of widget in application and on screen
             * When I was writing this I've thought that screen position is enough
             * but there was a shift on X
             */
            auto localPoint  = this->geometry().bottomLeft();
            auto globalPoint = mapToGlobal(localPoint);

            // Creating menu
            // UVAHA: Do not set "this" as a parent
            Menu* menu = new Menu;

            // Adding options
            menu->addAction("Username: Add format here", []() {});
            menu->addAction("Quit", []() { App::setAppState(AppState::LoginForm); });

            // We need this to happen on LMB
            if (ev->button() == Qt::LeftButton)
            {
                // Creating popup and setting
                auto pop = new PopupWidget();
                pop->setMenu(menu);

                // Now show the menu
                pop->popup(QPoint(globalPoint.x() - localPoint.x(), globalPoint.y() + 1));
            }
            return true;
        }
    }
    else
    {
        /*
         * If message to not our widget
         * then reset everything
         */
        fadeinAnim->stop();
        hoverColor = fadeinAnim->startValue().value<QColor>();
        repaint();
    }

    return QObject::eventFilter(object, event);
}

void BioButton::paintEvent(QPaintEvent* event)
{
    QPainter p(this);

    // No borders
    p.setPen(Qt::NoPen);

    // Background color
    p.fillRect(0, 0, width(), height(), hoverColor);

    // Profile image diameter 20 < 30 (height of button)
    int di = Style::valueDPIScale(20);

    // Right top offset
    const int hordi = (width() - di) / 2;
    const int verdi = (height() - di) / 2;

    // We need smooth images
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.drawImage(QRect(hordi, verdi, di, di), image);

    QWidget::paintEvent(event);
}
