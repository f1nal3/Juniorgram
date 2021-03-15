//
// Created by Stanislav on 11.03.2021.
//

#ifndef JUNIORGRAMUI_INPUT_FIELDS_H
#define JUNIORGRAMUI_INPUT_FIELDS_H

#include "../pch.h"
#include "../Style/style.h"

class FlatInput : public QLineEdit {
Q_OBJECT
public:
    explicit FlatInput(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *e) override {
        QLineEdit::paintEvent(e);/*
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        p.setBrush(Qt::red);
        p.setPen(Qt::NoPen);
        p.drawRoundedRect(QRect(50,0,width()-100,height()),5,5);*/
    }
};

#endif //JUNIORGRAMUI_INPUT_FIELDS_H
