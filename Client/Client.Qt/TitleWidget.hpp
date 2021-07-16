#pragma once

#include <QWidget>

class CaptionButton;

class TitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TitleWidget(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* paintEvent) override;
    void resizeEvent(QResizeEvent* resizeEvent) override;
    void mousePressEvent(QMouseEvent* mouseEvent) override;

private:
    std::unique_ptr<CaptionButton> _closebtn;
    std::unique_ptr<CaptionButton> _maximizebtn;
    std::unique_ptr<CaptionButton> _minbtn;
};