#pragma once

#include <QPropertyAnimation>
#include <Widgets/AbstractButton.hpp>

#include "Style/StyleBasic.hpp"

class CaptionButton : public AbstractButton
{
    Q_OBJECT
    Q_PROPERTY(QColor _hoverColor READ getHoverColor WRITE setHoverColor)
public:
    explicit CaptionButton(QWidget* parent = nullptr, const QColor& endColor = QColor(255, 255, 255, 76),
                           const Style::icon& icon = Style::icon());

    bool setIcon(const Style::icon* icon);

protected:
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    /**
     * @brief Changes button background color if in title bar
     * @param newColor New background color
     */
    void setHoverColor(QColor newColor)
    {
        _hoverColor = std::move(newColor);
        update();
    }
    /**
     * @brief Returns current background color
     * @return Current background color
     */
    QColor getHoverColor() { return _hoverColor; }

private:
    QColor              _hoverColor;
    const Style::icon&  _icon;
    const Style::icon*  _iconOverride = nullptr;
    QPropertyAnimation* _fadeinAnim;
};
