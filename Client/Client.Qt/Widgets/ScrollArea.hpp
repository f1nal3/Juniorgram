#pragma once
#include <QScrollArea>
#include <QScrollBar>
#include <QTimer>
#include <QWidget>

#include "Style/StyleBasic.hpp"
class ScrollArea;

class ScrollBar : public QWidget
{
    Q_OBJECT

public:
    ScrollBar(ScrollArea* parent, bool vertical, const Style::ScrollArea* st);

    void recountSize();
    void updateBar(bool force = false);

    void hideTimeout(int dtms);

private Q_SLOTS:
    void onValueChanged();
    void onRangeChanged();
    void onHideTimer();

protected:
    void paintEvent(QPaintEvent* e) override;
    void enterEvent(QEvent* e) override;
    void leaveEvent(QEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void resizeEvent(QResizeEvent* e) override;

private:
    ScrollArea* area();

    void setOver(bool over);
    void setOverBar(bool overbar);
    void setMoving(bool moving);

    const Style::ScrollArea* _st;

    bool _vertical = true;
    bool _hiding   = false;
    bool _over     = false;
    bool _overbar  = false;
    bool _moving   = false;

    QPoint      _dragStart;
    QScrollBar* _connected;

    int32_t _startFrom = 0, _scrollMax = 0;

    int    _hideIn = 0;
    QTimer _hideTimer;

    QRect _bar;
};

class ScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    explicit ScrollArea(QWidget* parent, bool handleTouch = true,
                        const Style::ScrollArea& st = st::defaultScrollArea);

    int scrollWidth() const;
    int scrollHeight() const;
    int scrollLeftMax() const;
    int scrollTopMax() const;
    int scrollLeft() const;
    int scrollTop() const;

    void setMovingByScrollBar(bool movingByScrollBar);
    void scrollToWidget(QWidget* widget);

    void updateBars();
    void rangeChanged(int oldMax, int newMax, bool vertical);

protected:
    void resizeEvent(QResizeEvent* e) override;
    void moveEvent(QMoveEvent* e) override;

    void enterEvent(QEvent* e) override;
    void leaveEvent(QEvent* e) override;
    void keyPressEvent(QKeyEvent* e) override;

public Q_SLOTS:
    void scrollToY(int toTop, int toBottom = -1);
    void disableScroll(bool dis);
    void onScrolled();
    void onInnerResized();

Q_SIGNALS:
    void scrolled();
    void innerResized();
    void scrollStarted();
    void scrollFinished();
    void geometryChanged();

protected:
    void scrollContentsBy(int dx, int dy) override;

private:
    const Style::ScrollArea& _st;

    ScrollBar _horizontalBar, _verticalBar;

    bool _touchEnabled;
    bool _disabled;
    bool _movingByScrollBar;

    int _horizontalValue, _verticalValue;
};
