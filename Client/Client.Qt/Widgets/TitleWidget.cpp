#include "TitleWidget.hpp"

#include <QLayout>
#include <QPainter>
#include <QWindow>
#include <QtEvents>

#include "Application.hpp"

TitleWidget::TitleWidget(QWidget* parent, const Style::TitleBar& st) : QWidget(parent), _st(st)
{
    _closeButton    = std::make_unique<CaptionButton>(this, &_st.closeButton);
    _maximizeButton = std::make_unique<CaptionButton>(this, &_st.maximizeButton);
    _minimizeButton = std::make_unique<CaptionButton>(this, &_st.minimizeButton);
    _bioButton      = std::make_unique<BioButton>(this);

    setAttribute(Qt::WA_OpaquePaintEvent);
    setFixedHeight(Style::valueDPIScale(30));

    _maximizedState = !(window()->windowState() & Qt::WindowMaximized);

    connect(window()->windowHandle(), &QWindow::windowStateChanged, this, [=](Qt::WindowState state) { windowStateChanged(state); });

    _maximizeButton->setClickCallback([=]() {
        if (parent->isMaximized())
            window()->setWindowState(Qt::WindowNoState);
        else
            window()->setWindowState(Qt::WindowMaximized);
        _maximizeButton->clearState();
    });
    _closeButton->setClickCallback([=]() {
        parent->deleteLater();
        _closeButton->clearState();
    });
    _minimizeButton->setClickCallback([=]() {
        parent->showMinimized();
        _minimizeButton->clearState();
    });
}

void TitleWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.fillRect(rect(), _st.bgColor);
}
void TitleWidget::resizeEvent(QResizeEvent*)
{
    const int buttonWidth = _closeButton->width();
    _closeButton->move(width() - buttonWidth, 0);
    _maximizeButton->move(width() - buttonWidth * 2, 0);
    _minimizeButton->move(width() - buttonWidth * 3, 0);
    _bioButton->move(width() - buttonWidth * 4, 0);
}

void TitleWidget::mousePressEvent(QMouseEvent*) { parentWidget()->window()->windowHandle()->startSystemMove(); }

void TitleWidget::showBioButton(bool show)
{
    if (show)
        _bioButton->show();
    else
        _bioButton->hide();
}

bool TitleWidget::setBioButtonIcon(const Style::icon* icon) { return _bioButton->setIcon(icon); }

void TitleWidget::mouseDoubleClickEvent(QMouseEvent*)
{
    if (_maximizedState)
    {
        window()->setWindowState(Qt::WindowNoState);
    }
    else
    {
        window()->setWindowState(Qt::WindowMaximized);
    }
}
void TitleWidget::windowStateChanged(Qt::WindowState state)
{
    if (state == Qt::WindowMinimized) return;

    auto maximized = (window()->windowState() & Qt::WindowMaximized) == Qt::WindowMaximized;

    if (maximized != _maximizedState)
    {
        _maximizedState = maximized;
        if (state == Qt::WindowNoState)
        {
            parentWidget()->setAttribute(Qt::WA_TranslucentBackground);
            parentWidget()->layout()->setMargin(9);
            _maximizeButton->setStyle(&_st.maximizeButton);
        }
        else if (state == Qt::WindowMaximized)
        {
            parentWidget()->setAttribute(Qt::WA_TranslucentBackground, false);
            parentWidget()->layout()->setMargin(0);
            _maximizeButton->setStyle(&_st.restoreButton);
        }
    }
}

void CaptionButton::enterEvent(QEvent* event)
{
    _fadeinAnim->setDirection(QAbstractAnimation::Forward);
    _fadeinAnim->start();
    return AbstractButton::enterEvent(event);
}

void CaptionButton::leaveEvent(QEvent* event)
{
    _fadeinAnim->setDirection(QAbstractAnimation::Backward);
    _fadeinAnim->start();
    return AbstractButton::leaveEvent(event);
}

CaptionButton::CaptionButton(QWidget* parent, const Style::TitleBarButton* st) : AbstractButton(parent), _st(st)
{
    setMouseTracking(true);
    _fadeinAnim = std::make_unique<QPropertyAnimation>(this, "_hoverColor");
    updateWidget();
}

void CaptionButton::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    painter.setPen(Qt::NoPen);
    painter.fillRect(0, 0, width(), height(), _hoverColor);

    const int          maxSide = Style::valueDPIScale(20);
    const Style::icon& icon    = _iconOverride ? *_iconOverride : _st->icon;

    const int horside = (width() - maxSide) / 2;
    const int verside = (height() - maxSide) / 2;

    if (icon.size().width() <= maxSide)
    {
        painter.drawPixmap((width() - icon->pixmap()->width()) / 2, (height() - icon->pixmap()->height()) / 2, *icon->pixmap());
    }
    else
    {
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        painter.drawPixmap(QRect(horside, verside, maxSide, maxSide), *icon->pixmap());
    }
}

void CaptionButton::setStyle(const Style::TitleBarButton* newSt)
{
    _st = newSt;
    updateWidget();
}

bool CaptionButton::setIcon(const Style::icon* icon)
{
    if (icon != nullptr)
    {
        if (icon->size().width() != icon->size().height()) return false;
    }
    _iconOverride = icon;
    return true;
}

void CaptionButton::updateWidget()
{
    resize(_st->width, _st->height);

    _fadeinAnim->setDuration(150);
    _fadeinAnim->setEasingCurve(QEasingCurve::InCubic);
    _fadeinAnim->setStartValue(_st->bgColor->color);
    _fadeinAnim->setEndValue(_st->overBgColor->color);
    _hoverColor = _fadeinAnim->startValue().value<QColor>();
    update();
}

BioButton::BioButton(QWidget* parent, bool) : CaptionButton(parent)
{
    _popup = std::make_unique<PopupWidget>(this);

    setClickCallback([=]() {
        auto globalPoint = mapToGlobal(QPoint(0, height()));

        // Creating menu
        auto menu = std::make_unique<Menu>();

        // Adding options
        menu->addAction("Username: Add format here WWWWWWWWWWWWWWWWWWWWWWWWWWW", []() {});
        menu->addSeparator();
        menu->addAction("Quit", []() { oApp->setAppState(App::AppState::LoginForm); });
        menu->addAction("Settings", [](){ oApp->setAppState(App::AppState::SettingsWidgetForm); });

        _popup->setMenu(std::move(menu));

        // Now show the menu
        _popup->popup(QPoint(globalPoint.x(), globalPoint.y() + 1));
    });
}
