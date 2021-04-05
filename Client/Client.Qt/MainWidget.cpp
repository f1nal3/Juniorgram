#include "MainWidget.h"

#include <Widgets/BioButton.h>

#include "Style/Shadow.hpp"
#include "Style/Style.h"
#include "Widgets/InputFields.h"

#ifdef _WIN32
bool IsCompositionEnabled()
{
    auto result        = BOOL(FALSE);
    const auto success = (::DwmIsCompositionEnabled(&result) == S_OK);
    return success && result;
}

bool MainWidget::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
    Q_UNUSED(eventType);
    MSG* msg = static_cast<MSG*>(message);

    if (msg->message == WM_NCACTIVATE)
    {
        if (IsCompositionEnabled())
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
            const auto monitor = ::MonitorFromWindow(msg->hwnd, MONITOR_DEFAULTTONULL);
            if (!monitor) return false;
            MONITORINFO monitor_info{};
            monitor_info.cbSize = sizeof(MONITORINFO);
            if (!::GetMonitorInfoW(monitor, &monitor_info)) return false;
            params.rgrc[0] = monitor_info.rcWork;
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
        const auto p = MAKEPOINTS(msg->lParam);
        if (p.x <= 9)
        {
            *result = HTLEFT;
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
            this->layout()->setMargin(0);
            this->setAttribute(Qt::WA_TranslucentBackground, false);
        }
        else if (msg->wParam == SIZE_MINIMIZED)
        {
            state = Qt::WindowMinimized;
            this->setAttribute(Qt::WA_TranslucentBackground, false);
        }
        else
        {
            state = Qt::WindowNoState;
            this->layout()->setMargin(9);
            this->setAttribute(Qt::WA_TranslucentBackground);
        }
        emit (window())->windowHandle()->windowStateChanged(state);
    }
    return false;
}
#endif

MainWidget::MouseType MainWidget::checkResizableField(QMouseEvent* event)
{
    QPointF position = event->screenPos();
    qreal x          = this->x();
    qreal y          = this->y();
    qreal width      = this->width();
    qreal height     = this->height();

    QRectF rectTop(x, y, width + 9, 8);
    QRectF rectBottom(x, y + height - 8, width + 9, 8);
    QRectF rectLeft(x, y, 8, height + 9);
    QRectF rectRight(x + width - 8, y, 8, height + 9);
    QRectF rectInterface(x + 9, y + 9, width - 18 - Style::valueDPIScale(46) * 3,
                         Style::valueDPIScale(30));

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
    else if (rectInterface.contains(position))
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
    if (event->button() == Qt::LeftButton)
    {
        _mousePressed            = true;
        m_leftMouseButtonPressed = checkResizableField(event);
        if (m_leftMouseButtonPressed == Top)
        {
            this->windowHandle()->startSystemResize(Qt::TopEdge);
        }
        else if (m_leftMouseButtonPressed == TopLeft)
        {
            this->windowHandle()->startSystemResize(Qt::TopEdge | Qt::LeftEdge);
        }
        else if (m_leftMouseButtonPressed == TopRight)
        {
            this->windowHandle()->startSystemResize(Qt::TopEdge | Qt::RightEdge);
        }
        else if (m_leftMouseButtonPressed == Bottom)
        {
            this->windowHandle()->startSystemResize(Qt::BottomEdge);
        }
        else if (m_leftMouseButtonPressed == BottomLeft)
        {
            this->windowHandle()->startSystemResize(Qt::BottomEdge | Qt::LeftEdge);
        }
        else if (m_leftMouseButtonPressed == BottomRight)
        {
            this->windowHandle()->startSystemResize(Qt::BottomEdge | Qt::RightEdge);
        }
        else if (m_leftMouseButtonPressed == Left)
        {
            this->windowHandle()->startSystemResize(Qt::LeftEdge);
        }
        else if (m_leftMouseButtonPressed == Right)
        {
            this->windowHandle()->startSystemResize(Qt::RightEdge);
        }
    }

    return QWidget::mousePressEvent(event);
}

void MainWidget::mouseMoveEvent(QMouseEvent* event)
{
    checkResizableField(event);
    switch (m_leftMouseButtonPressed)
    {
        case Move:
        {
            if (_mousePressed)
            {
                if (isMaximized())
                {
                    this->layout()->setMargin(9);
                    auto part = event->screenPos().x() / width();
                    this->showNormal();
                    // update();
                    QCoreApplication::processEvents(
                        QEventLoop::ProcessEventsFlag::ExcludeUserInputEvents);
                    auto offsetX = width() * part;
                    setGeometry(event->screenPos().x() - offsetX, 0, width(), height());
                }

                _mousePressed = false;
                this->windowHandle()->startSystemMove();
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
        m_leftMouseButtonPressed = None;
        _mousePressed            = false;
    }
    update();
    return QWidget::mouseReleaseEvent(event);
}

MainWidget::MainWidget(QWidget* parent) : QWidget(parent)
{
    Style::setDpiScale(logicalDpiX() * 100 / 96);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setWindowFlag(Qt::Window);
    this->setWindowIcon(QIcon(":/images/logo.png"));
    this->setStyleSheet(
        "QWidget { "
        "background-color: #424140; "
        "}");
    this->setMinimumSize(Style::valueDPIScale(630), 450);

    auto* grid = new QGridLayout(this);
    body       = new QWidget();
    auto title = new QWidget();
    title->setFixedHeight(Style::valueDPIScale(30));
    grid->addWidget(body, 1, 0);
    grid->addWidget(title, 0, 0);
    grid->setSpacing(0);
    this->setLayout(grid);
    layout()->setMargin(9);
    auto pBodyLayout = new QVBoxLayout(body);
    body->setLayout(pBodyLayout);
    pBodyLayout->setSpacing(0);
    pBodyLayout->setMargin(0);
    pBodyLayout->setContentsMargins(0, 0, 0, 0);
    body->setStyleSheet(
        "QWidget {"
        "background-color: #323232;"
        "}");

    pBodyLayout->setMargin(5);

    std::cout << QGuiApplication::platformName().toStdString() << std::endl;

    pTitleLayout = new QHBoxLayout(title);
    title->setLayout(pTitleLayout);
    close_btn    = new CaptionButton(CaptionButton::CaptionLogo::Close, QColor(232, 17, 35));
    maximize_btn = new CaptionButton(CaptionButton::CaptionLogo::Maximize);
    minimize_btn = new CaptionButton(CaptionButton::CaptionLogo::Minimize);
    refreshTitleBar();

    connect(maximize_btn, &CaptionButton::mouseRelease, [this]() {
        if (this->isMaximized())
        {
            this->layout()->setMargin(0);
            this->setAttribute(Qt::WA_TranslucentBackground);
            window()->setWindowState(Qt::WindowNoState);
            this->m_leftMouseButtonPressed = None;
        }
        else
        {
            this->layout()->setMargin(9);
            this->setAttribute(Qt::WA_TranslucentBackground, false);
            window()->setWindowState(Qt::WindowMaximized);
            this->m_leftMouseButtonPressed = None;
        }
        update();
    });
    connect(close_btn, &CaptionButton::mouseRelease, this, &MainWidget::close);
    connect(minimize_btn, &CaptionButton::mouseRelease, this, &MainWidget::showMinimized);
    title->setMouseTracking(true);
    body->setMouseTracking(true);
    this->setMouseTracking(true);
}

void MainWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.setPen(Qt::NoPen);
    // Shadow is ugly on corners
    drawShadow(p, 10, 2.0, QColor(0, 0, 0, 0x18), QColor(0, 0, 0, 0), 0.0, 1.0, 0.6, width(),
               height());
}

void MainWidget::setCentralWidget(QWidget* widget)
{
    static QWidget* current = nullptr;
    delete current;

    current = widget;
    body->layout()->addWidget(current);
}
void MainWidget::refreshTitleBar(BioButton* bio_button)
{
    pTitleLayout->setSpacing(0);
    pTitleLayout->setMargin(0);
    pTitleLayout->setAlignment(Qt::AlignTop | Qt::AlignRight);
    if (!pTitleLayout->isEmpty())
    {
        pTitleLayout->removeWidget(minimize_btn);
        pTitleLayout->removeWidget(maximize_btn);
        pTitleLayout->removeWidget(close_btn);
    }
    if (bio_button)
    {
        pTitleLayout->addWidget(bio_button);
    }
    pTitleLayout->addWidget(minimize_btn);
    pTitleLayout->addWidget(maximize_btn);
    pTitleLayout->addWidget(close_btn);
}
bool MainWidget::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::FocusOut)
    {
        update();
        QApplication::processEvents();
    }
    return QObject::eventFilter(watched, event);
}
void MainWidget::showEvent(QShowEvent* event) { QWidget::showEvent(event); update(); }
