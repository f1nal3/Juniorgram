#include "Style_color.hpp"

namespace Style
{
namespace internal
{
ColorData::ColorData(uchar r, uchar g, uchar b, uchar a)
    : c(int(r), int(g), int(b), int(a)), p(c), b(c)
{
}

void ColorData::set(uchar r, uchar g, uchar b, uchar a)
{
    this->c = QColor(int(r), int(g), int(b), int(a));
    this->p = QPen(c);
    this->b = QBrush(c);
}
}  // namespace internal
}  // namespace Style
