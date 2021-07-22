#include "TitleWidget.hpp"

#include <QLayout>
#include <QPainter>
#include <QWindow>
#include <QtEvents>

#include "Widgets/BioButton.hpp"
#include "Widgets/CaptionButton.hpp"

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
