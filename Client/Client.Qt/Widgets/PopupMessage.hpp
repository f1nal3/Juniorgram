#pragma once

#include <QGridLayout>
#include <QPropertyAnimation>
#include <QTimer>

#include <memory>

#include "AbstractPopupMessage.hpp"

namespace popupMessageAnimation
{
    enum class Animation
    {
        NOT_VISIBLE,
        VISIBLE
    };

    double getAnimationValue(Animation animation);

}  // namespace PopupMessageAnimation

class PopupMessage : public AbstractPopupMessage
{
public:
    explicit PopupMessage(QWidget* parent = nullptr);
    
    ~PopupMessage() override = default;

    void setAnimationDuration(unsigned int newAnimationDuration);
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
