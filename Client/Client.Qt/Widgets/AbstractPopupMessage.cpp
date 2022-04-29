#include "AbstractPopupMessage.hpp"

AbstractPopupMessage::AbstractPopupMessage(QWidget* parent) 
	: QWidget(parent) 
{
}

void AbstractPopupMessage::setPopupText(const QString& text) 
{
<<<<<<< HEAD
=======

>>>>>>> 3e84defda7d13bc0a3850a4c2c0787b38009cdff
}

void AbstractPopupMessage::popupShow() 
{
<<<<<<< HEAD
=======

>>>>>>> 3e84defda7d13bc0a3850a4c2c0787b38009cdff
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