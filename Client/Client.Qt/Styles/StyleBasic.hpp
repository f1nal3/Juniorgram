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
/// defaultFontSize: pixels
extern const int& defaultFontSize;
/// defaultFont: Style::font
extern const Style::font& defaultFont;
/// semiboldFont: Style::font
extern const Style::font& semiboldFont;
/// italicFont: Style::font
extern const Style::font& italicFont;
/// boldFont: Style::font
extern const Style::font& boldFont;
/// underlineFont: Style::font
extern const Style::font& underlineFont;
/// defaultMargin: pixels
extern const int& defaultMargin;
/// defaultMargins: Style::margins
extern const Style::margins& defaultMargins;
/// minWidth: pixels
extern const int& minWidth;
/// minHeight: pixels
extern const int& minHeight;
/// shadowPadding: pixels
extern const int& shadowPadding;
/// authBlockWidth: pixels
extern const int& authBlockWidth;
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
