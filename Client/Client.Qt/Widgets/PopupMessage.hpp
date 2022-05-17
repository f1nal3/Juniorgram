#pragma once

#include <QGridLayout>
#include <QPropertyAnimation>
#include <QTimer>

#include <memory>

#include "AbstractPopupMessage.hpp"

enum class Animation
{
    NOT_VISIBLE,
    VISIBLE
};

double getAnimationValue(Animation animation) 
{
    if (animation == Animation::VISIBLE) 
    {
        return 1.0;
    }
    else
    {
        return 0.0;
    }
}

class PopupMessage : public AbstractPopupMessage
{
public:
    explicit PopupMessage(QWidget* parent = nullptr);
    
    ~PopupMessage() override = default;

    void setShowTime(unsigned int newShowTime);
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
};
