#pragma once

#include <QBrush>
#include <QColor>
#include <QPen>

namespace Style
{
class palette;
namespace internal
{
class Color;

class ColorData
{
public:
    QColor c;
    QPen   p;
    QBrush b;

    [[nodiscard]] QColor transparent() const { return QColor(c.red(), c.green(), c.blue(), 0); };

private:
    ColorData(uchar r, uchar g, uchar b, uchar a);
    ColorData(const ColorData& other) = default;
    ColorData& operator=(const ColorData& other) = default;

    void set(uchar r, uchar g, uchar b, uchar a);

    friend class Color;
    friend class Style::palette;
};

class Color
{
public:
    Color(Qt::Initialization = Qt::Uninitialized) {}
    Color(const Color& other) = default;
    Color& operator=(const Color& other) = default;

    void set(uchar r, uchar g, uchar b, uchar a) const { _data->set(r, g, b, a); }

    operator const QBrush&() const { return _data->b; }

    operator const QPen&() const { return _data->p; }

    ColorData* operator->() const { return _data; }
    ColorData* v() const { return _data; }

    explicit operator bool() const { return !!_data; }

private:
    friend class Style::palette;
    Color(ColorData* data) : _data(data) {}

    ColorData* _data = nullptr;
};
}  // namespace internal
}  // namespace Style