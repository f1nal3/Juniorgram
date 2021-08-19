// WARNING! All changes made in this file will be lost!
// This file was created automatically
#include "StyleIcons.hpp"

namespace
{
bool inited = false;

class Module_StyleIcons : public Style::internal::ModuleBase
{
public:
    Module_StyleIcons() { Style::internal::registerModule(this); }

    void start(int scale) override { Style::internal::init_StyleIcons(scale); }
};
Module_StyleIcons registrator;

Style::icon _smileIcon   = {Qt::Uninitialized};
Style::icon _dislikeIcon = {Qt::Uninitialized};
Style::icon _likeIcon    = {Qt::Uninitialized};
Style::icon _fireIcon    = {Qt::Uninitialized};
Style::icon _catIcon     = {Qt::Uninitialized};

}  // namespace

namespace st
{
const Style::icon& smileIcon(_smileIcon);
const Style::icon& dislikeIcon(_dislikeIcon);
const Style::icon& likeIcon(_likeIcon);
const Style::icon& fireIcon(_fireIcon);
const Style::icon& catIcon(_catIcon);
}  // namespace st

namespace Style
{
namespace internal
{
namespace
{
}  // namespace

void init_StyleIcons(int)
{
    if (inited) return;
    inited = true;

    _smileIcon   = {":reactions/smile.png", 1};
    _dislikeIcon = {":reactions/dislike.png", 1};
    _likeIcon    = {":reactions/like.png", 1};
    _fireIcon    = {":reactions/fire.png", 1};
    _catIcon     = {":reactions/cat.png", 1};
}

}  // namespace internal
}  // namespace Style
