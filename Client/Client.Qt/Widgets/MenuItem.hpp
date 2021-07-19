#pragma once
#include <QWidget>

#include "AbstractButton.hpp"

class ItemBase : public AbstractButton
{
    Q_OBJECT
public:
    explicit ItemBase(QString text, QAction* action, QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* paintEvent) override;
    void onStateChanged(State was, StateChanger source) override
    {
        Q_UNUSED(was)
        Q_UNUSED(source)
        update();
    }

private:
    QString  _text;
    QAction* _action;
};
