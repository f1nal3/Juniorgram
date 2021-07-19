#include "MainWidget.hpp"

#include <QDebug>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QWindow>
#include <QtEvents>
#include <iostream>

#include "Style/Shadow.hpp"
#include "Widgets/BioButton.hpp"

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
        /*
         * https://github.com/melak47/BorderlessWindow
         */
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
    else if (msg->message == WM_NCHITTEST)
    {
        const auto P = MAKEPOINTS(msg->lParam);
        if (P.x - this->x() <= 9)
        {
            if (result) *result = HTLEFT;
            return true;
        }
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
        emit(window())->windowHandle()->windowStateChanged(state);
    }
    return false;
}
#endif

MainWidget::MouseType MainWidget::checkResizableField(QMouseEvent* event)
{
    QPointF position = event->globalPos();
    qreal   x        = this->x();
    qreal   y        = this->y();
    qreal   width    = this->width();
    qreal   height   = this->height();

    QRectF rectTop(x, y, width + 9, 8);
    QRectF rectBottom(x, y + height - 8, width + 9, 8);
    QRectF rectLeft(x, y, 8, height + 9);
    QRectF rectRight(x + width - 8, y, 8, height + 9);
    QRectF rectInterface(x + 9, y + 9, width - 18 - Style::valueDPIScale(46) * 3, Style::valueDPIScale(30));

    if (!isMaximized())
    {
        if (rectTop.contains(position))
        {
            if (rectLeft.contains(position))
            {
                setCursor(Qt::SizeFDiagCursor);
                return TopLeft;
            }
            if (rectRight.contains(position))
            {
                setCursor(Qt::SizeBDiagCursor);
                return TopRight;
            }
            setCursor(Qt::SizeVerCursor);
            return Top;
        }
        else if (rectBottom.contains(position))
        {
            if (rectLeft.contains(position))
            {
                setCursor(Qt::SizeBDiagCursor);
                return BottomLeft;
            }
            if (rectRight.contains(position))
            {
                setCursor(Qt::SizeFDiagCursor);
                return BottomRight;
            }
            setCursor(Qt::SizeVerCursor);
            return Bottom;
        }
        else if (rectLeft.contains(position))
        {
            setCursor(Qt::SizeHorCursor);
            return Left;
        }
        else if (rectRight.contains(position))
        {
            setCursor(Qt::SizeHorCursor);
            return Right;
        }
    }
    setCursor(QCursor());
    return None;
}

void MainWidget::mousePressEvent(QMouseEvent* event)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    if (event->button() == Qt::LeftButton)
    {
        _lmbPos = checkResizableField(event);
        if (_lmbPos == Top)
        {
            this->windowHandle()->startSystemResize(Qt::TopEdge);
        }
        else if (_lmbPos == TopLeft)
        {
            this->windowHandle()->startSystemResize(Qt::TopEdge | Qt::LeftEdge);
        }
        else if (_lmbPos == TopRight)
        {
            this->windowHandle()->startSystemResize(Qt::TopEdge | Qt::RightEdge);
        }
        else if (_lmbPos == Bottom)
        {
            this->windowHandle()->startSystemResize(Qt::BottomEdge);
        }
        else if (_lmbPos == BottomLeft)
        {
            this->windowHandle()->startSystemResize(Qt::BottomEdge | Qt::LeftEdge);
        }
        else if (_lmbPos == BottomRight)
        {
            this->windowHandle()->startSystemResize(Qt::BottomEdge | Qt::RightEdge);
        }
        else if (_lmbPos == Left)
        {
            this->windowHandle()->startSystemResize(Qt::LeftEdge);
        }
        else if (_lmbPos == Right)
        {
            this->windowHandle()->startSystemResize(Qt::RightEdge);
        }
    }
#endif

    return QWidget::mousePressEvent(event);
}

void MainWidget::mouseMoveEvent(QMouseEvent* event)
{
    checkResizableField(event);
    switch (_lmbPos)
    {
        case Top:
        case Bottom:
        case Left:
        case Right:
        {
            if (!isMaximized())
            {
            }
            break;
        }
        default:
            checkResizableField(event);
            break;
    }
    return QWidget::mouseMoveEvent(event);
}

void MainWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        _lmbPos = None;
    }
    return QWidget::mouseReleaseEvent(event);
}

MainWidget::MainWidget(QWidget* parent) : QWidget(parent)
{
    Style::setDevicePixelRatio(devicePixelRatioF());
    Style::setDpiScale(logicalDpiX() * devicePixelRatioF() * 100 / 96);
    Style::startManager(Style::getDpiScale());

    setWindowFlag(Qt::FramelessWindowHint);
    setWindowIcon(QIcon(":/images/logo.png"));
    setMinimumWidth(Style::valueDPIScale(800));

    window()->createWinId();
    auto* grid = new QGridLayout(this);

    _body = std::make_unique<QWidget>();
    _body->setMinimumHeight(Style::valueDPIScale(480));

    _title = std::make_unique<TitleWidget>(this);

    grid->addWidget(_body.get(), 1, 0);
    grid->addWidget(_title.get(), 0, 0);
    grid->setSpacing(0);
    setLayout(grid);

    _body->setStyleSheet(
        "QWidget {"
        "background-color: #323232;"
        "}");

    std::cout << QGuiApplication::platformName().toStdString() << std::endl;

    refreshTitleBar();

    setAttribute(Qt::WA_Hover);
    setMouseTracking(true);
    installEventFilter(this);
}

void MainWidget::resizeEvent(QResizeEvent* event)
{
    if (_current >= 0) _widgets[_current]->resize(_body->width(), _body->height());
    return QWidget::resizeEvent(event);
}

void MainWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
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

void MainWidget::refreshTitleBar(BioButton*)
{
    /*if (!pTitleLayout->isEmpty())
    {
    }
    if (bio_button)
    {
        pTitleLayout->addWidget(bio_button);
    }*/
}
bool MainWidget::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::HoverMove)
    {
        checkResizableField(static_cast<QMouseEvent*>(event));
    }
    return QObject::eventFilter(watched, event);
}