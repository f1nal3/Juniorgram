#pragma once

#include <QWidget>

class I_PopupMessage : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(float popupOpacity READ getPopupOpacity WRITE setPopupOpacity)
public:

    I_PopupMessage(QWidget* parent)
        : QWidget(parent) 
    {
    }
    
    virtual ~I_PopupMessage(){};
    
    virtual void setPopupText(const QString& text)  = 0;
    virtual void  popupShow()                       = 0;
    virtual float getPopupOpacity() const           = 0;
    virtual void  setPopupOpacity(float opacity)    = 0;
    
protected:
    float popupOpacity;
};
