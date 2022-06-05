#pragma once

#include <QGridLayout>
#include <QPropertyAnimation>
#include <QTimer>

#include <memory>

#include "AbstractNotificationMessage.hpp"

class NotificationMessage : public AbstractNotificationMessage
{
public:
    explicit NotificationMessage(QWidget* parent = nullptr);
    
    ~NotificationMessage() override = default;

    void setAnimationDuration(uint32_t newAnimationDuration);
public slots:
    void notificationShow() override;

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void hideAnimation();
    void hide();

private:
    QGridLayout             _layout;
    QPropertyAnimation      _animation;
    std::unique_ptr<QTimer> _timer;

    int _animationDuration      = 3000;
    int _row_widgetPosition     = 0;
    int _column_widgetPosition  = 0;
    int _offsetXY               = 5;
    int _offsetWH               = 10;
    int _xRadius                = 10;
    int _yRadius                = 10;
    qreal _transparent          = 0.0;
    QColor _black               = QColor(0, 0, 0, 180);

    const AbstractNotificationMessage::AnimationValue _animationValue;
};
