#pragma once
#include <Style/Style.h>

#include "pch.h"

class BioButton : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor hover_color MEMBER hover_color NOTIFY animation)
public:
    explicit BioButton(QImage bio_image, bool in_caption = true, QWidget* parent = nullptr);

    void setImage(QImage new_image);

protected:
    bool eventFilter(QObject* object, QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

public slots:

    void animation();

private:
    QPropertyAnimation* fadein_anim;
    QColor hover_color;
    QImage image;
};
