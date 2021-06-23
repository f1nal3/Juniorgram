#include "BioButton.hpp"

#include <utility>

#include "Application.hpp"
#include "PopupWidget.hpp"

BioButton::BioButton(QImage bioImage, bool inCaption, QWidget* parent) : QWidget(parent)
{
    /*
     * This part set button sizes.
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

    // For Hover messages
    setAttribute(Qt::WA_Hover);
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

            // We need this to happen on
            if (ev->button() == Qt::LeftButton)
            {
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
                auto localPoint  = this->geometry().bottomLeft();
                auto globalPoint = mapToGlobal(localPoint);

                // Creating menu
                Menu* menu = new Menu;

                // Adding options
                menu->addAction("Username: Add format here", []() {});
                menu->addAction("Quit", []() {
                    oApp->setAppState(App::AppState::LoginForm);
                    //App::setAppState(AppState::LoginForm);
                });

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
    QPainter painter(this);

    // No borders
    painter.setPen(Qt::NoPen);

    // Background color
    painter.fillRect(0, 0, width(), height(), hoverColor);

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
