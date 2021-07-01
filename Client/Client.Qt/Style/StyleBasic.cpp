// WARNING! All changes made in this file will be lost!
// This file was created automatically
#include "StyleBasic.hpp"

#include "Style/Palette.hpp"

namespace
{
bool inited = false;

class Module_StyleBasic : public Style::internal::ModuleBase
{
public:
    Module_StyleBasic() { Style::internal::registerModule(this); }

    void start(int scale) override { Style::internal::init_StyleBasic(scale); }
};
Module_StyleBasic registrator;

Style::TextPalette _defaultTextPalette = {{Qt::Uninitialized},
                                          {Qt::Uninitialized},
                                          {Qt::Uninitialized},
                                          {Qt::Uninitialized},
                                          {Qt::Uninitialized}};
int                _fsize              = 0;
Style::font        _defaultFont        = {Qt::Uninitialized};
Style::font        _semiboldFont       = {Qt::Uninitialized};
Style::TextStyle   _defaultTextStyle   = {
    {Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, 0};
Style::TextStyle _semiboldTextStyle = {
    {Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, 0};
Style::IconButton _defaultTitleButton = {{Qt::Uninitialized}, {Qt::Uninitialized}, 0};
Style::IconButton _closeButton        = {{Qt::Uninitialized}, {Qt::Uninitialized}, 0};
int               _defaultMargin      = 0;
Style::margins    _mar                = {0, 0, 0, 0};
Style::font       _TextStyle          = {Qt::Uninitialized};
Style::LinkButton _defaultLinkButton  = {
    {Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}};
Style::FlatButton _defaultFlatButton = {{0, 0, 0, 0},        0,
                                        {Qt::Uninitialized}, {Qt::Uninitialized},
                                        {Qt::Uninitialized}, {Qt::Uninitialized},
                                        {Qt::Uninitialized}, {Qt::Uninitialized}};
Style::FlatButton _stylingButton     = {{0, 0, 0, 0},        0,
                                    {Qt::Uninitialized}, {Qt::Uninitialized},
                                    {Qt::Uninitialized}, {Qt::Uninitialized},
                                    {Qt::Uninitialized}, {Qt::Uninitialized}};
Style::FlatButton _boldnessButton    = {{0, 0, 0, 0},        0,
                                     {Qt::Uninitialized}, {Qt::Uninitialized},
                                     {Qt::Uninitialized}, {Qt::Uninitialized},
                                     {Qt::Uninitialized}, {Qt::Uninitialized}};
Style::FlatButton _italicButton      = {{0, 0, 0, 0},        0,
                                   {Qt::Uninitialized}, {Qt::Uninitialized},
                                   {Qt::Uninitialized}, {Qt::Uninitialized},
                                   {Qt::Uninitialized}, {Qt::Uninitialized}};
Style::FlatButton _underlineButton   = {{0, 0, 0, 0},        0,
                                      {Qt::Uninitialized}, {Qt::Uninitialized},
                                      {Qt::Uninitialized}, {Qt::Uninitialized},
                                      {Qt::Uninitialized}, {Qt::Uninitialized}};
Style::ScrollArea _defaultScrollArea = {
    0, 0, 0, 0, {Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized},
    0, 0};

}  // namespace

namespace st
{
const Style::TextPalette& defaultTextPalette(_defaultTextPalette);
const int&                fsize(_fsize);
const Style::font&        defaultFont(_defaultFont);
const Style::font&        semiboldFont(_semiboldFont);
const Style::TextStyle&   defaultTextStyle(_defaultTextStyle);
const Style::TextStyle&   semiboldTextStyle(_semiboldTextStyle);
const Style::IconButton&  defaultTitleButton(_defaultTitleButton);
const Style::IconButton&  closeButton(_closeButton);
const int&                defaultMargin(_defaultMargin);
const Style::margins&     mar(_mar);
const Style::font&        TextStyle(_TextStyle);
const Style::LinkButton&  defaultLinkButton(_defaultLinkButton);
const Style::FlatButton&  defaultFlatButton(_defaultFlatButton);
const Style::FlatButton&  stylingButton(_stylingButton);
const Style::FlatButton&  boldnessButton(_boldnessButton);
const Style::FlatButton&  italicButton(_italicButton);
const Style::FlatButton&  underlineButton(_underlineButton);
const Style::ScrollArea&  defaultScrollArea(_defaultScrollArea);
}  // namespace st

namespace Style
{
namespace internal
{
namespace
{
int  px0  = 0;
int  px2  = 2;
int  px3  = 3;
int  px5  = 5;
int  px8  = 8;
int  px10 = 10;
int  px14 = 14;
int  px15 = 15;
void initPxValues(int scale)
{
    px0  = ConvertScale(0, scale);
    px2  = ConvertScale(2, scale);
    px3  = ConvertScale(3, scale);
    px5  = ConvertScale(5, scale);
    px8  = ConvertScale(8, scale);
    px10 = ConvertScale(10, scale);
    px14 = ConvertScale(14, scale);
    px15 = ConvertScale(15, scale);
}

}  // namespace

void init_StyleBasic(int scale)
{
    if (inited) return;
    inited = true;

    init_Palette(scale);

    initPxValues(scale);

    _defaultTextPalette = {st::windowActiveTextFg, st::windowActiveTextFg, st::transparent,
                           st::windowActiveTextFg, st::windowActiveTextFg};
    _fsize              = px15;
    _defaultFont        = {px15, 0, 0};
    _semiboldFont       = {px15, 16, 0};
    _defaultTextStyle   = {st::defaultFont, st::defaultFont, {px15, 4, 0}, px0};
    _semiboldTextStyle  = {st::semiboldFont, st::semiboldFont, {px15, 20, 0}, px0};
    _defaultTitleButton = {st::titleButton, st::titleButtonOver, 150};
    _closeButton        = {st::closeButtonC, st::closeButtonCOver, 150};
    _defaultMargin      = px8;
    _mar                = {px8, px8, px8, px8};
    _TextStyle          = {px14, 1, 0};
    _defaultLinkButton = {st::linkButtonColor, st::linkButtonOverColor, {px15, 0, 0}, {px15, 2, 0}};
    _defaultFlatButton = {st::mar,
                          px5,
                          st::windowActiveTextFg,
                          st::windowActiveTextFg,
                          st::windowColor,
                          st::windowColorOver,
                          {px15, 0, 0},
                          {px15, 0, 0}};
    _stylingButton     = {st::mar,
                      px5,
                      st::windowActiveTextFg,
                      st::windowActiveTextFg,
                      st::window,
                      st::windowColorOver,
                      {px15, 0, 0},
                      {px15, 0, 0}};
    _boldnessButton    = {st::mar,
                       px5,
                       st::windowActiveTextFg,
                       st::windowActiveTextFg,
                       st::window,
                       st::windowColorOver,
                       {px15, 1, 0},
                       {px15, 1, 0}};
    _italicButton      = {st::mar,
                     px5,
                     st::windowActiveTextFg,
                     st::windowActiveTextFg,
                     st::window,
                     st::windowColorOver,
                     {px15, 2, 0},
                     {px15, 2, 0}};
    _underlineButton   = {st::mar,
                        px5,
                        st::windowActiveTextFg,
                        st::windowActiveTextFg,
                        st::window,
                        st::windowColorOver,
                        {px15, 4, 0},
                        {px15, 4, 0}};
    _defaultScrollArea = {px15,
                          px10,
                          px10,
                          px2,
                          st::windowColor,
                          st::windowColorOver,
                          st::windowColorOver,
                          st::windowActiveTextFg,
                          px3,
                          2000};
}

}  // namespace internal
}  // namespace Style
