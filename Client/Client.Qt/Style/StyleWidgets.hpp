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
    /// width: pixels
    int width;
    /// deltaT: pixels
    int deltaT;
    /// deltaB: pixels
    int deltaB;
    /// deltaX: pixels
    int deltaX;
    /// bg: Style::color
    Style::color bg;
    /// bgOver: Style::color
    Style::color bgOver;
    /// barBg: Style::color
    Style::color barBg;
    /// barBgOver: Style::color
    Style::color barBgOver;
    /// round: pixels
    int round;
    /// hiding: int
    int hiding;
};

/// Style struct for FlatButton
struct FlatButton
{
    /// paddings: Style::margins
    Style::margins paddings;
    /// rounding: pixels
    int rounding;
    /// color: Style::color
    Style::color color;
    /// overColor: Style::color
    Style::color overColor;
    /// bgColor: Style::color
    Style::color bgColor;
    /// overBgColor: Style::color
    Style::color overBgColor;
    /// font: Style::font
    Style::font font;
    /// overFont: Style::font
    Style::font overFont;
};

/// Style struct for FlatTextEdit
struct FlatTextEdit
{
    /// font: Style::font
    Style::font font;
    /// textcolor: Style::color
    Style::color textcolor;
    /// selectedcolor: Style::color
    Style::color selectedcolor;
    /// selectedtextcolor: Style::color
    Style::color selectedtextcolor;
    /// bg: Style::color
    Style::color bg;
    /// margins: Style::margins
    Style::margins margins;
};

/// Style struct for MessageWidget
struct MessageWidget
{
    /// radius: pixels
    int radius;
    /// fontname: Style::font
    Style::font fontname;
    /// fontdate: Style::font
    Style::font fontdate;
    /// fonttext: Style::font
    Style::font fonttext;
    /// border: pixels
    int border;
    /// button: Style::FlatButton
    Style::FlatButton button;
    /// textedit: Style::FlatTextEdit
    Style::FlatTextEdit textedit;
};

/// Style struct for LinkButton
struct LinkButton
{
    /// color: Style::color
    Style::color color;
    /// overColor: Style::color
    Style::color overColor;
    /// font: Style::font
    Style::font font;
    /// overFont: Style::font
    Style::font overFont;
};

/// Style struct for IconButton
struct IconButton
{
    /// bgColor: Style::color
    Style::color bgColor;
    /// overBgColor: Style::color
    Style::color overBgColor;
    /// color: Style::color
    Style::color color;
    /// overColor: Style::color
    Style::color overColor;
    /// icon: Style::icon
    Style::icon icon;
    /// margins: Style::margins
    Style::margins margins;
    /// rounding: pixels
    int rounding;
    /// font: Style::font
    Style::font font;
    /// overFont: Style::font
    Style::font overFont;
    /// duration: int
    int duration;
};

/// Style struct for TitleBarButton
struct TitleBarButton
{
    /// bgColor: Style::color
    Style::color bgColor;
    /// overBgColor: Style::color
    Style::color overBgColor;
    /// icon: Style::icon
    Style::icon icon;
    /// height: pixels
    int height;
    /// width: pixels
    int width;
};

/// Style struct for TitleBar
struct TitleBar
{
    /// bgColor: Style::color
    Style::color bgColor;
    /// closeButton: Style::TitleBarButton
    Style::TitleBarButton closeButton;
    /// maximizeButton: Style::TitleBarButton
    Style::TitleBarButton maximizeButton;
    /// restoreButton: Style::TitleBarButton
    Style::TitleBarButton restoreButton;
    /// minimizeButton: Style::TitleBarButton
    Style::TitleBarButton minimizeButton;
};

/// Style struct for Menu
struct Menu
{
    /// separatorWidth: pixels
    int separatorWidth;
    /// separatorPadding: Style::margins
    Style::margins separatorPadding;
    /// separatorFg: Style::color
    Style::color separatorFg;
    /// itemPadding: Style::margins
    Style::margins itemPadding;
    /// itemFont: Style::font
    Style::font itemFont;
    /// itemFg: Style::color
    Style::color itemFg;
    /// itemFgOver: Style::color
    Style::color itemFgOver;
    /// itemFgDisabled: Style::color
    Style::color itemFgDisabled;
    /// itemBg: Style::color
    Style::color itemBg;
    /// itemBgOver: Style::color
    Style::color itemBgOver;
    /// widthMin: pixels
    int widthMin;
    /// widthMax: pixels
    int widthMax;
};

/// Style struct for LoadingSpinner
struct LoadingSpinner
{
    /// lineColor: Style::color
    Style::color lineColor;
    /// revolutionsPerSecond: double
    double revolutionsPerSecond;
    /// numberOfLines: int
    int numberOfLines;
    /// lineLength: pixels
    int lineLength;
    /// lineWidth: pixels
    int lineWidth;
    /// innerRadius: pixels
    int innerRadius;
    /// minimumTrailOpacity: double
    double minimumTrailOpacity;
    /// trailFadePercentage: double
    double trailFadePercentage;
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
