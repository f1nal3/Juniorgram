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

    setFixedHeight(Style::valueDPIScale(30));

    connect(window()->windowHandle(), &QWindow::windowStateChanged, this, [=](Qt::WindowState state) {
        if (state == Qt::WindowMinimized) return;

        if (state == Qt::WindowNoState)
        {
            parent->setAttribute(Qt::WA_TranslucentBackground);
            parent->layout()->setMargin(9);
            _maximizeButton->setStyle(&_st.maximizeButton);
        }
        else if (state == Qt::WindowMaximized)
        {
            parent->setAttribute(Qt::WA_TranslucentBackground, false);
            parent->layout()->setMargin(0);
            _maximizeButton->setStyle(&_st.restoreButton);
        }
    });

    _maximizeButton->setClickCallback([=]() {
        if (parent->isMaximized())
        {
            window()->setWindowState(Qt::WindowNoState);
        }
        else
        {
            window()->setWindowState(Qt::WindowMaximized);
        }
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

void TitleWidget::paintEvent(QPaintEvent* paintEvent)
{
    QPainter painter(this);
    painter.fillRect(rect(), _st.bgColor);
    QWidget::paintEvent(paintEvent);
}
void TitleWidget::resizeEvent(QResizeEvent* resizeEvent)
{
    const int buttonWidth = _closeButton->width();
    _closeButton->move(width() - buttonWidth, 0);
    _maximizeButton->move(width() - buttonWidth * 2, 0);
    _minimizeButton->move(width() - buttonWidth * 3, 0);
    _bioButton->move(width() - buttonWidth * 4, 0);
    QWidget::resizeEvent(resizeEvent);
}

void TitleWidget::mousePressEvent(QMouseEvent* mouseEvent)
{
    parentWidget()->window()->windowHandle()->startSystemMove();
    QWidget::mousePressEvent(mouseEvent);
}

void TitleWidget::showBioButton(bool show)
{
    if (show)
        _bioButton->show();
    else
        _bioButton->hide();
}

bool TitleWidget::setBioButtonIcon(const Style::icon* icon) { return _bioButton->setIcon(icon); }

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

void CaptionButton::paintEvent(QPaintEvent* event)
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
    QWidget::paintEvent(event);
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
    _popup->setFocusPolicy(Qt::NoFocus);
    _popup->setFocusProxy(this);

    setClickCallback([=]() {
        // Creating popup and setting
        /*
         * We need to know where to place popup menu on screen
         * If we just use mapToGlobal(localPoint) we'll get shift on X
         * like globalPoint.x = screenPosition.x + localPoint.x
         * so we need localPoint and globalPoint
         * localPoint - position on window
         * globalPoint - position on screen(with shift on X)
         */
        auto localPoint  = geometry().bottomLeft();
        auto globalPoint = mapToGlobal(localPoint);

        // Creating menu
        Menu* menu = new Menu;

        // Adding options
        menu->addAction("Username: Add format here", []() {});
        menu->addAction("Quit", []() { oApp->setAppState(App::AppState::LoginForm); });

        _popup->setMenu(menu);

        // Now show the menu
        _popup->popup(QPoint(globalPoint.x() - localPoint.x(), globalPoint.y() + 1));
    });
}