#include "AbstractPopupMessage.hpp"

AbstractPopupMessage::AbstractPopupMessage(QWidget* parent) 
	: QWidget(parent) 
{
}

void AbstractPopupMessage::setPopupText(const QString& text) 
{

}

void AbstractPopupMessage::popupShow() 
{

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