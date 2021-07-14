#pragma once

#include <QPropertyAnimation>
#include <Widgets/AbstractButton.hpp>

class CaptionButton : public AbstractButton
{
    Q_OBJECT
    Q_PROPERTY(QColor hoverColor READ getHoverColor WRITE setHoverColor)
public:
    enum class CaptionLogo
    {
        Maximize,
        Minimize,
        Restore,
        Close
    };
    static QString Lg2str(CaptionLogo logo);
    static QString dpi2str(int scale);
    explicit CaptionButton(CaptionLogo logo, const QColor& endColor = QColor(255, 255, 255, 76), QWidget* parent = nullptr);
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    ~CaptionButton() override = default;

private:
    void setHoverColor(QColor newColor)
    {
        hoverColor = std::move(newColor);
        update();
    }
    QColor getHoverColor() { return hoverColor; }

private:
    QColor              hoverColor;
    QPropertyAnimation* fadeinAnim;
    QPixmap*            pixmap;
};
