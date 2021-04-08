#pragma once

#include "pch.hpp"

class CaptionButton : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor hoverColor MEMBER hoverColor NOTIFY repaint)
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

    explicit CaptionButton(CaptionLogo logo, const QColor& endColor = QColor(255, 255, 255, 76),
                           QWidget* parent = nullptr);

    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    ~CaptionButton() override = default;
signals:

    void mouseRelease();

public:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    QColor hoverColor;
    QPropertyAnimation* fadeinAnim;
    QPixmap* pixmap;
};
