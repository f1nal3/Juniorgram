#include "StyleColor.hpp"

namespace Style
{
namespace internal
{
ColorData::ColorData(uchar r, uchar g, uchar b, uchar a)
    : color(int(r), int(g), int(b), int(a)), pen(color), brush(color)
{
}

void ColorData::set(uchar r, uchar g, uchar b, uchar a)
{
    this->color = QColor(int(r), int(g), int(b), int(a));
    this->pen   = QPen(color);
    this->brush = QBrush(color);
}
}  // namespace internal
}  // namespace Style
