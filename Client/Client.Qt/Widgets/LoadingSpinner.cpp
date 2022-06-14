
#include "LoadingSpinner.hpp"

#include <QDebug>
#include <QPainter>
#include <QTimer>
#include <QtCore>
#include <algorithm>
#include <asio/connect.hpp>
#include <cmath>

LoadingSpinner::LoadingSpinner(QWidget* parent, bool disableParentWhenSpinning, const Style::LoadingSpinner& st)
    : QWidget(parent), _st(st), _disableParentWhenSpinning(disableParentWhenSpinning)
{
    _timer = std::make_unique<QTimer>(this);
    connect(_timer.get(), SIGNAL(timeout()), this, SLOT(rotate()));
    _timer->setInterval(static_cast<int>(1000.0 / (_st.numberOfLines * _st.revolutionsPerSecond)));
    updateSize();
    hide();
}

void LoadingSpinner::paintEvent(QPaintEvent*)
{
    raise();
    updatePosition();
    QPainter painter(this);
    painter.fillRect(rect(), QColor(0, 0, 0, 128));
    painter.setRenderHint(QPainter::Antialiasing, true);

    if (_currentCounter >= _st.numberOfLines)
    {
        _currentCounter = 0;
    }

    painter.setPen(Qt::NoPen);
    for (int i = 0; i < _st.numberOfLines; ++i)
    {
        painter.save();
        /// Translate to center of parent
        painter.translate(width() / 2, height() / 2);
        /// Calculate angle
        double rotateAngle = static_cast<double>(360 * i) / static_cast<double>(_st.numberOfLines);
        /// Rotate, so "right" will face from center in angle
        painter.rotate(rotateAngle);
        /// Move from center
        painter.translate(_st.innerRadius, 0);
        /// Distance to highlighted line
        int    distance = (_currentCounter - i + _st.numberOfLines) % _st.numberOfLines;
        QColor color    = currentLineColor(distance, _st.numberOfLines, _st.trailFadePercentage, _st.lineColor->color);
        painter.setBrush(color);
        painter.drawRect(QRect(0, -_st.lineWidth / 2, _st.lineLength, _st.lineWidth));
        painter.restore();
    }
}

void LoadingSpinner::start()
{
    updateSize();
    updatePosition();
    _isSpinning = true;
    show();

    if (parentWidget() && _disableParentWhenSpinning)
    {
        parentWidget()->setEnabled(false);
    }

    if (!_timer->isActive())
    {
        _timer->start();
        _currentCounter = 0;
    }
}

void LoadingSpinner::stop()
{
    _isSpinning = false;
    hide();

    if (parentWidget() && _disableParentWhenSpinning)
    {
        parentWidget()->setEnabled(true);
    }

    if (_timer->isActive())
    {
        _timer->stop();
        _currentCounter = 0;
    }
}

bool LoadingSpinner::isSpinning() const { return _isSpinning; }

void LoadingSpinner::rotate()
{
    ++_currentCounter;
    if (_currentCounter >= _st.numberOfLines)
    {
        _currentCounter = 0;
    }
    update();
}

void LoadingSpinner::updateSize() { resize(parentWidget()->size()); }

void LoadingSpinner::updatePosition()
{
    if (parentWidget())
    {
        move(parentWidget()->width() / 2 - width() / 2, parentWidget()->height() / 2 - height() / 2);
    }
}

QColor LoadingSpinner::currentLineColor(int countDistance, int numOfLines, double trailFadePerc, QColor color) const
{
    if (countDistance == 0)
    {
        return color;
    }

    if (auto distanceThreshold = static_cast<int>(ceil((numOfLines - 1) * trailFadePerc)); distanceThreshold < countDistance)
    {
        color.setAlphaF(_st.minimumTrailOpacity);
    }
    else
    {
        double alphaDiff   = color.alphaF() - _st.minimumTrailOpacity;
        double gradient    = alphaDiff / static_cast<double>(distanceThreshold + 1);
        double resultAlpha = color.alphaF() - gradient * countDistance;

        resultAlpha = std::min(1.0, std::max(0.0, resultAlpha));
        color.setAlphaF(resultAlpha);
    }
    return color;
}