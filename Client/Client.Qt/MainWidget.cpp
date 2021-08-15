#include "MainWidget.hpp"

#include <QDebug>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QWindow>
#include <QtEvents>

#include "Style/Shadow.hpp"

#ifdef _WIN32
#include <dwmapi.h>

bool isCompositionEnabled()
{
    auto result  = BOOL(FALSE);
    bool SUCCESS = (::DwmIsCompositionEnabled(&result) == S_OK);
    return SUCCESS && result;
}

bool MainWidget::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
    Q_UNUSED(eventType)
    MSG* msg = static_cast<MSG*>(message);

    if (msg->message == WM_NCACTIVATE)
    {
        // https://github.com/melak47/BorderlessWindow
        if (isCompositionEnabled())
        {
            const auto res = DefWindowProc(msg->hwnd, msg->message, msg->wParam, -1);
            if (result) *result = res;
        }
        else
        {
            if (result) *result = 1;
        }
        return true;
    }
    else if (msg->message == WM_NCCALCSIZE)
    {
        NCCALCSIZE_PARAMS& params = *reinterpret_cast<NCCALCSIZE_PARAMS*>(msg->lParam);
        if (this->isMaximized() && msg->wParam == TRUE)
        {
            const auto MONITOR = ::MonitorFromWindow(msg->hwnd, MONITOR_DEFAULTTONULL);
            if (!MONITOR) return false;
            MONITORINFO monitorInfo{};
            monitorInfo.cbSize = sizeof(MONITORINFO);
            if (!::GetMonitorInfoW(MONITOR, &monitorInfo)) return false;
            params.rgrc[0] = monitorInfo.rcWork;
        }

        if (result) *result = 0;
        return true;
    }
    else if (msg->message == WM_ERASEBKGND)
    {
        if (result) *result = 1;
        return true;
    }
    else if (msg->message == WM_SIZE)
    {
        WINDOWPLACEMENT wp;
        wp.length = sizeof(WINDOWPLACEMENT);
        ::GetWindowPlacement(reinterpret_cast<HWND>(winId()), &wp);
        Qt::WindowState state;
        if (msg->wParam == SIZE_MAXIMIZED)
        {
            state = Qt::WindowMaximized;
        }
        else if (msg->wParam == SIZE_MINIMIZED)
        {
            state = Qt::WindowMinimized;
        }
        else
        {
            state = Qt::WindowNoState;
        }
        window()->windowHandle()->windowStateChanged(state);
    }
    return false;
}
#endif

MainWidget::MainWidget() : QWidget(nullptr)
{
    Style::setDevicePixelRatio(devicePixelRatioF());
    /**
     * logicalDpiX() is current display dpi
     * devicePixelRatioF() is how much pixels will scale?
     * 100 is 100%
     * 96 is default dpi(100%)
     */
    Style::setDpiScale(logicalDpiX() * devicePixelRatioF() * 100 / 96);
    Style::startManager(Style::getDpiScale());

    setWindowFlag(Qt::FramelessWindowHint);
    setWindowIcon(QIcon(":/images/logo.png"));

    window()->createWinId();

    setMinimumWidth(st::minWidth);
    setMinimumHeight(st::minHeight);

    auto* grid = new QGridLayout(this);

    _body  = std::make_unique<QWidget>(this);
    _title = std::make_unique<TitleWidget>(this);

    grid->addWidget(_body.get(), 1, 0);
    grid->addWidget(_title.get(), 0, 0);
    grid->setSpacing(0);
    setLayout(grid);

    refreshTitleBar(false);
    qApp->installEventFilter(this);
}

void MainWidget::resizeEvent(QResizeEvent* event)
{
    if (_current >= 0) _widgets[_current]->resize(_body->width(), _body->height());
    return QWidget::resizeEvent(event);
}

void MainWidget::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setPen(Qt::NoPen);
    auto start = QColor(0, 0, 0, 0x18);
    auto end   = QColor(0, 0, 0, 0);
    drawShadow(p, 10, 2.0, start, end, 0.0, 1.0, 0.6, width(), height());
}

std::int32_t MainWidget::addWidget(std::unique_ptr<QWidget> widget)
{
    widget->setParent(_body.get());
    widget->hide();
    widget->resize(_body->width(), _body->height());
    _widgets.push_back(std::move(widget));
    return int(_widgets.size()) - 1;
}

void MainWidget::setCentralWidget(std::int32_t index)
{
    if (index >= 0 && index < std::int32_t(_widgets.size()))
    {
        _widgets[index]->resize(_body->width(), _body->height());
        _widgets[index]->show();
        if (_current >= 0) _widgets[_current]->hide();
        _current = index;
    }
}

void MainWidget::refreshTitleBar(bool showBioButton) { _title->showBioButton(showBioButton); }

bool MainWidget::eventFilter(QObject* obj, QEvent* e)
{
    if (e->type() == QEvent::MouseMove || e->type() == QEvent::MouseButtonPress)
    {
        if (obj->isWidgetType() && window()->isAncestorOf(static_cast<QWidget*>(obj)))
        {
            const auto mouseEvent   = static_cast<QMouseEvent*>(e);
            const auto currentPoint = mouseEvent->windowPos().toPoint();
            const auto edges        = edgesFromPos(currentPoint);

            if (e->type() == QEvent::MouseMove && mouseEvent->buttons() == Qt::NoButton)
            {
                updateCursor(edges);
            }

            if (e->type() == QEvent::MouseButtonPress && mouseEvent->button() == Qt::LeftButton && edges)
                return window()->windowHandle()->startSystemResize(edges);
        }
    }
    else if (e->type() == QEvent::Leave)
    {
        if (obj->isWidgetType() && window() == static_cast<QWidget*>(obj))
        {
            restoreCursor();
        }
    }
    return QObject::eventFilter(obj, e);
}

bool MainWidget::setBioButtonIcon(const Style::icon* icon) { return _title->setBioButtonIcon(icon); }

void MainWidget::updateCursor(Qt::Edges edges)
{
    if (!edges)
    {
        restoreCursor();
        return;
    }
    else if (!QGuiApplication::overrideCursor())
    {
        _cursorOverride = false;
    }

    if (!_cursorOverride)
    {
        _cursorOverride = true;
        QGuiApplication::setOverrideCursor(QCursor());
    }

    if (((edges & Qt::LeftEdge) && (edges & Qt::TopEdge)) || ((edges & Qt::RightEdge) && (edges & Qt::BottomEdge)))
    {
        QGuiApplication::changeOverrideCursor(Style::cur_sizefdiag);
    }
    else if (((edges & Qt::LeftEdge) && (edges & Qt::BottomEdge)) || ((edges & Qt::RightEdge) && (edges & Qt::TopEdge)))
    {
        QGuiApplication::changeOverrideCursor(Style::cur_sizebdiag);
    }
    else if ((edges & Qt::LeftEdge) || (edges & Qt::RightEdge))
    {
        QGuiApplication::changeOverrideCursor(Style::cur_sizehor);
    }
    else if ((edges & Qt::TopEdge) || (edges & Qt::BottomEdge))
    {
        QGuiApplication::changeOverrideCursor(Style::cur_sizever);
    }
}

void MainWidget::restoreCursor()
{
    if (_cursorOverride)
    {
        _cursorOverride = false;
        QGuiApplication::restoreOverrideCursor();
    }
}

Qt::Edges MainWidget::edgesFromPos(const QPoint& pos)
{
    const auto area = QRect(0, 0, width(), height()).marginsRemoved(QMargins(9, 9, 9, 9));

    if (area.isNull() || isMaximized())
    {
        return Qt::Edges();
    }
    else if (pos.x() <= area.left())
    {
        if (pos.y() <= area.top())
            return Qt::LeftEdge | Qt::TopEdge;
        else if (pos.y() >= (window()->height() - area.top()))
            return Qt::LeftEdge | Qt::BottomEdge;

        return Qt::LeftEdge;
    }
    else if (pos.x() >= (window()->width() - area.left()))
    {
        if (pos.y() <= area.top())
            return Qt::RightEdge | Qt::TopEdge;
        else if (pos.y() >= (window()->height() - area.top()))
            return Qt::RightEdge | Qt::BottomEdge;

        return Qt::RightEdge;
    }
    else if (pos.y() <= area.top())
        return Qt::TopEdge;
    else if (pos.y() >= (window()->height() - area.top()))
        return Qt::BottomEdge;

    return Qt::Edges();
}
