#include "BioButton.hpp"

#include <utility>

#include "App.hpp"
#include "PopupWidget.hpp"

BioButton::BioButton(QImage bio_image, bool in_caption, QWidget* parent) : QWidget(parent)
{
    if (in_caption)
    {
        setFixedWidth(Style::valueDPIScale(46));
        setFixedHeight(Style::valueDPIScale(30));
    }
    else
    {
    }
    fadeinAnim = new QPropertyAnimation(this, "hoverColor");
    fadeinAnim->setDuration(150);
    fadeinAnim->setEasingCurve(QEasingCurve::InCubic);
    fadeinAnim->setStartValue(QColor(255, 255, 255, 0));
    fadeinAnim->setEndValue(QColor(255, 255, 255, 76));
    hoverColor  = QColor(255, 255, 255, 0);
    image       = std::move(bio_image);
    installEventFilter(this);
    setAttribute(Qt::WA_Hover);
    // setMouseTracking(true);
}

void BioButton::setImage(QImage new_image) { image = std::move(new_image); }

bool BioButton::eventFilter(QObject* object, QEvent* event)
{
    if (object == this)
    {
        if (event->type() == QEvent::HoverEnter)
        {
            fadeinAnim->start();
            return true;
        }
        else if (event->type() == QEvent::HoverLeave)
        {
            fadeinAnim->stop();
            hoverColor = fadeinAnim->startValue().value<QColor>();
            update();
        }
        else if (event->type() == QEvent::MouseButtonRelease)
        {
            fadeinAnim->stop();
            hoverColor       = fadeinAnim->startValue().value<QColor>();
            auto ev          = dynamic_cast<QMouseEvent*>(event);
            auto beforepoint = this->geometry().bottomLeft();
            auto point       = mapToGlobal(this->geometry().bottomLeft());
            Menu* menu       = new Menu;
            menu->addAction("Username: Add format here", []() {});
            menu->addAction("Quit", []() { App::setAppState(AppState::LoginForm); });
            if (ev->button() == Qt::LeftButton)
            {
                auto pop = new PopupWidget();
                pop->setMenu(menu);
                pop->popup(QPoint(point.x() - beforepoint.x(), point.y() + 1));
            }
        }
    }
    else
    {
        fadeinAnim->stop();
        hoverColor = fadeinAnim->startValue().value<QColor>();
        repaint();
    }
    return QObject::eventFilter(object, event);
}

void BioButton::paintEvent(QPaintEvent* event)
{
    QPainter p(this);

    p.setPen(Qt::NoPen);
    p.fillRect(0, 0, width(), height(), hoverColor);
    int di          = Style::valueDPIScale(20);
    const int hordi = (width() - di) / 2;
    const int verdi = (height() - di) / 2;
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.setRenderHint(QPainter::Antialiasing);
    p.drawImage(QRect(hordi, verdi, di, di), image);

    QWidget::paintEvent(event);
}

