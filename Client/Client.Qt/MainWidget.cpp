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
            const auto RES = DefWindowProc(msg->hwnd, msg->message, msg->wParam, -1);
            if (result) *result = RES;
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
        if (P.x <= 9)
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
    QPointF position = event->screenPos();
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
    if (rectInterface.contains(position))
    {
        setCursor(QCursor());
        return Move;
    }
    else
    {
        setCursor(QCursor());
        return None;
    }
}

void MainWidget::mousePressEvent(QMouseEvent* event)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    if (event->button() == Qt::LeftButton)
    {
        _mousePressed            = true;
        _lmbPos                  = checkResizableField(event);
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
        case Move:
        {
            if (_mousePressed)
            {
                /*if (isMaximized())
                {
                    this->layout()->setMargin(9);
                    auto part = event->screenPos().x() / width();
                    this->showNormal();
                    // update();
                    auto offsetX = width() * part;
                    setGeometry(event->screenPos().x() - offsetX, 0, width(), height());
                }*/
                _mousePressed = false;
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
                this->windowHandle()->startSystemMove();
#endif
            }
            break;
        }
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
        _lmbPos                  = None;
        _mousePressed            = false;
    }
    update();
    return QWidget::mouseReleaseEvent(event);
}

MainWidget::MainWidget(QWidget* parent) : QWidget(parent)
{
    Style::setDpiScale(logicalDpiX() * 100 / 96);
    Style::startManager(Style::getDpiScale());

    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setWindowFlag(Qt::Window);
    this->setWindowIcon(QIcon(":/images/logo.png"));
    this->setMinimumWidth(Style::valueDPIScale(800));

    auto* grid = new QGridLayout(this);

    _body = std::make_unique<QWidget>();
    _body->setMinimumHeight(Style::valueDPIScale(480));

    auto title = new QWidget();
    title->setFixedHeight(Style::valueDPIScale(30));
    grid->addWidget(_body.get(), 1, 0);
    grid->addWidget(title, 0, 0);
    grid->setSpacing(0);
    this->setLayout(grid);
    layout()->setMargin(9);
    _body->setStyleSheet(
        "QWidget {"
        "background-color: #323232;"
        "}");

    title->setStyleSheet(
        "QWidget { "
        "background-color: #424140; "
        "}");

    std::cout << QGuiApplication::platformName().toStdString() << std::endl;

    window()->createWinId();

    pTitleLayout = std::make_unique<QHBoxLayout>(title);
    title->setLayout(pTitleLayout.get());

    close_btn    = std::make_unique<CaptionButton>(title, QColor(232, 17, 35), st::closeButtonIcon);
    maximize_btn = std::make_unique<CaptionButton>(title, QColor(255, 255, 255, 76), st::maximizeButtonIcon);
    minimize_btn = std::make_unique<CaptionButton>(title, QColor(255, 255, 255, 76), st::minimizeButtonIcon);
    refreshTitleBar();

    connect(window()->windowHandle(), &QWindow::windowStateChanged, this, [=](Qt::WindowState state) {
        if (state == Qt::WindowMinimized) return;

        if (state == Qt::WindowNoState)
        {
            setAttribute(Qt::WA_TranslucentBackground);
            layout()->setMargin(9);
            maximize_btn->setIcon(nullptr);
        }
        else if (state == Qt::WindowMaximized)
        {
            setAttribute(Qt::WA_TranslucentBackground, false);
            layout()->setMargin(0);
            maximize_btn->setIcon(&st::restoreButtonIcon);
        }
    });

    maximize_btn->setClickCallback([=]() {
        if (this->isMaximized())
        {
            window()->setWindowState(Qt::WindowNoState);
        }
        else
        {
            window()->setWindowState(Qt::WindowMaximized);
        }
        _lmbPos = None;
        update();
    });
    close_btn->setClickCallback([=]() { deleteLater(); });
    minimize_btn->setClickCallback([=]() { showMinimized(); });
    title->setMouseTracking(true);
    this->setMouseTracking(true);
    title->installEventFilter(this);
    this->installEventFilter(this);
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

void MainWidget::refreshTitleBar(BioButton* bio_button)
{
    pTitleLayout->setSpacing(0);
    pTitleLayout->setMargin(0);
    pTitleLayout->setAlignment(Qt::AlignTop | Qt::AlignRight);
    if (!pTitleLayout->isEmpty())
    {
        pTitleLayout->removeWidget(minimize_btn.get());
        pTitleLayout->removeWidget(maximize_btn.get());
        pTitleLayout->removeWidget(close_btn.get());
    }
    if (bio_button)
    {
        pTitleLayout->addWidget(bio_button);
    }
    pTitleLayout->addWidget(minimize_btn.get());
    pTitleLayout->addWidget(maximize_btn.get());
    pTitleLayout->addWidget(close_btn.get());
}
bool MainWidget::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::HoverMove)
    {
        checkResizableField(static_cast<QMouseEvent*>(event));
    }
    return QObject::eventFilter(watched, event);
}
