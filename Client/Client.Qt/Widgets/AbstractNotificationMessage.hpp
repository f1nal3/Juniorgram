#pragma once

#include <QWidget>
#include <QLabel>

class INotificationMessage
{
public:
    virtual ~INotificationMessage() = default;

    virtual void  setNotificationText(const QString& text) = 0;
    virtual void  notificationShow()                       = 0;
};


class AbstractNotificationMessage : public QWidget, public INotificationMessage
{
    Q_OBJECT
    Q_PROPERTY(float _notificationOpacity READ getNotificationOpacity WRITE setNotificationOpacity)
public:
    explicit AbstractNotificationMessage(QWidget* parent);

    ~AbstractNotificationMessage() override = default;

    void  setNotificationText(const QString& text) override;
    void  notificationShow() override;

    float getNotificationOpacity() const;
    void  setNotificationOpacity(float opacity);

    QLabel& getNotificationLabel();

    struct AnimationValue
    {
        int animationTimeShow = 150;
        int animationTimeHide = 1000;

        double transparent     = 1.0;
        double not_transparent = 0.0;
    };

private:
    float _notificationOpacity;
    QLabel _label;
};
