// WARNING! All changes made in this file will be lost!
// This file was created automatically
#pragma once

#include "Style/Style.hpp"

namespace Style
{
namespace internal
{
/// StyleBasic initializer
void init_StyleBasic(int scale);

}  // namespace internal
}  // namespace Style

namespace st
{
/// fsize: pixels
extern const int& fsize;
/// defaultFont: Style::font
extern const Style::font& defaultFont;
/// semiboldFont: Style::font
extern const Style::font& semiboldFont;
/// defaultMargin: pixels
extern const int& defaultMargin;
/// mar: Style::margins
extern const Style::margins& mar;
/// TextStyle: Style::font
extern const Style::font& TextStyle;
/// smileIcon: Style::icon
extern const Style::icon& smileIcon;
/// dislikeIcon: Style::icon
extern const Style::icon& dislikeIcon;
/// likeIcon: Style::icon
extern const Style::icon& likeIcon;
/// fireIcon: Style::icon
extern const Style::icon& fireIcon;
/// catIcon: Style::icon
extern const Style::icon& catIcon;
}  // namespace st
