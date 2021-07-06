// WARNING! All changes made in this file will be lost!
// This file was created automatically
#pragma once

#include "Style/Style.hpp"

namespace Style
{
namespace internal
{
void init_StyleBasic(int scale);

}  // namespace internal

struct TextPalette
{
    Style::color linkFg;
    Style::color selectBg;
    Style::color selectFg;
    Style::color selectLinkFg;
    Style::color selectOverlay;
};

struct TextStyle
{
    Style::font font;
    Style::font linkFont;
    Style::font linkFontOver;
    int         lineHeight;
};

struct ScrollArea
{
    int          width;
    int          deltat;
    int          deltab;
    int          deltax;
    Style::color bg;
    Style::color bgOver;
    Style::color barBg;
    Style::color barBgOver;
    int          round;
    int          hiding;
};

struct FlatButton
{
    Style::margins paddings;
    int            rounding;
    Style::color   color;
    Style::color   overColor;
    Style::color   bgColor;
    Style::color   overBgColor;
    Style::font    font;
    Style::font    overFont;
};

struct LinkButton
{
    Style::color color;
    Style::color overColor;
    Style::font  font;
    Style::font  overFont;
};

struct IconButton
{
    Style::color bgColor;
    Style::color overBgColor;
    int          duration;
};

}  // namespace Style

namespace st
{
extern const Style::TextPalette& defaultTextPalette;
extern const int&                fsize;
extern const Style::font&        defaultFont;
extern const Style::font&        semiboldFont;
extern const Style::TextStyle&   defaultTextStyle;
extern const Style::TextStyle&   semiboldTextStyle;
extern const Style::IconButton&  defaultTitleButton;
extern const Style::IconButton&  closeButton;
extern const int&                defaultMargin;
extern const Style::margins&     mar;
extern const Style::font&        TextStyle;
extern const Style::LinkButton&  defaultLinkButton;
extern const Style::FlatButton&  defaultFlatButton;
extern const Style::FlatButton&  stylingButton;
extern const Style::FlatButton&  boldnessButton;
extern const Style::FlatButton&  italicButton;
extern const Style::FlatButton&  underlineButton;
extern const Style::ScrollArea&  defaultScrollArea;
}  // namespace st
