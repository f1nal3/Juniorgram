// WARNING! All changes made in this file will be lost!
// This file was created automatically
#include "Palette.hpp"

namespace
{
bool inited = false;

class Module_Palette : public Style::internal::ModuleBase
{
public:
    Module_Palette() { Style::internal::registerModule(this); }

    void start(int scale) override { Style::internal::init_Palette(scale); }
};
Module_Palette registrator;

Style::palette _palette;

}  // namespace

namespace st
{
const Style::color& transparent(_palette.get_transparent());  // special color
const Style::color& windowActiveTextFg(_palette.get_windowActiveTextFg());
const Style::color& window(_palette.get_window());
const Style::color& windowColor(_palette.get_windowColor());
const Style::color& windowColorOver(_palette.get_windowColorOver());
const Style::color& titleButton(_palette.get_titleButton());
const Style::color& titleButtonOver(_palette.get_titleButtonOver());
const Style::color& closeButtonC(_palette.get_closeButtonC());
const Style::color& closeButtonCOver(_palette.get_closeButtonCOver());
const Style::color& linkButtonColor(_palette.get_linkButtonColor());
const Style::color& linkButtonOverColor(_palette.get_linkButtonOverColor());
const Style::color& scrollArea(_palette.get_scrollArea());
const Style::color& scrollAreaColor(_palette.get_scrollAreaColor());
const Style::color& scrollAreaBar(_palette.get_scrollAreaBar());
const Style::color& scrollAreaColorOver(_palette.get_scrollAreaColorOver());
const Style::color& scrollAreaBarOver(_palette.get_scrollAreaBarOver());
}  // namespace st

namespace Style
{
int palette::indexOfColor(Style::color c) const
{
    auto start = data(0);
    if (c._data >= start && c._data < start + kCount)
    {
        return static_cast<int>(c._data - start);
    }
    return -1;
}

color palette::colorAtIndex(int index) const
{
    assert(_ready);
    assert(index >= 0 && index < kCount);
    return _colors[index];
}

void palette::finalize()
{
    if (_ready) return;
    _ready = true;

    compute(0, -1, {255, 255, 255, 0});  // special color
    compute(1, -1, {255, 255, 255, 255});
    compute(2, -1, {50, 50, 50, 255});
    compute(3, -1, {64, 65, 66, 255});
    compute(4, -1, {82, 83, 84, 255});
    compute(5, -1, {255, 255, 255, 0});
    compute(6, -1, {255, 255, 255, 76});
    compute(7, -1, {232, 17, 35, 0});
    compute(8, -1, {232, 17, 35, 76});
    compute(9, -1, {0, 0, 255, 255});
    compute(10, -1, {0, 255, 0, 255});
    compute(11, 2, {50, 50, 50, 255});
    compute(12, 3, {64, 65, 66, 255});
    compute(13, 4, {82, 83, 84, 255});
    compute(14, -1, {71, 72, 73, 255});
    compute(15, -1, {89, 90, 91, 255});
}
namespace internal
{
void init_Palette(int)
{
    if (inited) return;
    inited = true;

    _palette.finalize();
}

}  // namespace internal
}  // namespace Style
