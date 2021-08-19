// WARNING! All changes made in this file will be lost!
// This file was created automatically
#include "StyleWidgets.hpp"

#include "Palette.hpp"
#include "StyleBasic.hpp"
#include "StyleIcons.hpp"

namespace
{
bool inited = false;

class Module_StyleWidgets : public Style::internal::ModuleBase
{
public:
    Module_StyleWidgets() { Style::internal::registerModule(this); }

    void start(int scale) override { Style::internal::init_StyleWidgets(scale); }
};
Module_StyleWidgets registrator;

Style::LoadingSpinner _defaultLoadingSpinner    = {{Qt::Uninitialized}, 0., 0, 0, 0, 0, 0., 0.};
Style::Menu           _defaultMenu              = {0,
                            {0, 0, 0, 0},
                            {Qt::Uninitialized},
                            {0, 0, 0, 0},
                            {Qt::Uninitialized},
                            {Qt::Uninitialized},
                            {Qt::Uninitialized},
                            {Qt::Uninitialized},
                            {Qt::Uninitialized},
                            {Qt::Uninitialized},
                            0,
                            0};
Style::TitleBarButton _defaultTitleBarButton    = {{Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, 0, 0};
Style::TitleBarButton _closeButton              = {{Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, 0, 0};
Style::TitleBarButton _restoreButton            = {{Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, 0, 0};
Style::TitleBarButton _maximizeButton           = {{Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, 0, 0};
Style::TitleBarButton _minimizeButton           = {{Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, 0, 0};
Style::TitleBar       _defaultTitleBar          = {{Qt::Uninitialized},
                                    {{Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, 0, 0},
                                    {{Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, 0, 0},
                                    {{Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, 0, 0},
                                    {{Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, 0, 0}};
Style::IconButton     _defaultIconButton        = {{Qt::Uninitialized},
                                        {Qt::Uninitialized},
                                        {Qt::Uninitialized},
                                        {Qt::Uninitialized},
                                        {Qt::Uninitialized},
                                        {0, 0, 0, 0},
                                        0,
                                        {Qt::Uninitialized},
                                        {Qt::Uninitialized},
                                        0};
Style::IconButton     _reactionIconButtonInMenu = {{Qt::Uninitialized},
                                               {Qt::Uninitialized},
                                               {Qt::Uninitialized},
                                               {Qt::Uninitialized},
                                               {Qt::Uninitialized},
                                               {0, 0, 0, 0},
                                               0,
                                               {Qt::Uninitialized},
                                               {Qt::Uninitialized},
                                               0};
Style::IconButton     _reactionIconButton       = {{Qt::Uninitialized},
                                         {Qt::Uninitialized},
                                         {Qt::Uninitialized},
                                         {Qt::Uninitialized},
                                         {Qt::Uninitialized},
                                         {0, 0, 0, 0},
                                         0,
                                         {Qt::Uninitialized},
                                         {Qt::Uninitialized},
                                         0};
Style::IconButton     _userReactionIconButton   = {{Qt::Uninitialized},
                                             {Qt::Uninitialized},
                                             {Qt::Uninitialized},
                                             {Qt::Uninitialized},
                                             {Qt::Uninitialized},
                                             {0, 0, 0, 0},
                                             0,
                                             {Qt::Uninitialized},
                                             {Qt::Uninitialized},
                                             0};
Style::LinkButton     _defaultLinkButton        = {{Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}};
Style::FlatButton     _defaultFlatButton        = {
    {0, 0, 0, 0},       0, {Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized},
    {Qt::Uninitialized}};
Style::FlatButton _stylingButton = {
    {0, 0, 0, 0},       0, {Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized},
    {Qt::Uninitialized}};
Style::FlatButton _boldnessButton = {
    {0, 0, 0, 0},       0, {Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized},
    {Qt::Uninitialized}};
Style::FlatButton _italicButton = {
    {0, 0, 0, 0},       0, {Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized},
    {Qt::Uninitialized}};
Style::FlatButton _underlineButton = {
    {0, 0, 0, 0},       0, {Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized},
    {Qt::Uninitialized}};
Style::ScrollArea   _defaultScrollArea = {0, 0, 0, 0, {Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized},
                                        0, 0};
Style::FlatTextEdit _defaultTextEdit   = {{Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized},
                                        {Qt::Uninitialized}, {Qt::Uninitialized}, {0, 0, 0, 0}};
Style::FlatTextEdit _messageTextEdit   = {{Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized},
                                        {Qt::Uninitialized}, {Qt::Uninitialized}, {0, 0, 0, 0}};
Style::FlatButton   _messageButton     = {
    {0, 0, 0, 0},       0, {Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized},
    {Qt::Uninitialized}};
Style::MessageWidget _defaultMessageWidget = {
    0,
    {Qt::Uninitialized},
    {Qt::Uninitialized},
    {Qt::Uninitialized},
    0,
    {{0, 0, 0, 0},
     0,
     {Qt::Uninitialized},
     {Qt::Uninitialized},
     {Qt::Uninitialized},
     {Qt::Uninitialized},
     {Qt::Uninitialized},
     {Qt::Uninitialized}},
    {{Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, {0, 0, 0, 0}}};

}  // namespace

namespace st
{
const Style::LoadingSpinner& defaultLoadingSpinner(_defaultLoadingSpinner);
const Style::Menu&           defaultMenu(_defaultMenu);
const Style::TitleBarButton& defaultTitleBarButton(_defaultTitleBarButton);
const Style::TitleBarButton& closeButton(_closeButton);
const Style::TitleBarButton& restoreButton(_restoreButton);
const Style::TitleBarButton& maximizeButton(_maximizeButton);
const Style::TitleBarButton& minimizeButton(_minimizeButton);
const Style::TitleBar&       defaultTitleBar(_defaultTitleBar);
const Style::IconButton&     defaultIconButton(_defaultIconButton);
const Style::IconButton&     reactionIconButtonInMenu(_reactionIconButtonInMenu);
const Style::IconButton&     reactionIconButton(_reactionIconButton);
const Style::IconButton&     userReactionIconButton(_userReactionIconButton);
const Style::LinkButton&     defaultLinkButton(_defaultLinkButton);
const Style::FlatButton&     defaultFlatButton(_defaultFlatButton);
const Style::FlatButton&     stylingButton(_stylingButton);
const Style::FlatButton&     boldnessButton(_boldnessButton);
const Style::FlatButton&     italicButton(_italicButton);
const Style::FlatButton&     underlineButton(_underlineButton);
const Style::ScrollArea&     defaultScrollArea(_defaultScrollArea);
const Style::FlatTextEdit&   defaultTextEdit(_defaultTextEdit);
const Style::FlatTextEdit&   messageTextEdit(_messageTextEdit);
const Style::FlatButton&     messageButton(_messageButton);
const Style::MessageWidget&  defaultMessageWidget(_defaultMessageWidget);
}  // namespace st

namespace Style
{
namespace internal
{
namespace
{
int  px0   = 0;
int  px1   = 1;
int  px2   = 2;
int  px3   = 3;
int  px4   = 4;
int  px5   = 5;
int  px8   = 8;
int  px10  = 10;
int  px12  = 12;
int  px15  = 15;
int  px30  = 30;
int  px46  = 46;
int  px100 = 100;
int  px300 = 300;
void initPxValues(int scale)
{
    px0   = ConvertScale(0, scale);
    px1   = ConvertScale(1, scale);
    px2   = ConvertScale(2, scale);
    px3   = ConvertScale(3, scale);
    px4   = ConvertScale(4, scale);
    px5   = ConvertScale(5, scale);
    px8   = ConvertScale(8, scale);
    px10  = ConvertScale(10, scale);
    px12  = ConvertScale(12, scale);
    px15  = ConvertScale(15, scale);
    px30  = ConvertScale(30, scale);
    px46  = ConvertScale(46, scale);
    px100 = ConvertScale(100, scale);
    px300 = ConvertScale(300, scale);
}

}  // namespace

void init_StyleWidgets(int scale)
{
    if (inited) return;
    inited = true;

    init_Palette(scale);
    init_StyleBasic(scale);
    init_StyleIcons(scale);

    initPxValues(scale);

    _defaultLoadingSpinner    = {st::spinnerLineColor, 1.5708, 16, px8, px2, px12, 0.0314159, 0.8};
    _defaultMenu              = {px2,
                    {px4, px4, px4, px4},
                    st::windowActiveTextFg,
                    {px4, px4, px4, px4},
                    {px12, 0, 0},
                    st::windowActiveTextFg,
                    st::windowActiveTextFg,
                    st::windowColorOver,
                    st::windowColor,
                    st::windowColorOver,
                    px100,
                    px300};
    _defaultTitleBarButton    = {st::titleButton, st::titleButtonOver, {":", 0}, px30, px46};
    _closeButton              = {st::closeButtonC, st::closeButtonCOver, {":icons/close", 0}, px30, px46};
    _restoreButton            = {st::titleButton, st::titleButtonOver, {":icons/restore", 0}, px30, px46};
    _maximizeButton           = {st::titleButton, st::titleButtonOver, {":icons/max", 0}, px30, px46};
    _minimizeButton           = {st::titleButton, st::titleButtonOver, {":icons/min", 0}, px30, px46};
    _defaultTitleBar          = {st::windowColor, st::closeButton, st::maximizeButton, st::restoreButton, st::minimizeButton};
    _defaultIconButton        = {st::windowColor,
                          st::windowColorOver,
                          st::windowActiveTextFg,
                          st::windowActiveTextFg,
                          {":icons/max", 0},
                          st::defaultMargins,
                          px5,
                          st::defaultFont,
                          st::defaultFont,
                          500};
    _reactionIconButtonInMenu = {st::windowColor,
                                 st::windowColorOver,
                                 st::windowActiveTextFg,
                                 st::windowActiveTextFg,
                                 {":icons/max", 0},
                                 {px4, px4, px4, px4},
                                 px0,
                                 {px1, 0, 0},
                                 {px1, 0, 0},
                                 500};
    _reactionIconButton       = {st::windowColor,
                           st::windowColorOver,
                           st::windowActiveTextFg,
                           st::windowActiveTextFg,
                           {":icons/max", 0},
                           {px4, px4, px4, px4},
                           px4,
                           {px12, 0, 0},
                           {px12, 0, 0},
                           500};
    _userReactionIconButton   = {st::windowColor,
                               st::windowColorOver,
                               st::windowActiveTextFg,
                               st::windowActiveTextFg,
                               {":icons/max", 0},
                               {px4, px4, px4, px4},
                               px4,
                               {px12, 1, 0},
                               {px12, 1, 0},
                               500};
    _defaultLinkButton        = {st::linkButtonColor, st::linkButtonOverColor, st::defaultFont, st::italicFont};
    _defaultFlatButton        = {
        st::defaultMargins, px5, st::windowActiveTextFg, st::windowActiveTextFg, st::windowColor, st::windowColorOver, st::defaultFont,
        st::defaultFont};
    _stylingButton = {
        st::defaultMargins, px5, st::windowActiveTextFg, st::windowActiveTextFg, st::window, st::windowColorOver, st::defaultFont,
        st::defaultFont};
    _boldnessButton  = {st::defaultMargins, px5,         st::windowActiveTextFg, st::windowActiveTextFg, st::window, st::windowColorOver,
                       st::boldFont,       st::boldFont};
    _italicButton    = {st::defaultMargins, px5,           st::windowActiveTextFg, st::windowActiveTextFg, st::window, st::windowColorOver,
                     st::italicFont,     st::italicFont};
    _underlineButton = {
        st::defaultMargins, px5, st::windowActiveTextFg, st::windowActiveTextFg, st::window, st::windowColorOver, st::underlineFont,
        st::underlineFont};
    _defaultScrollArea = {px15, px10, px10, px2, st::scrollAreaColor, st::scrollAreaColorOver, st::scrollAreaBar, st::scrollAreaBarOver,
                          px3,  500};
    _defaultTextEdit   = {st::defaultFont,        st::windowActiveTextFg, st::windowColorOver,
                        st::windowActiveTextFg, st::windowColor,        st::defaultMargins};
    _messageTextEdit   = {{px12, 0, 0},           st::windowActiveTextFg, st::windowColorOver,
                        st::windowActiveTextFg, st::transparent,        {px0, px0, px0, px0}};
    _messageButton     = {
        {px4, px4, px4, px4}, px4,         st::windowActiveTextFg, st::windowActiveTextFg, st::windowColor, st::windowColorOver,
        {px12, 0, 0},         {px12, 0, 0}};
    _defaultMessageWidget = {px4, {px12, 1, 0}, {px12, 16, 0}, {px12, 0, 0}, px2, st::messageButton, st::messageTextEdit};
}

}  // namespace internal
}  // namespace Style
