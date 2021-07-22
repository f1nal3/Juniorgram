#pragma once

#include <QPropertyAnimation>
#include <Widgets/AbstractButton.hpp>

#include "Style/StyleBasic.hpp"

/**
 * @class CaptionButton
 * @brief A button for title
 */
class CaptionButton : public AbstractButton
{
    Q_OBJECT
    Q_PROPERTY(QColor _hoverColor READ getHoverColor WRITE setHoverColor)
public:
    /**
     * @brief Constructor for caption button
     * @param parent Title widget
     * @param endColor Color on hover
     * @param icon Default icon
     */
    explicit CaptionButton(QWidget* parent = nullptr, const Style::TitleBarButton* st = &st::defaultTitleBarButton);

    /**
     * @brief Override icon by a new one
     * @param icon New icon
     * @return Icon has been set?
     */
    bool setIcon(const Style::icon* icon);

    /**
     * @brief Sets a new style
     * @param newSt New style
     */
    void setStyle(const Style::TitleBarButton* newSt);

protected:
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    /**
     * @brief Updates widget to current style
     */
    void updateWidget();

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
    QColor                              _hoverColor;
    const Style::TitleBarButton*        _st;
    const Style::icon*                  _iconOverride = nullptr;
    std::unique_ptr<QPropertyAnimation> _fadeinAnim;
};
