#include "BioButton.hpp"

#include <utility>

#include "PopupWidget.hpp"
#include "Style/Style.hpp"
#include "app.h"

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
    fadein_anim = new QPropertyAnimation(this, "hover_color");
    fadein_anim->setDuration(150);
    fadein_anim->setEasingCurve(QEasingCurve::InCubic);
    fadein_anim->setStartValue(QColor(255, 255, 255, 0));
    fadein_anim->setEndValue(QColor(255, 255, 255, 76));
    hover_color = QColor(255, 255, 255, 15);
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
            fadein_anim->start();
            return true;
        }
        else if (event->type() == QEvent::HoverLeave)
        {
            fadein_anim->stop();
            hover_color = fadein_anim->startValue().value<QColor>();
            update();
        }
        else if (event->type() == QEvent::MouseButtonRelease)
        {
            fadein_anim->stop();
            hover_color      = fadein_anim->startValue().value<QColor>();
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
        fadein_anim->stop();
        hover_color = fadein_anim->startValue().value<QColor>();
        repaint();
    }
    return QObject::eventFilter(object, event);
}

void BioButton::paintEvent(QPaintEvent* event)
{
    QPainter p(this);

    p.setPen(Qt::NoPen);
    p.fillRect(0, 0, width(), height(), hover_color);
    int di          = Style::valueDPIScale(20);
    const int hordi = (width() - di) / 2;
    const int verdi = (height() - di) / 2;
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.setRenderHint(QPainter::Antialiasing);
    p.drawImage(QRect(hordi, verdi, di, di), image);

    QWidget::paintEvent(event);
}

void BioButton::animation() { repaint(); }
