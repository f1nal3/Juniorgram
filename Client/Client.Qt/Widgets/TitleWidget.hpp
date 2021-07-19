#pragma once

#include <QWidget>
#include <Style/StyleTypes.hpp>

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
    explicit TitleWidget(QWidget* parent = nullptr);
    void showBioButton(bool show = true);
    bool setBioButtonIcon(const Style::icon* icon);

protected:
    void paintEvent(QPaintEvent* paintEvent) override;
    void resizeEvent(QResizeEvent* resizeEvent) override;
    void mousePressEvent(QMouseEvent* mouseEvent) override;

private:
    std::unique_ptr<CaptionButton> _closeButton;
    std::unique_ptr<CaptionButton> _maximizeButton;
    std::unique_ptr<CaptionButton> _minimizeButton;
    std::unique_ptr<BioButton>     _bioButton;
};
