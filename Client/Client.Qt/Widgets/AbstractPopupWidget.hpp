#pragma once

#ifndef ABSTRUCTPOPUPWIDGET_H
#define ABSTRUCTPOPUPWIDGET_H

#include <QGridLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QTimer>
#include <QWidget>

class AbstractPopupWidget : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(float _opacity READ getOpacity WRITE setOpacity);

public:
    explicit AbstractPopupWidget(QWidget* parent = nullptr);
    ~AbstractPopupWidget();

    void   setPosition(QPoint position);
    QPoint getPosition() const;

    bool getRoundedRect() const;
    void setRoundedRect(bool newRoundedRect);

    bool getAutoHideAnimation() const;
    void setAutoHideAnimation(bool newAutoHideAnimation);

private:
    void  setOpacity(float opacity);
    float getOpacity() const;

    // QWidget interface
public slots:
    void setContent();
    void show();

private slots:
    void hideAnimation();
    void hide();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;

private:
    QLabel* _label;

    QGridLayout        _layout;
    QPoint             _position;
    QPropertyAnimation _animation;
    float              _opacity;
    QTimer*            _timer;
    bool               _roundedRect;
    bool               _autoHideAnimation;
};

#endif  // ABSTRUCTPOPUPWIDGET_H
