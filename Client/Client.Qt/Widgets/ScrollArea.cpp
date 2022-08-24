#include "ScrollArea.hpp"

#include <QPainter>
#include <QtEvents>

ScrollBar::ScrollBar(ScrollArea* parent, bool vertical, const Style::ScrollArea* st)
    : QWidget(parent),
      _st(st),
      _vertical(vertical),
      _hiding(_st->hiding != 0),
      _connected(vertical ? parent->verticalScrollBar() : parent->horizontalScrollBar()),
      _scrollMax(_connected->maximum())
{
    updateSize();

    _hideTimer.setSingleShot(true);
    connect(&_hideTimer, SIGNAL(timeout()), this, SLOT(onHideTimer()));

    connect(_connected, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged()));
    connect(_connected, SIGNAL(rangeChanged(int, int)), this, SLOT(onRangeChanged()));

    updateBar();
}

void ScrollBar::updateSize()
{
    setGeometry(_vertical ? QRect(area()->width() - _st->width, _st->deltaT, _st->width, area()->height() - _st->deltaT - _st->deltaB)
                          : QRect(_st->deltaT, area()->height() - _st->width, area()->width() - _st->deltaT - _st->deltaB, _st->width));
}

void ScrollBar::updateBar(bool)
{
    QRect newBar;
    if (_connected->maximum() != _scrollMax)
    {
        int32_t oldMax = _scrollMax;
        int32_t newMax = _connected->maximum();
        _scrollMax = newMax;
        area()->rangeChanged(oldMax, newMax, _vertical);
    }
    if (_vertical)
    {
        int scrollHeight = area()->scrollHeight();
        int canvasHeight = height();
        int height = area()->height();
        int       lHeight       = getLocation(scrollHeight, canvasHeight,height);
        const int minHeight     = 30;
        if (lHeight >= canvasHeight || !area()->scrollTopMax() || canvasHeight < minHeight)
        {
            if (!isHidden()) hide();
            return;
        }

        if (lHeight <= minHeight) lHeight = minHeight;
        int scrollTopMax = area()->scrollTopMax();
        int scrollTop    = area()->scrollTop();
        int leftCorner   = getLeftCorner(scrollTopMax, canvasHeight, lHeight, scrollTop);
        if (leftCorner > canvasHeight - lHeight) leftCorner = canvasHeight - lHeight;

        newBar = QRect(_st->deltaX, leftCorner, width() - 2 * _st->deltaX, lHeight);
    }
    else
    {
        int scrollWidth  = area()->scrollWidth();
        int canvasWidth  = width();
        int       width         = area()->width();
        int       lWidth        = getLocation(scrollWidth, canvasWidth,width);
        const int minWidth      = 30;
        if (lWidth >= canvasWidth || !area()->scrollLeftMax() || canvasWidth < minWidth)
        {
            if (!isHidden()) hide();
            return;
        }

        if (lWidth <= minWidth) lWidth = minWidth;
        int scrollLeftMax = area()->scrollLeftMax();
        int scrollLeft    = area()->scrollLeft();
        int leftCorner    = getLeftCorner(scrollLeftMax, canvasWidth, lWidth, scrollLeft);
        if (leftCorner > canvasWidth - lWidth) leftCorner = canvasWidth - lWidth;

        newBar = QRect(leftCorner, _st->deltaX, lWidth, height() - 2 * _st->deltaX);
    }
    if (newBar != _bar)
    {
        _bar = newBar;
        update();
    }
    if (isHidden()) show();
}

void ScrollBar::hideTimeout(int dtms)
{
    if (_hiding && dtms > 0)
    {
        _hiding = false;
        update();
    }
    _hideIn = dtms;
    if (!_moving)
    {
        _hideTimer.start(_hideIn);
    }
}

void ScrollBar::paintEvent(QPaintEvent*)
{
    if (!_bar.width() && !_bar.height())
    {
        hide();
        return;
    }

    auto opacity = _hiding ? 0. : 1.;
    if (opacity == 0.) return;

    QPainter p(this);
    auto deltaL = _vertical ? _st->deltaX : 0;
    auto deltaR = _vertical ? _st->deltaX : 0;
    auto deltaT = _vertical ? 0 : _st->deltaX;
    auto deltaB = _vertical ? 0 : _st->deltaX;
    p.setPen(Qt::NoPen);
    auto bg = (_over || _moving) ? _st->bgOver->color : _st->bg->color;
    bg.setAlpha(static_cast<int>(bg.alpha() * opacity));
    auto bar = (_overBar || _moving) ? _st->barBgOver->color : _st->barBg->color;
    bar.setAlpha(static_cast<int>(bar.alpha() * opacity));
    if (_st->round)
    {
        p.setRenderHint(QPainter::Antialiasing);
        p.setBrush(bg);
        p.drawRoundedRect(QRect(deltaL, deltaT, width() - deltaL - deltaR, height() - deltaT - deltaB), _st->round, _st->round);
        p.setBrush(bar);
        p.drawRoundedRect(_bar, _st->round, _st->round);
    }
    else
    {
        p.fillRect(QRect(deltaL, deltaT, width() - deltaL - deltaR, height() - deltaT - deltaB), bg);
        p.fillRect(_bar, bar);
    }
}

ScrollArea* ScrollBar::area() { return static_cast<ScrollArea*>(parentWidget()); }

void ScrollBar::onValueChanged()
{
    area()->onScrolled();
    updateBar();
}

void ScrollBar::onRangeChanged()
{
    area()->onInnerResized();
    updateBar();
}

void ScrollBar::onHideTimer()
{
    if (!_hiding)
    {
        _hiding = true;
        update();
    }
}

void ScrollBar::setOver(bool over)
{
    if (_over != over)
    {
        auto wasOver = (_over || _moving);
        _over        = over;
        auto nowOver = (_over || _moving);
        if (wasOver != nowOver)
        {
            update();
        }
        if (nowOver && _hiding)
        {
            _hiding = false;
            update();
        }
    }
}

void ScrollBar::setOverBar(bool overBar)
{
    if (_overBar != overBar)
    {
        auto wasBarOver = (_overBar || _moving);
        _overBar        = overBar;
        auto nowBarOver = (_overBar || _moving);
        if (wasBarOver != nowBarOver)
        {
            update();
        }
    }
}

void ScrollBar::setMoving(bool moving)
{
    if (_moving != moving)
    {
        auto wasOver    = (_over || _moving);
        auto wasBarOver = (_overBar || _moving);
        _moving         = moving;
        auto nowBarOver = (_overBar || _moving);
        if (wasBarOver != nowBarOver)
        {
            update();
        }
        auto nowOver = (_over || _moving);
        if (wasOver != nowOver)
        {
            update();
        }
        if (!nowOver && _st->hiding && !_hiding)
        {
            _hideTimer.start(_hideIn);
        }
    }
}

constexpr int ScrollBar::getLocation(int scrollValue, int canvasValue,int areaValue) const
{
    return (scrollValue ? static_cast<int32_t>((canvasValue * static_cast<int64_t>(areaValue)) / scrollValue) : 0);
}

constexpr int ScrollBar::getLeftCorner(int scrollValueMax,int canvasValue,int lValue,int scrollValue) const
{
    return (scrollValueMax ? static_cast<int32_t>(((canvasValue - lValue) * static_cast<int64_t>(scrollValue)) / scrollValueMax) : 0);
}

void ScrollBar::enterEvent(QEvent*)
{
    _hideTimer.stop();
    setMouseTracking(true);
    setOver(true);
}

void ScrollBar::leaveEvent(QEvent*)
{
    if (!_moving)
    {
        setMouseTracking(false);
    }
    setOver(false);
    setOverBar(false);
    if (_st->hiding && !_hiding)
    {
        _hideTimer.start(_hideIn);
    }
}

void ScrollBar::mouseMoveEvent(QMouseEvent* e)
{
    setOverBar(_bar.contains(e->pos()));
    if (_moving)
    {
        int delta = 0;
        int barDelta = _vertical ? (area()->height() - _bar.height()) : (area()->width() - _bar.width());
        if (barDelta > 0)
        {
            QPoint d = (e->globalPos() - _dragStart);
            delta =
                static_cast<int32_t>((_vertical ? (d.y() * static_cast<int64_t>(area()->scrollTopMax())) : (d.x() * static_cast<int64_t>(area()->scrollLeftMax()))) / barDelta);
        }
        _connected->setValue(_startFrom + delta);
    }
}

void ScrollBar::mousePressEvent(QMouseEvent* e)
{
    if (!width() || !height()) return;

    _dragStart = e->globalPos();
    setMoving(true);
    if (_overBar)
    {
        _startFrom = _connected->value();
    }
    else
    {
        int32_t val = _vertical ? e->pos().y() : e->pos().x();
        int32_t division = _vertical ? height() : width();

        val         = (val <= _st->deltaT) ? 0 : (val - _st->deltaT);
        division         = (division <= _st->deltaT + _st->deltaB) ? 1 : (division - _st->deltaT - _st->deltaB);
        _startFrom  = _vertical ? static_cast<int32_t>((val * static_cast<int64_t>(area()->scrollTopMax())) / division)
                                : ((val * area()->scrollLeftMax()) / division);
        _connected->setValue(_startFrom);
        setOverBar(true);
    }

    area()->setMovingByScrollBar(true);
    area()->scrollStarted();
}

void ScrollBar::mouseReleaseEvent(QMouseEvent*)
{
    if (_moving)
    {
        setMoving(false);

        area()->setMovingByScrollBar(false);
        area()->scrollFinished();
    }
    if (!_over)
    {
        setMouseTracking(false);
    }
}

void ScrollBar::resizeEvent(QResizeEvent*) { updateBar(); }

ScrollArea::ScrollArea(QWidget* parent, bool handleTouch, const Style::ScrollArea& st)
    : QScrollArea(parent), _st(st), _horizontalBar(this, false, &_st), _verticalBar(this, true, &_st), _touchEnabled(handleTouch)
{
    setLayoutDirection(Qt::LayoutDirection::LeftToRight);
    setFocusPolicy(Qt::NoFocus);

    _verticalBar.updateBar();

    verticalScrollBar()->setSingleStep(Style::valueDPIScale(verticalScrollBar()->singleStep()));
    horizontalScrollBar()->setSingleStep(Style::valueDPIScale(horizontalScrollBar()->singleStep()));

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setFrameStyle(QFrame::NoFrame | QFrame::Plain);
    viewport()->setAutoFillBackground(false);

    _horizontalValue = horizontalScrollBar()->value();
    _verticalValue   = verticalScrollBar()->value();
}

void ScrollArea::onScrolled()
{
    bool changed         = false;
    int  horizontalValue = horizontalScrollBar()->value();
    int  verticalValue   = verticalScrollBar()->value();
    if (_horizontalValue != horizontalValue)
    {
        if (_disabled)
        {
            horizontalScrollBar()->setValue(_horizontalValue);
        }
        else
        {
            _horizontalValue = horizontalValue;
            if (_st.hiding)
            {
                _horizontalBar.hideTimeout(_st.hiding);
            }
            changed = true;
        }
    }
    if (_verticalValue != verticalValue)
    {
        if (_disabled)
        {
            verticalScrollBar()->setValue(_verticalValue);
        }
        else
        {
            _verticalValue = verticalValue;
            if (_st.hiding)
            {
                _verticalBar.hideTimeout(_st.hiding);
            }
            changed = true;
        }
    }
    if (changed)
    {
        scrolled();
    }
}

void ScrollArea::onInnerResized() { innerResized(); }

int ScrollArea::scrollWidth() const
{
    const QWidget* w(widget());
    return w ? qMax(w->width(), width()) : width();
}

int ScrollArea::scrollHeight() const
{
    const QWidget* w(widget());
    return w ? qMax(w->height(), height()) : height();
}

int ScrollArea::scrollLeftMax() const { return scrollWidth() - width(); }

int ScrollArea::scrollTopMax() const { return scrollHeight() - height(); }

int ScrollArea::scrollLeft() const { return _horizontalValue; }

int ScrollArea::scrollTop() const { return _verticalValue; }

void ScrollArea::resizeEvent(QResizeEvent* e)
{
    QScrollArea::resizeEvent(e);
    _horizontalBar.updateSize();
    _verticalBar.updateSize();
    geometryChanged();
}

void ScrollArea::moveEvent(QMoveEvent* e)
{
    QScrollArea::moveEvent(e);
    geometryChanged();
}

void ScrollArea::keyPressEvent(QKeyEvent* e)
{
    if ((e->key() == Qt::Key_Up || e->key() == Qt::Key_Down) && e->modifiers().testFlag(Qt::AltModifier))
    {
        e->ignore();
    }
    else if (e->key() == Qt::Key_Escape || e->key() == Qt::Key_Back)
    {
        ((QObject*)widget())->event(e);
    }
    else
    {
        QScrollArea::keyPressEvent(e);
    }
}

void ScrollArea::enterEvent(QEvent* e)
{
    if (_disabled) return;
    if (_st.hiding)
    {
        _horizontalBar.hideTimeout(_st.hiding);
        _verticalBar.hideTimeout(_st.hiding);
    }
    return QScrollArea::enterEvent(e);
}

void ScrollArea::leaveEvent(QEvent* e)
{
    if (_st.hiding)
    {
        _horizontalBar.hideTimeout(2000);
        _verticalBar.hideTimeout(2000);
    }
    return QScrollArea::leaveEvent(e);
}

void ScrollArea::scrollToWidget(const QWidget* widget) const
{
    if (auto local = this->widget())
    {
        auto globalPosition = widget->mapToGlobal(QPoint(0, 0));
        auto localPosition  = local->mapFromGlobal(globalPosition);
        auto localTop       = localPosition.y();
        auto localBottom    = localTop + widget->height();
        scrollToY(localTop, localBottom);
    }
}

void ScrollArea::scrollToY(int toTop, int toBottom) const
{
    int toMin = 0;
    int toMax = scrollTopMax();
    if (toTop < toMin)
    {
        toTop = toMin;
    }
    else if (toTop > toMax)
    {
        toTop = toMax;
    }
    bool exact = (toBottom < 0);

    int curTop = scrollTop();
    int curHeight = height();
    int curBottom = curTop + curHeight;
    int scToTop   = toTop;
    if (!exact && toTop >= curTop)
    {
        if (toBottom < toTop) toBottom = toTop;
        if (toBottom <= curBottom) return;

        scToTop = toBottom - curHeight;
        if (scToTop > toTop) scToTop = toTop;
        if (scToTop == curTop) return;
    }
    else
    {
        scToTop = toTop;
    }
    verticalScrollBar()->setValue(scToTop);
}

void ScrollArea::rangeChanged(int oldMax, int newMax, bool vertical) const
{
    Q_UNUSED(oldMax)
    Q_UNUSED(newMax)
    Q_UNUSED(vertical)
}

void ScrollArea::disableScroll(bool dis)
{
    _disabled = dis;
    if (_disabled && _st.hiding)
    {
        _horizontalBar.hideTimeout(0);
        _verticalBar.hideTimeout(0);
    }
}

void ScrollArea::scrollContentsBy(int dx, int dy)
{
    if (_disabled)
    {
        return;
    }
    QScrollArea::scrollContentsBy(dx, dy);
}

void ScrollArea::setMovingByScrollBar(bool movingByScrollBar) { _movingByScrollBar = movingByScrollBar; }

void ScrollArea::updateBars()
{
    _verticalBar.updateBar();
    _horizontalBar.updateBar();
}

void ScrollArea::setOwnedWidget(std::unique_ptr<QWidget> w)
{
    if (widget())
    {
        // T\todo if we will something done to widget undo it
    }
    _widget = std::move(w);
    QScrollArea::setWidget(_widget.get());
    if (_widget)
    {
        _widget->setAutoFillBackground(false);
    }
}

std::unique_ptr<QWidget> ScrollArea::takeWidget()
{
    QScrollArea::takeWidget();
    return std::move(_widget);
}
