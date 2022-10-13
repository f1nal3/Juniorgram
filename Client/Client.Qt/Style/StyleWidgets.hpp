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

}  /// namespace internal

/// Style struct for ScrollArea
struct ScrollArea
{
    int width;
    int deltaT;
    int deltaB;
    int deltaX;
   
    Style::color bg;
    Style::color bgOver;
    Style::color barBg;
    Style::color barBgOver;
   
    int round;
    int hiding;
};

/// Style struct for FlatButton
struct FlatButton
{
    Style::margins paddings;
    
    int rounding;

    Style::color color;
    Style::color overColor;
    Style::color bgColor;
    Style::color overBgColor;

    Style::font font;
    Style::font overFont;
};

/// Style struct for FlatTextEdit
struct FlatTextEdit
{
    Style::font font;

    Style::color textcolor;
    Style::color selectedcolor;
    Style::color selectedtextcolor;
    Style::color bg;

    Style::margins margins;
};

/// Style struct for MessageWidget
struct MessageWidget
{
    int radius;

    Style::font fontname;
    Style::font fontdate;
    Style::font fonttext;
    
    int border;

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
   
    int rounding;

    Style::font font;
    Style::font overFont;

    int duration;
};

/// Style struct for TitleBarButton
struct TitleBarButton
{
    Style::color bgColor;
    Style::color overBgColor;
    Style::icon  icon;
    
    int height;
    int width;
};

/// Style struct for TitleBar
struct TitleBar
{
    Style::color bgColor;

    Style::TitleBarButton closeButton;
    Style::TitleBarButton maximizeButton;
    Style::TitleBarButton restoreButton;
    Style::TitleBarButton minimizeButton;
};

/// Style struct for Menu
struct Menu
{
    int separatorWidth;
   
    Style::margins separatorPadding;
    Style::color   separatorFg;
    Style::margins itemPadding;

    Style::font  itemFont;
    Style::color itemFg;
    Style::color itemFgOver;
    Style::color itemFgDisabled;
    Style::color itemBg;
    Style::color itemBgOver;
   
    int widthMin;
    int widthMax;
};

/// Style struct for LoadingSpinner
struct LoadingSpinner
{
    Style::color lineColor; 

    double revolutionsPerSecond;
    int    numberOfLines;
    
    int lineLength;
    int lineWidth;

    int    innerRadius;
    double minimumTrailOpacity;
    double trailFadePercentage;
};

}  /// namespace Style

namespace st
{
extern const Style::LoadingSpinner& defaultLoadingSpinner;
extern const Style::Menu&           defaultMenu;
extern const Style::TitleBarButton& defaultTitleBarButton;
extern const Style::TitleBarButton& closeButton;
extern const Style::TitleBarButton& restoreButton;
extern const Style::TitleBarButton& maximizeButton;
extern const Style::TitleBarButton& minimizeButton;

extern const Style::TitleBar&   defaultTitleBar;
extern const Style::IconButton& defaultIconButton;
extern const Style::IconButton& reactionIconButtonInMenu;
extern const Style::IconButton& reactionIconButton;
extern const Style::IconButton& userReactionIconButton;
extern const Style::LinkButton& defaultLinkButton;
extern const Style::FlatButton& defaultFlatButton;
extern const Style::FlatButton& stylingButton;
extern const Style::FlatButton& boldnessButton;
extern const Style::FlatButton& italicButton;
extern const Style::FlatButton& underlineButton;
extern const Style::ScrollArea& defaultScrollArea;

extern const Style::FlatTextEdit&  defaultTextEdit;
extern const Style::FlatTextEdit&  messageTextEdit;
extern const Style::FlatButton&    messageButton;
extern const Style::MessageWidget& defaultMessageWidget;
}  /// namespace st
