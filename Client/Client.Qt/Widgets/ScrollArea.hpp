#pragma once
#include <QScrollArea>
#include <QScrollBar>
#include <QTimer>
#include <QWidget>
#include <Style/Styles.hpp>

class ScrollArea;

/**
 * @class ScrollBar
 * @brief Custom scrollbar visual for scrollarea
 */
class ScrollBar : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for scrollbar, creates from ScrollArea directly
     * @param parent Scrollarea attached to
     * @param vertical Need to be vertical?
     * @param st Styles
     */
    ScrollBar(ScrollArea* parent, bool vertical, const Style::ScrollArea* st);

    /// updates position and size
    void updateSize();

    /**
     * @brief Update bar visual representation
     * @param force Redraw by force
     */
    void updateBar(bool force = false);

    /**
     * @brief Hide scrollbar in time
     * @param dtms Milliseconds before hide
     */
    void hideTimeout(int dtms);

private Q_SLOTS:
    /// on scrollbar move
    void onValueChanged();
    /// on scrollbar range change
    void onRangeChanged();
    /// time to hide
    void onHideTimer();

protected:
    /// Handle paint
    void paintEvent(QPaintEvent* e) override;
    /// Handle mouse enter event
    void enterEvent(QEvent* e) override;
    /// Handle mouse leave event
    void leaveEvent(QEvent* e) override;
    /// Handle mouse move event
    void mouseMoveEvent(QMouseEvent* e) override;
    /// Handle mouse press event
    void mousePressEvent(QMouseEvent* e) override;
    /// Handle mouse click event
    void mouseReleaseEvent(QMouseEvent* e) override;
    /// Handle resize
    void resizeEvent(QResizeEvent* e) override;

private:
    /// ScrollArea this scrollbar are connected to
    ScrollArea* area();

    /**
     * @brief Is mouse over?
     * @param over Is mouse over?
     */
    void setOver(bool over);
    /**
     * @brief Is mouse over bar?
     * @param overBar is mouse over bar?
     */
    void setOverBar(bool overBar);
    /**
     * @brief Does mouse been pressed on bar
     * @param moving
     */
    void setMoving(bool moving);

    const Style::ScrollArea* _st;

    bool _vertical = true;
    bool _hiding   = false;
    bool _over     = false;
    bool _overBar  = false;
    bool _moving   = false;

    QPoint      _dragStart;
    QScrollBar* _connected;

    int32_t _startFrom = 0;
    int32_t _scrollMax = 0;

    int    _hideIn = 0;
    QTimer _hideTimer;

    QRect _bar;
};

/**
 * @class ScrollArea
 * @brief ScrollArea with custom scrollbars
 */
class ScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    /**
     * @brief Constructor for ScrollArea
     * @param parent where we put our scrollarea
     * @param handleTouch should we tahandle touches(not working for now)
     * @param st Styles
     */
    explicit ScrollArea(QWidget* parent, bool handleTouch = true, const Style::ScrollArea& st = st::defaultScrollArea);

    /**
     * @brief Returns possible scroll width
     * @return Scroll width
     */
    [[nodiscard]] int scrollWidth() const;
    /**
     * @brief Returns possible scroll height
     * @return Scroll width
     */
    [[nodiscard]] int scrollHeight() const;
    /**
     * @brief Returns possible horizontal scroll
     * @return Possible horizontal scroll
     */
    [[nodiscard]] int scrollLeftMax() const;
    /**
     * @brief Returns possible vertical scroll
     * @return Possible vertical scroll
     */
    [[nodiscard]] int scrollTopMax() const;
    /**
     * @brief Returns current horizontal scroll
     * @return Current horizontal scroll
     */
    [[nodiscard]] int scrollLeft() const;
    /**
     * @brief Returns current vertical scroll
     * @return Current vertical scroll
     */
    [[nodiscard]] int scrollTop() const;

    /**
     * @brief Getting know that we are scrolling by scrollbar
     * @param movingByScrollBar Do we scroll by scroll bar?
     */
    void setMovingByScrollBar(bool movingByScrollBar);
    /**
     * @brief Scrolls to make widget visible
     * @param widget Widget to which we are scrolling
     */
    void scrollToWidget(const QWidget* widget) const;

    /// Update scrollbars
    void updateBars();

    /**
     * @brief Range of a scrollbar has changed
     * @param oldMax old range
     * @param newMax new range
     * @param vertical vertical?
     */
    void rangeChanged(int oldMax, int newMax, bool vertical) const;

    /**
     * @brief Takes control on widget
     * @param widget widget to take control on
     */
    void setOwnedWidget(std::unique_ptr<QWidget> widget);

    /**
     * @brief Return widget and remove ownership
     * @return Widget
     */
    std::unique_ptr<QWidget> takeWidget();

protected:
    /// Handle resize
    void resizeEvent(QResizeEvent* e) override;
    /// Handle move event
    void moveEvent(QMoveEvent* e) override;

    /// Handle mouse enter event
    void enterEvent(QEvent* e) override;
    /// Handle mouse leave event
    void leaveEvent(QEvent* e) override;
    /// Handle key press event
    void keyPressEvent(QKeyEvent* e) override;

public Q_SLOTS:
    /**
     * @brief Scroll to toTop but not bottom as toBottom
     */
    void scrollToY(int toTop, int toBottom = -1) const;
    /**
     * @brief Disable scrolling by user
     * @param dis Disable scrolling?
     */
    void disableScroll(bool dis);
    /**
     * @brief Widget has been scrolled
     */
    void onScrolled();
    /**
     * @brief Inner widget has been resized
     */
    void onInnerResized();

Q_SIGNALS:
    /// area has been scrolled
    void scrolled();
    /// inner widget changed size
    void innerResized();
    /// scroll just started
    void scrollStarted();
    /// scroll just ended
    void scrollFinished();
    /// scroll area just resized/moved
    void geometryChanged();

protected:
    /// Override to make scroll locking possible
    void scrollContentsBy(int dx, int dy) override;

private:
    const Style::ScrollArea& _st;

    ScrollBar _horizontalBar;
    ScrollBar _verticalBar;

    bool _touchEnabled      = false;
    bool _disabled          = false;
    bool _movingByScrollBar = false;

    std::unique_ptr<QWidget> _widget;

    int _horizontalValue;
    int _verticalValue;
};
