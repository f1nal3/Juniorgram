#pragma once

#include <QWidget>

#include "Style/StyleBasic.hpp"

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

private:
    const Style::TitleBar&         _st;
    std::unique_ptr<CaptionButton> _closeButton;
    std::unique_ptr<CaptionButton> _maximizeButton;
    std::unique_ptr<CaptionButton> _minimizeButton;
    std::unique_ptr<BioButton>     _bioButton;
};
