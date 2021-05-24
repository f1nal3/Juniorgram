// WARNING! All changes made in this file will be lost!
// This file was created automatically
#include "palette.h"

namespace
{
bool inited = false;

class Module_palette : public Style::internal::ModuleBase
{
public:
    Module_palette() { Style::internal::registerModule(this); }

    void start(int scale) override { Style::internal::init_palette(scale); }
};
Module_palette registrator;

Style::palette _palette;

}  // namespace

namespace st
{
const Style::color& transparent(_palette.get_transparent());  // special color
const Style::color& windowActiveTextFg(_palette.get_windowActiveTextFg());
const Style::color& window(_palette.get_window());
const Style::color& windowColor(_palette.get_windowColor());
const Style::color& windowColorOver(_palette.get_windowColorOver());
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
    compute(3, -1, {66, 67, 68, 255});
    compute(4, -1, {82, 83, 84, 255});
}

namespace internal
{
void init_palette(int /*scale*/)
{
    if (inited) return;
    inited = true;

    _palette.finalize();
}

}  // namespace internal
}  // namespace Style
