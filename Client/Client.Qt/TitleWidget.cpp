#include "TitleWidget.hpp"

#include <QLayout>
#include <QPainter>
#include <QWindow>
#include <QtEvents>

#include "Widgets/CaptionButton.hpp"

TitleWidget::TitleWidget(QWidget* parent) : QWidget(parent)
{
    _closeButton = std::make_unique<CaptionButton>(this, QColor(232, 17, 35), st::closeButtonIcon);
    _maximizeButton = std::make_unique<CaptionButton>(this, QColor(255, 255, 255, 76), st::maximizeButtonIcon);
    _minimizeButton = std::make_unique<CaptionButton>(this, QColor(255, 255, 255, 76), st::minimizeButtonIcon);

    setFixedHeight(Style::valueDPIScale(30) / Style::devicePixelRatio());

    connect(window()->windowHandle(), &QWindow::windowStateChanged, this, [=](Qt::WindowState state) {
        if (state == Qt::WindowMinimized) return;

        if (state == Qt::WindowNoState)
        {
            parent->setAttribute(Qt::WA_TranslucentBackground);
            parent->layout()->setMargin(9);
            _maximizeButton->setIcon(nullptr);
        }
        else if (state == Qt::WindowMaximized)
        {
            parent->setAttribute(Qt::WA_TranslucentBackground, false);
            parent->layout()->setMargin(0);
            _maximizeButton->setIcon(&st::restoreButtonIcon);
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
        update();
    });
    _closeButton->setClickCallback([=]() { parent->deleteLater(); });
    _minimizeButton->setClickCallback([=]() { parent->showMinimized(); });
}

void TitleWidget::paintEvent(QPaintEvent* paintEvent)
{
    QPainter painter(this);
    painter.fillRect(rect(), QColor(0x40, 0x41, 0x42));
    QWidget::paintEvent(paintEvent);
}
void TitleWidget::resizeEvent(QResizeEvent* resizeEvent)
{
    const int buttonWidth = _closeButton->width();
    _closeButton->move(width() - buttonWidth, 0);
    _maximizeButton->move(width() - buttonWidth * 2, 0);
    _minimizeButton->move(width() - buttonWidth * 3, 0);
    QWidget::resizeEvent(resizeEvent);
}

void TitleWidget::mousePressEvent(QMouseEvent* mouseEvent)
{
    parentWidget()->window()->windowHandle()->startSystemMove();
    QWidget::mousePressEvent(mouseEvent);
}
