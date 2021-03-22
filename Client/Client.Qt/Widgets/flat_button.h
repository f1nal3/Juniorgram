#ifndef JUNIORGRAM_FLAT_BUTTON_H
#define JUNIORGRAM_FLAT_BUTTON_H

#include "../pch.h"

class FlatButton : public QPushButton
{
public:
    explicit FlatButton(const QString& text, QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif  // JUNIORGRAM_FLAT_BUTTON_H
