#include "NotificationWidget.hpp"

#include <QPainter>
#include <QScreen>
#include <Style/Styles.hpp>

#include "Application.hpp"

NotificationWidget::NotificationWidget(QWidget* parent) : QWidget(parent)
{
    setWindowFlags(Qt::WindowFlags(Qt::FramelessWindowHint) | Qt::Popup | Qt::BypassWindowManagerHint | Qt::NoDropShadowWindowHint);
    setMouseTracking(true);

    hide();
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
}

void NotificationWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    QColor   inputField(0x35, 0x35, 0x35);
    painter.setBrush(inputField);
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, width(), height());
}

void NotificationWidget::hideEvent(QHideEvent*)
{
    if (_deleteOnHide) deleteLater();
}

void NotificationWidget::setMessage(QString text) 
{ 
    if (!_messageText)
    {
        _messageText = std::make_unique<QLabel>(text, this);
    }
    else
    {
        _messageText->setText(text);
    }
}

void NotificationWidget::notification(const QPoint& globalPoint)
{
    if (_innerMenu)
    {
        _innerMenu->move(0, 10);
        resize(_innerMenu->width(), _innerMenu->height() + 20);
        _innerMenu->show();
    }
    auto screen = Application::screenAt(globalPoint);
    if (!screen) return;
    auto p = globalPoint;
    // Make sure notification is on screen
    if (auto w = screen->availableGeometry(); p.x() + width() > w.right())
    {
        p.rx() -= p.x() + width() - w.right();
    }
    move(p);
    show();
}

void NotificationWidget::setMenu(std::unique_ptr<Menu> menu)
{
    _innerMenu = std::move(menu);
    _innerMenu->setParent(this);
    _innerMenu->setTriggeredCallback([this](const CallbackData&) { this->hide(); });
}
void NotificationWidget::setDeleteOnHide(bool deleteOnHide) { _deleteOnHide = deleteOnHide; }

void messageOut(std::shared_ptr<NotificationWidget> notificationWidget, QString text)
{
    notificationWidget->resize(400, 150);
    notificationWidget->setMessage(text);
    notificationWidget->notification(QPoint(960 - 400 / 2, 540 - 150 / 2));  // alignment near the center
}
