#pragma once
#include <QWidget>
#include <QWidgetAction>
#include <Style/Styles.hpp>

#include "Buttons.hpp"

struct CallbackData
{
    QAction* action;
    int      actionTop = 0;
    int      index     = 0;
    bool     selected  = false;
};

class ItemBase : public AbstractButton
{
    Q_OBJECT
public:
    explicit ItemBase(QWidget* parent = nullptr, const Style::Menu& st = st::defaultMenu);

    [[nodiscard]] virtual QAction* action() const    = 0;
    [[nodiscard]] virtual bool     isEnabled() const = 0;

    void               setSelected(bool selected);
    [[nodiscard]] bool isSelected() const;

    int  index() const;
    void setIndex(int32_t index);

    [[nodiscard]] virtual int contentHeight() const = 0;

signals:
    void clicked(const CallbackData& data);
    void selected(const CallbackData& data);

protected:
    bool eventFilter(QObject* obj, QEvent* e) override;

private:
    int32_t _index = -1;

    bool _selected = false;
};

class WidgetItem : public ItemBase
{
public:
    explicit WidgetItem(QWidget* parent, QWidgetAction* action);
    [[nodiscard]] QAction* action() const override;
    [[nodiscard]] bool     isEnabled() const override;
    [[nodiscard]] int      contentHeight() const override;

private:
    QWidgetAction* _action;
};

class MenuItem : public ItemBase
{
public:
    MenuItem(QWidget* parent, const Style::Menu& st, QAction* action, const Style::icon* icon, const Style::icon* iconOver);

    [[nodiscard]] QAction* action() const override;
    [[nodiscard]] bool     isEnabled() const override;

protected:
    void paintEvent(QPaintEvent* event) override;

    [[nodiscard]] int contentHeight() const override;

private:
    void processAction();

private:
    QString            _text;
    const QAction*     _action;
    const Style::Menu& _st;
    const Style::icon* _icon;
    const Style::icon* _iconOver;
    int32_t            _textWidth;
    const int          _height;
};

class Separator : public ItemBase
{
public:
    Separator(QWidget* parent = nullptr, const Style::Menu& st = st::defaultMenu, QAction* action = nullptr);

    [[nodiscard]] QAction* action() const override;
    [[nodiscard]] bool     isEnabled() const override;

protected:
    [[nodiscard]] int contentHeight() const override;
    void              paintEvent(QPaintEvent* event) override;

private:
    const int             _lineWidth;
    const Style::margins& _padding;
    const Style::color&   _fg;
    const Style::color&   _bg;
    const int             _height;
    QAction*              _action;
};
