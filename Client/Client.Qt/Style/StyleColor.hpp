#pragma once

#include <QBrush>
#include <QColor>
#include <QPen>

namespace Style
{
class Palette;
namespace internal
{
class Color;

/**
 * @class ColorData
 * @brief Class for holding data about color
 */
class ColorData
{
public:
    /// Color itself
    QColor color;
    /// Pen with the color
    QPen   pen;
    /// Brush with the color
    QBrush brush;

    /// Transparent color
    [[nodiscard]] QColor transparent() const { return QColor(color.red(), color.green(), color.blue(), 0); };

private:
    ColorData(uchar r, uchar g, uchar b, uchar a);
    ColorData(const ColorData& other) = default;
    ColorData& operator=(const ColorData& other) = default;

    void set(uchar r, uchar g, uchar b, uchar a);

    friend class Color;
    friend class Style::Palette;
};

/**
 * @class Color
 * @brief Class for pointing to color data
 */
class Color
{
public:
    /// Empty initialization
    Color(Qt::Initialization = Qt::Uninitialized) {}
    /// Copy constructor
    Color(const Color& other) = default;
    /// Copy assigment
    Color& operator=(const Color& other) = default;

    /// Change a color
    void set(uchar r, uchar g, uchar b, uchar a) const { _data->set(r, g, b, a); }

    /// Get a brush with the color
    operator const QBrush&() const { return _data->brush; }

    /// Get a pen with the color
    operator const QPen&() const { return _data->pen; }

    /// Refer to storage
    ColorData* operator->() const { return _data; }
    /// Refer to storage
    ColorData* v() const { return _data; }

    /// Does data exist
    explicit operator bool() const { return !!_data; }

private:
    friend class Style::Palette;
    Color(ColorData* data) : _data(data) {}

    ColorData* _data = nullptr;
};
}  // namespace internal
}  // namespace Style
