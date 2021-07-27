#pragma once

#include <QPropertyAnimation>
#include <QWidget>
#include <memory>

#include "Style/StyleBasic.hpp"
#include "Widgets/AbstractButton.hpp"
#include "Widgets/PopupWidget.hpp"

class CaptionButton;
class BioButton;

/**
 * @class TitleWidget
 * @brief Title for a widget
 */
class TitleWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief creates system title for window
     * @param parent window
     */
    explicit TitleWidget(QWidget* parent = nullptr, const Style::TitleBar& st = st::defaultTitleBar);
    /**
     * @brief show bio-button
     * @param show show bio-button?
     */
    void showBioButton(bool show = true);
    /**
     * @brief Set a new bio-button icon
     * @param icon A new icon (nullptr removes icon)
     * @return icon has been set?
     */
    bool setBioButtonIcon(const Style::icon* icon);

protected:
    void paintEvent(QPaintEvent* paintEvent) override;
    void resizeEvent(QResizeEvent* resizeEvent) override;
    void mousePressEvent(QMouseEvent* mouseEvent) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
    void windowStateChanged(Qt::WindowState state);

private:
    const Style::TitleBar&         _st;
    std::unique_ptr<CaptionButton> _closeButton;
    std::unique_ptr<CaptionButton> _maximizeButton;
    std::unique_ptr<CaptionButton> _minimizeButton;
    std::unique_ptr<BioButton>     _bioButton;

    bool _maximizedState;
};

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

/**
 * @class BioButton
 * @brief Class for Profile menu widget
 */
class BioButton : public CaptionButton
{
    Q_OBJECT
public:
    /**
     * @brief Constructor of bio button
     * @param parent Parent of widget
     * @param inCaption Must be set true for now
     */
    explicit BioButton(QWidget* parent = nullptr, bool inCaption = true);

private:
    std::unique_ptr<PopupWidget> _popup;
};
