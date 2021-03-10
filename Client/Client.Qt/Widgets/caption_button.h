//
// Created by Stanislav on 17.02.2021.
//

#ifndef JUNIORGRAMUI_CAPTION_BUTTON_H
#define JUNIORGRAMUI_CAPTION_BUTTON_H

#include <QApplication>
#include <QBrush>
#include <QCoreApplication>
#include <QGraphicsItem>
#include <QImage>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QPropertyAnimation>
#include <QStringListModel>
#include <QStyleOption>
#include <QThread>
#include <QTimer>
#include <QWidget>
#include <cmath>
#include <iostream>
#include <memory>

class CaptionButton : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor close_button MEMBER close_button NOTIFY animation)
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

    explicit CaptionButton(QWidget* parent = nullptr, CaptionLogo logo = CaptionLogo::Restore,
                           const QColor& end_color = QColor(255, 255, 255, 76));

    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    ~CaptionButton() override = default;

public slots:

    void animation() { repaint(); }

signals:

    void mouseRelease();

private:
    QColor close_button;
    QPropertyAnimation* fadein_anim;
    QPixmap* pixmap;
};

#endif  // JUNIORGRAMUI_CAPTION_BUTTON_H
