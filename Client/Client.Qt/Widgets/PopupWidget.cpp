#include "PopupWidget.hpp"

#include <QPainter>
#include <QScreen>
#include <Style/Styles.hpp>

#include "Application.hpp"

PopupWidget::PopupWidget(QWidget* parent) : QWidget(parent), _innerMenu(nullptr)
{
    setWindowFlags(Qt::WindowFlags(Qt::FramelessWindowHint) | Qt::Popup | Qt::BypassWindowManagerHint | Qt::NoDropShadowWindowHint);
    setMouseTracking(true);

    hide();
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
}

void PopupWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    QColor   inputField(0x35, 0x35, 0x35);
    painter.setBrush(inputField);
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, width(), height());
}

void PopupWidget::hideEvent(QHideEvent*)
{
    if (_deleteOnHide) deleteLater();
}

void PopupWidget::setMessage(QString text) 
{ 
    if (!_messageText)
    {
        _messageText = std::make_unique<QLabel>(text, this);
    }
 
    _messageText->setText(text);
}

void PopupWidget::popup(const QPoint& globalPoint)
{
    if (_innerMenu)
    {
        _innerMenu->move(0, 10);
        resize(_innerMenu->width(), _innerMenu->height() + 20);
        _innerMenu->show();
    }
    auto screen = oApp->screenAt(globalPoint);
    if (!screen) return;
    auto p = globalPoint;
    auto w = screen->availableGeometry();
    // Make sure popup is on screen
    if (p.x() + width() > w.right())
    {
        p.rx() -= p.x() + width() - w.right();
    }
    move(p);
    show();
}

void PopupWidget::setMenu(std::unique_ptr<Menu> menu)
{
    _innerMenu = std::move(menu);
    _innerMenu->setParent(this);
    _innerMenu->setTriggeredCallback([=](const CallbackData&) { hide(); });
}
void PopupWidget::setDeleteOnHide(bool deleteOnHide) { _deleteOnHide = deleteOnHide; }

void messageOut(std::shared_ptr<PopupWidget> popupWidget, QString text)
{
    popupWidget->resize(400, 150);
    popupWidget->setMessage(text);
    popupWidget->popup(QPoint(960 - 400 / 2, 540 - 150 / 2));  //nahardkodil, ny ya i negodyai
}