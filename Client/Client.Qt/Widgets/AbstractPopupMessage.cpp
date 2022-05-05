#include "AbstractPopupMessage.hpp"

AbstractPopupMessage::AbstractPopupMessage(QWidget* parent) 
	: QWidget(parent) 
{
}

void AbstractPopupMessage::setPopupText(const QString& text) 
{
    _label.setText(text);
    adjustSize();
}

void AbstractPopupMessage::popupShow() 
{
    setWindowOpacity(0.0);

    QWidget::show();
}

float AbstractPopupMessage::getPopupOpacity() const 
{ 
	return _popupOpacity; 
}

void AbstractPopupMessage::setPopupOpacity(float opacity)
{
    _popupOpacity = opacity;

    setWindowOpacity(opacity);
}
