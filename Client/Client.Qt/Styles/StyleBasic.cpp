// WARNING! All changes made in this file will be lost!
// This file was created automatically
#include "StyleBasic.hpp"

namespace
{
bool inited = false;

class Module_StyleBasic : public Style::internal::ModuleBase
{
public:
    Module_StyleBasic() { Style::internal::registerModule(this); }

    void start(int scale) override { Style::internal::init_StyleBasic(scale); }
};
Module_StyleBasic registrator;

int            _fsize         = 0;
Style::font    _defaultFont   = {Qt::Uninitialized};
Style::font    _semiboldFont  = {Qt::Uninitialized};
int            _defaultMargin = 0;
Style::margins _mar           = {0, 0, 0, 0};
Style::font    _TextStyle     = {Qt::Uninitialized};
Style::icon    _smileIcon     = {Qt::Uninitialized};
Style::icon    _dislikeIcon   = {Qt::Uninitialized};
Style::icon    _likeIcon      = {Qt::Uninitialized};
Style::icon    _fireIcon      = {Qt::Uninitialized};
Style::icon    _catIcon       = {Qt::Uninitialized};

}  // namespace

namespace st
{
const int&            fsize(_fsize);
const Style::font&    defaultFont(_defaultFont);
const Style::font&    semiboldFont(_semiboldFont);
const int&            defaultMargin(_defaultMargin);
const Style::margins& mar(_mar);
const Style::font&    TextStyle(_TextStyle);
const Style::icon&    smileIcon(_smileIcon);
const Style::icon&    dislikeIcon(_dislikeIcon);
const Style::icon&    likeIcon(_likeIcon);
const Style::icon&    fireIcon(_fireIcon);
const Style::icon&    catIcon(_catIcon);
}  // namespace st

namespace Style
{
namespace internal
{
namespace
{
int  px8  = 8;
int  px14 = 14;
int  px15 = 15;
void initPxValues(int scale)
{
    px8  = ConvertScale(8, scale);
    px14 = ConvertScale(14, scale);
    px15 = ConvertScale(15, scale);
}

}  // namespace

void init_StyleBasic(int scale)
{
    if (inited) return;
    inited = true;

    initPxValues(scale);

    _fsize         = px15;
    _defaultFont   = {px15, 0, 0};
    _semiboldFont  = {px15, 16, 0};
    _defaultMargin = px8;
    _mar           = {px8, px8, px8, px8};
    _TextStyle     = {px14, 1, 0};
    _smileIcon     = {":reactions/smile.png", 1};
    _dislikeIcon   = {":reactions/dislike.png", 1};
    _likeIcon      = {":reactions/like.png", 1};
    _fireIcon      = {":reactions/fire.png", 1};
    _catIcon       = {":reactions/cat.png", 1};
}

}  // namespace internal
}  // namespace Style
