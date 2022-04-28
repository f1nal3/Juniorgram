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

    void  setPopupText(const QString& text)      override = 0;
    virtual void  popupShow()                    override = 0;
    virtual float getPopupOpacity() const        override = 0;
    virtual void  setPopupOpacity(float opacity) override = 0;
};