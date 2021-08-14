// WARNING! All changes made in this file will be lost!
// This file was created automatically
#pragma once

#include "Style/Style.hpp"

namespace Style
{
namespace internal
{
/// StyleWidgets initializer
void init_StyleWidgets(int scale);

}  // namespace internal

/// Style struct for ScrollArea
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

/// Style struct for FlatButton
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

/// Style struct for FlatTextEdit
struct FlatTextEdit
{
    Style::font    font;
    Style::color   textcolor;
    Style::color   selectedcolor;
    Style::color   selectedtextcolor;
    Style::color   bg;
    Style::margins margins;
};

/// Style struct for MessageWidget
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

/// Style struct for LinkButton
struct LinkButton
{
    Style::color color;
    Style::color overColor;
    Style::font  font;
    Style::font  overFont;
};

/// Style struct for IconButton
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

/// Style struct for TitleBarButton
struct TitleBarButton
{
    Style::color bgColor;
    Style::color overBgColor;
    Style::icon  icon;
    int          height;
    int          width;
};

/// Style struct for TitleBar
struct TitleBar
{
    Style::color          bgColor;
    Style::TitleBarButton closeButton;
    Style::TitleBarButton maximizeButton;
    Style::TitleBarButton restoreButton;
    Style::TitleBarButton minimizeButton;
};

/// Style struct for Menu
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

/// Style struct for LoadingSpinner
struct LoadingSpinner
{
    Style::color lineColor;
    double       revolutionsPerSecond;
    int          numberOfLines;
    int          lineLength;
    int          lineWidth;
    int          innerRadius;
    double       minimumTrailOpacity;
    double       trailFadePercentage;
};

}  // namespace Style

namespace st
{
/// defaultLoadingSpinner: Style::LoadingSpinner
extern const Style::LoadingSpinner& defaultLoadingSpinner;
/// defaultMenu: Style::Menu
extern const Style::Menu& defaultMenu;
/// defaultTitleBarButton: Style::TitleBarButton
extern const Style::TitleBarButton& defaultTitleBarButton;
/// closeButton: Style::TitleBarButton
extern const Style::TitleBarButton& closeButton;
/// restoreButton: Style::TitleBarButton
extern const Style::TitleBarButton& restoreButton;
/// maximizeButton: Style::TitleBarButton
extern const Style::TitleBarButton& maximizeButton;
/// minimizeButton: Style::TitleBarButton
extern const Style::TitleBarButton& minimizeButton;
/// defaultTitleBar: Style::TitleBar
extern const Style::TitleBar& defaultTitleBar;
/// defaultIconButton: Style::IconButton
extern const Style::IconButton& defaultIconButton;
/// reactionIconButtonInMenu: Style::IconButton
extern const Style::IconButton& reactionIconButtonInMenu;
/// reactionIconButton: Style::IconButton
extern const Style::IconButton& reactionIconButton;
/// userReactionIconButton: Style::IconButton
extern const Style::IconButton& userReactionIconButton;
/// defaultLinkButton: Style::LinkButton
extern const Style::LinkButton& defaultLinkButton;
/// defaultFlatButton: Style::FlatButton
extern const Style::FlatButton& defaultFlatButton;
/// stylingButton: Style::FlatButton
extern const Style::FlatButton& stylingButton;
/// boldnessButton: Style::FlatButton
extern const Style::FlatButton& boldnessButton;
/// italicButton: Style::FlatButton
extern const Style::FlatButton& italicButton;
/// underlineButton: Style::FlatButton
extern const Style::FlatButton& underlineButton;
/// defaultScrollArea: Style::ScrollArea
extern const Style::ScrollArea& defaultScrollArea;
/// defaultTextEdit: Style::FlatTextEdit
extern const Style::FlatTextEdit& defaultTextEdit;
/// messageTextEdit: Style::FlatTextEdit
extern const Style::FlatTextEdit& messageTextEdit;
/// messageButton: Style::FlatButton
extern const Style::FlatButton& messageButton;
/// defaultMessageWidget: Style::MessageWidget
extern const Style::MessageWidget& defaultMessageWidget;
}  // namespace st
