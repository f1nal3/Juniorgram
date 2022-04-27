#pragma once

#include "IPopupMessage.hpp"
#include <QLabel>
#include <QGridLayout>
#include <QPropertyAnimation>
#include <QTimer>

class PopupMessage : public IPopupMessage
{
public:
    explicit PopupMessage(QWidget* parent = nullptr);

    ~PopupMessage() override;

    float getPopupOpacity() const override;
    void  setPopupOpacity(float opacity) override;

public slots:
    void setPopupText(const QString& text) override;
    void popupShow() override;

protected:
    void paintEvent(QPaintEvent* event);

private slots:
    void hideAnimation();
    void hide();

private:
    QLabel             _label;
    QGridLayout        _layout;
    QPropertyAnimation _animation;
    QTimer*            _timer;
};
