#pragma once

#include <QWidget>

class I_PopupMessage : public QWidget
{
public:
    Q_OBJECT

    I_PopupMessage(QWidget* parent)
        : QWidget(parent) 
    {
    }
    /*
    virtual ~I_PopupMessage();
    */
    virtual void setPopupText(const QString& text);
    virtual void popupShow();
    virtual float getPopupOpacity() const;
    virtual void  setPopupOpacity(float opacity);
};
