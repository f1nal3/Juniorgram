#pragma once

#include "AbstractPopupMessage.hpp"
#include <QLabel>
#include <QGridLayout>
#include <QPropertyAnimation>
#include <QTimer>
#include <memory>

class PopupMessage : public AbstractPopupMessage
{
public:
    explicit PopupMessage(QWidget* parent = nullptr);
    
    ~PopupMessage() override = default;

    float getPopupOpacity() const override;
    void  setPopupOpacity(float opacity) override;

public slots:
    void setPopupText(const QString& text) override;
    void popupShow() override;

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void hideAnimation();
    void hide();

private:
    QLabel                   _label;
    QGridLayout              _layout;
    QPropertyAnimation       _animation;
    std::unique_ptr<QTimer>  _timer;
};
