#pragma once

#include <QWidget>
#include <QLabel>

class IPopupMessage
{
public:
    virtual ~IPopupMessage() = default;

    virtual void  setPopupText(const QString& text) = 0;
    virtual void  popupShow()                       = 0;
};


class AbstractPopupMessage : public QWidget, public IPopupMessage
{
    Q_OBJECT
    Q_PROPERTY(float _popupOpacity READ getPopupOpacity WRITE setPopupOpacity)
public:
    explicit AbstractPopupMessage(QWidget* parent);

    ~AbstractPopupMessage() override = default;

    void  setPopupText(const QString& text) override;
    void  popupShow() override;

    float getPopupOpacity() const;
    void  setPopupOpacity(float opacity);

    QLabel& getPopupLabel();

    struct AnimationValue
    {
        int animationTimeShow = 150;
        int animationTimeHide = 1000;

        double transparent     = 1.0;
        double not_transparent = 0.0;
    };

private:
    float _popupOpacity;
    QLabel _label;
};
