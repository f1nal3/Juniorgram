#pragma once

#include <QGridLayout>
#include <QPropertyAnimation>
#include <QTimer>

#include <memory>

#include "AbstractPopupMessage.hpp"

class PopupMessage : public AbstractPopupMessage
{
public:
    explicit PopupMessage(QWidget* parent = nullptr);
    
    ~PopupMessage() override = default;

public slots:
    void popupShow() override;

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void hideAnimation();
    void hide();

private:
    QGridLayout              _layout;
    QPropertyAnimation      _animation;
    std::unique_ptr<QTimer> _timer;
};
