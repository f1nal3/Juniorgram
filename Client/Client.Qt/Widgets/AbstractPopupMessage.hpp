#pragma once

#include "IPopupMessage.hpp"

class AbstractPopupMessage : public IPopupMessage
{
public:
    explicit AbstractPopupMessage(QWidget* parent) 
        : IPopupMessage(parent) 
    {
    }

    ~AbstractPopupMessage() override = default;

    void  setPopupText(const QString& text)     override = 0;
    void  popupShow()                           override = 0;
    float getPopupOpacity() const               override = 0;
    void  setPopupOpacity(float opacity)        override = 0;
};