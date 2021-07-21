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

struct FlatTextEdit
{
    Style::font    font;
    Style::color   textcolor;
    Style::color   selectedcolor;
    Style::color   selectedtextcolor;
    Style::color   bg;
    Style::margins margins;
};

struct MessageWidget
{
    int                 radius;
    Style::font         fontname;
    Style::font         fontdate;
    Style::font         fonttext;
    int                 border;
    Style::FlatButton   button;
    Style::FlatTextEdit textedit;
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
extern const int&                  fsize;
extern const Style::font&          defaultFont;
extern const Style::font&          semiboldFont;
extern const Style::IconButton&    defaultTitleButton;
extern const Style::IconButton&    closeButton;
extern const int&                  defaultMargin;
extern const Style::margins&       mar;
extern const Style::font&          TextStyle;
extern const Style::LinkButton&    defaultLinkButton;
extern const Style::FlatButton&    defaultFlatButton;
extern const Style::FlatButton&    stylingButton;
extern const Style::FlatButton&    boldnessButton;
extern const Style::FlatButton&    italicButton;
extern const Style::FlatButton&    underlineButton;
extern const Style::ScrollArea&    defaultScrollArea;
extern const Style::FlatTextEdit&  defaultTextEdit;
extern const Style::FlatTextEdit&  messageTextEdit;
extern const Style::icon&          closeButtonIcon;
extern const Style::icon&          restoreButtonIcon;
extern const Style::icon&          maximizeButtonIcon;
extern const Style::icon&          minimizeButtonIcon;
extern const Style::FlatButton&    messageButton;
extern const Style::MessageWidget& defaultMessageWidget;
}  // namespace st
