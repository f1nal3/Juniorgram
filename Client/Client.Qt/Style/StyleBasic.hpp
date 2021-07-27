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
    Style::color   bgColor;
    Style::color   overBgColor;
    Style::color   color;
    Style::color   overColor;
    Style::icon    icon;
    Style::margins margins;
    int            rounding;
    Style::font    font;
    Style::font    overFont;
    int            duration;
};

struct TitleBarButton
{
    Style::color bgColor;
    Style::color overBgColor;
    Style::icon  icon;
    int          height;
    int          width;
};

struct TitleBar
{
    Style::color          bgColor;
    Style::TitleBarButton closeButton;
    Style::TitleBarButton maximizeButton;
    Style::TitleBarButton restoreButton;
    Style::TitleBarButton minimizeButton;
};

struct Menu
{
    int            separatorWidth;
    Style::margins separatorPadding;
    Style::color   separatorFg;
    Style::margins itemPadding;
    Style::font    itemFont;
    Style::color   itemFg;
    Style::color   itemFgOver;
    Style::color   itemFgDisabled;
    Style::color   itemBg;
    Style::color   itemBgOver;
    int            widthMin;
    int            widthMax;
};

}  // namespace Style

namespace st
{
extern const int&                   fsize;
extern const Style::font&           defaultFont;
extern const Style::font&           semiboldFont;
extern const Style::Menu&           defaultMenu;
extern const Style::TitleBarButton& defaultTitleBarButton;
extern const Style::TitleBarButton& closeButton;
extern const Style::TitleBarButton& restoreButton;
extern const Style::TitleBarButton& maximizeButton;
extern const Style::TitleBarButton& minimizeButton;
extern const Style::TitleBar&       defaultTitleBar;
extern const int&                   defaultMargin;
extern const Style::margins&        mar;
extern const Style::font&           TextStyle;
extern const Style::IconButton&     defaultIconButton;
extern const Style::IconButton&     reactionIconButton;
extern const Style::IconButton&     smileIconButton;
extern const Style::IconButton&     dislikeIconButton;
extern const Style::IconButton&     likeIconButton;
extern const Style::IconButton&     fireIconButton;
extern const Style::IconButton&     catIconButton;
extern const Style::LinkButton&     defaultLinkButton;
extern const Style::FlatButton&     defaultFlatButton;
extern const Style::FlatButton&     stylingButton;
extern const Style::FlatButton&     boldnessButton;
extern const Style::FlatButton&     italicButton;
extern const Style::FlatButton&     underlineButton;
extern const Style::ScrollArea&     defaultScrollArea;
extern const Style::FlatTextEdit&   defaultTextEdit;
extern const Style::FlatTextEdit&   messageTextEdit;
extern const Style::FlatButton&     messageButton;
extern const Style::MessageWidget&  defaultMessageWidget;
}  // namespace st
