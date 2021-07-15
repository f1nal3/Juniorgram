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
    QColor color;
    QPen   pen;
    QBrush brush;

    [[nodiscard]] QColor transparent() const
    {
        return QColor(color.red(), color.green(), color.blue(), 0);
    };

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

    operator const QBrush&() const { return _data->brush; }

    operator const QPen&() const { return _data->pen; }

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
