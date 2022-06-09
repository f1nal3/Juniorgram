#include "AbstractNotificationMessage.hpp"

AbstractNotificationMessage::AbstractNotificationMessage(QWidget* parent) 
	: QWidget(parent) 
{
}

void AbstractNotificationMessage::setNotificationText(const QString& text) 
{
    _label.setText(text);
    adjustSize();
}

void AbstractNotificationMessage::notificationShow() 
{
    setWindowOpacity(0.0);

    QWidget::show();
}

float AbstractNotificationMessage::getNotificationOpacity() const 
{ 
	return _notificationOpacity; 
}

void AbstractNotificationMessage::setNotificationOpacity(float opacity)
{
    _notificationOpacity = opacity;

    setWindowOpacity(opacity);
}

QLabel& AbstractNotificationMessage::getNotificationLabel()
{ 
    return _label;
}
