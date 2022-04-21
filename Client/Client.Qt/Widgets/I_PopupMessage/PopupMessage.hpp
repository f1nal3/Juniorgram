#pragma once

#include "I_PopupMessage.hpp"
#include <QLabel>
#include <QGridLayout>
#include <QPropertyAnimation>
#include <QTimer>

class PopupMessage : public I_PopupMessage
{
public:
    explicit PopupMessage(QWidget* parent = nullptr);

    virtual ~PopupMessage() override
    {

    }
    

public slots:
    void setPopupText(const QString& text) override;
    void popupShow() override;

protected:
    void paintEvent(QPaintEvent* event);

private slots:
    void hideAnimation();
    void hide();

private:
    QLabel             label;
    QGridLayout        layout;
    QPropertyAnimation animation;
    //float              popupOpacity;
    QTimer*            timer;

    float getPopupOpacity() const override;
    void  setPopupOpacity(float opacity) override;
};
