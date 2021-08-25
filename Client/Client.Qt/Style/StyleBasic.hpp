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
/// titleBarHeight: pixels
extern const int& titleBarHeight;
/// titleBarMaxIconWidth: pixels
extern const int& titleBarMaxIconWidth;
/// authBlockWidth: pixels
extern const int& authBlockWidth;
/// authBlockSpacing: pixels
extern const int& authBlockSpacing;
/// chatWidgetMinWidth: pixels
extern const int& chatWidgetMinWidth;
/// channelListMinWidth: pixels
extern const int& channelListMinWidth;
/// channelListMaxWidth: pixels
extern const int& channelListMaxWidth;
}  // namespace st
