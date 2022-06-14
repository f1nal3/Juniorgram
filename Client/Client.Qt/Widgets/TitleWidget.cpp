#include "TitleWidget.hpp"

#include <QDebug>
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
    setFixedHeight(st::titleBarHeight);

    _maximizedState = !(window()->windowState() & Qt::WindowMaximized);

    connect(window()->windowHandle(), &QWindow::windowStateChanged, this, [=](Qt::WindowState state) { windowStateChanged(state); });

    _maximizeButton->setClickCallback([parent, this]() {
        if (parent->isMaximized())
            this->window()->setWindowState(Qt::WindowNoState);
        else
            this->window()->setWindowState(Qt::WindowMaximized);
        this->_maximizeButton->clearState();
    });
    _closeButton->setClickCallback([parent, this]() {
        parent->deleteLater();
        this->_closeButton->clearState();
    });
    _minimizeButton->setClickCallback([parent, this]() {
        parent->showMinimized();
        this->_minimizeButton->clearState();
    });
    setMinimumWidth(_minimizeButton->width() + _maximizeButton->width() + _closeButton->width() + _bioButton->width());
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

void TitleWidget::showBioButton(bool show) const
{
    if (show)
        _bioButton->show();
    else
        _bioButton->hide();
}

bool TitleWidget::setBioButtonIcon(const Style::icon* icon) const { return _bioButton->setIcon(icon); }

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
            parentWidget()->layout()->setMargin(st::shadowPadding);
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

    const Style::icon& icon = _iconOverride ? *_iconOverride : _st->icon;

    const int horside = (width() - st::titleBarMaxIconWidth) / 2;
    const int verside = (height() - st::titleBarMaxIconWidth) / 2;

    if (icon.size().width() <= st::titleBarMaxIconWidth)
    {
        painter.drawPixmap((width() - icon->pixmap()->width()) / 2, (height() - icon->pixmap()->height()) / 2, *icon->pixmap());
    }
    else
    {
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        painter.drawPixmap(QRect(horside, verside, st::titleBarMaxIconWidth, st::titleBarMaxIconWidth), *icon->pixmap());
    }
}

void CaptionButton::setStyle(const Style::TitleBarButton* newSt)
{
    _st = newSt;
    updateWidget();
}

bool CaptionButton::setIcon(const Style::icon* icon)
{
    if ((icon != nullptr) && (icon->size().width() != icon->size().height()))
    {
        return false;
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
    _dropDownMenu = std::make_unique<DropDownMenu>(this);
    _settingsWidget = std::make_unique<SettingsWidget>();

    setClickCallback([this]() {
        auto globalPoint = mapToGlobal(QPoint(0, height()));

        // Creating menu
        auto menu = std::make_unique<Menu>();

        // Adding options
        menu->addAction("Username: Add format here", []() {});
        menu->addSeparator();
        menu->addAction("Settings", [this](){ _settingsWidget->show(); });
        menu->addSeparator();
        menu->addAction("Quit", []() { oApp->setAppState(App::AppState::LoginForm); });

        _dropDownMenu->setMenu(std::move(menu));

        // Displacement in pixels of the dropdown menu relative to the top-left corner of the BioButton
        int displacement = 1;
        // Now show the menu
        _dropDownMenu->showDropDownMenu(QPoint(globalPoint.x(), globalPoint.y() + displacement));
    });
}
