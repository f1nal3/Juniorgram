#pragma once

#include <QGridLayout>
#include <QPropertyAnimation>
#include <QTimer>

#include <memory>

#include "AbstractPopupMessage.hpp"

struct AnimationValue
{
    int animationTimeShow = 150;
    int  animationTimeHide = 1000;

    double transparent     = 1.0;
    double not_transparent = 0.0;
};

class PopupMessage : public AbstractPopupMessage
{
public:
    explicit PopupMessage(QWidget* parent = nullptr);
    
    ~PopupMessage() override = default;

    void setAnimationDuration(uint32_t newAnimationDuration);
public slots:
    void popupShow() override;

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void hideAnimation();
    void hide();

private:
    QGridLayout             _layout;
    QPropertyAnimation      _animation;
    std::unique_ptr<QTimer> _timer;

    int                     _animationDuration = 3000;
    int _row            = 0;
    int _column         = 0;
    int _offsetXY       = 5;
    int _offsetWH       = 10;
    int _xRadius        = 10;
    int _yRadius        = 10;
    qreal _transparent  = 0.0;
    QColor _black       = QColor(0, 0, 0, 180);

    const AnimationValue _animationValue;
};
