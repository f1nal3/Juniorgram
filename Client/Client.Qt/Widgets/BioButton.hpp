#pragma once
#include <Style/Style.hpp>

#include "pch.hpp"

class BioButton : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor hover_color MEMBER hoverColor NOTIFY animation)
public:
    explicit BioButton(QImage bio_image, bool in_caption = true, QWidget* parent = nullptr);

    void setImage(QImage new_image);

protected:
    bool eventFilter(QObject* object, QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

public slots:

    void animation();

private:
    QPropertyAnimation* fadeinAnim;
    QColor hoverColor;
    QImage image;
};
