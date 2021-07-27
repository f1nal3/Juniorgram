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

int                   _fsize                 = 0;
Style::font           _defaultFont           = {Qt::Uninitialized};
Style::font           _semiboldFont          = {Qt::Uninitialized};
Style::Menu           _defaultMenu           = {0,
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
Style::TitleBarButton _defaultTitleBarButton = {{Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, 0, 0};
Style::TitleBarButton _closeButton           = {{Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, 0, 0};
Style::TitleBarButton _restoreButton         = {{Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, 0, 0};
Style::TitleBarButton _maximizeButton        = {{Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, 0, 0};
Style::TitleBarButton _minimizeButton        = {{Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, 0, 0};
Style::TitleBar       _defaultTitleBar       = {{Qt::Uninitialized},
                                    {{Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, 0, 0},
                                    {{Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, 0, 0},
                                    {{Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, 0, 0},
                                    {{Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, 0, 0}};
int                   _defaultMargin         = 0;
Style::margins        _mar                   = {0, 0, 0, 0};
Style::font           _TextStyle             = {Qt::Uninitialized};
Style::IconButton     _defaultIconButton     = {{Qt::Uninitialized},
                                        {Qt::Uninitialized},
                                        {Qt::Uninitialized},
                                        {Qt::Uninitialized},
                                        {Qt::Uninitialized},
                                        {0, 0, 0, 0},
                                        0,
                                        {Qt::Uninitialized},
                                        {Qt::Uninitialized},
                                        0};
Style::IconButton     _reactionIconButton    = {{Qt::Uninitialized},
                                         {Qt::Uninitialized},
                                         {Qt::Uninitialized},
                                         {Qt::Uninitialized},
                                         {Qt::Uninitialized},
                                         {0, 0, 0, 0},
                                         0,
                                         {Qt::Uninitialized},
                                         {Qt::Uninitialized},
                                         0};
Style::IconButton     _smileIconButton       = {{Qt::Uninitialized},
                                      {Qt::Uninitialized},
                                      {Qt::Uninitialized},
                                      {Qt::Uninitialized},
                                      {Qt::Uninitialized},
                                      {0, 0, 0, 0},
                                      0,
                                      {Qt::Uninitialized},
                                      {Qt::Uninitialized},
                                      0};
Style::IconButton     _dislikeIconButton     = {{Qt::Uninitialized},
                                        {Qt::Uninitialized},
                                        {Qt::Uninitialized},
                                        {Qt::Uninitialized},
                                        {Qt::Uninitialized},
                                        {0, 0, 0, 0},
                                        0,
                                        {Qt::Uninitialized},
                                        {Qt::Uninitialized},
                                        0};
Style::IconButton     _likeIconButton        = {{Qt::Uninitialized},
                                     {Qt::Uninitialized},
                                     {Qt::Uninitialized},
                                     {Qt::Uninitialized},
                                     {Qt::Uninitialized},
                                     {0, 0, 0, 0},
                                     0,
                                     {Qt::Uninitialized},
                                     {Qt::Uninitialized},
                                     0};
Style::IconButton     _fireIconButton        = {{Qt::Uninitialized},
                                     {Qt::Uninitialized},
                                     {Qt::Uninitialized},
                                     {Qt::Uninitialized},
                                     {Qt::Uninitialized},
                                     {0, 0, 0, 0},
                                     0,
                                     {Qt::Uninitialized},
                                     {Qt::Uninitialized},
                                     0};
Style::IconButton     _catIconButton         = {{Qt::Uninitialized},
                                    {Qt::Uninitialized},
                                    {Qt::Uninitialized},
                                    {Qt::Uninitialized},
                                    {Qt::Uninitialized},
                                    {0, 0, 0, 0},
                                    0,
                                    {Qt::Uninitialized},
                                    {Qt::Uninitialized},
                                    0};
Style::LinkButton     _defaultLinkButton     = {{Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}, {Qt::Uninitialized}};
Style::FlatButton     _defaultFlatButton     = {
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
const int&                   fsize(_fsize);
const Style::font&           defaultFont(_defaultFont);
const Style::font&           semiboldFont(_semiboldFont);
const Style::Menu&           defaultMenu(_defaultMenu);
const Style::TitleBarButton& defaultTitleBarButton(_defaultTitleBarButton);
const Style::TitleBarButton& closeButton(_closeButton);
const Style::TitleBarButton& restoreButton(_restoreButton);
const Style::TitleBarButton& maximizeButton(_maximizeButton);
const Style::TitleBarButton& minimizeButton(_minimizeButton);
const Style::TitleBar&       defaultTitleBar(_defaultTitleBar);
const int&                   defaultMargin(_defaultMargin);
const Style::margins&        mar(_mar);
const Style::font&           TextStyle(_TextStyle);
const Style::IconButton&     defaultIconButton(_defaultIconButton);
const Style::IconButton&     reactionIconButton(_reactionIconButton);
const Style::IconButton&     smileIconButton(_smileIconButton);
const Style::IconButton&     dislikeIconButton(_dislikeIconButton);
const Style::IconButton&     likeIconButton(_likeIconButton);
const Style::IconButton&     fireIconButton(_fireIconButton);
const Style::IconButton&     catIconButton(_catIconButton);
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
int  px2   = 2;
int  px3   = 3;
int  px4   = 4;
int  px5   = 5;
int  px8   = 8;
int  px10  = 10;
int  px12  = 12;
int  px14  = 14;
int  px15  = 15;
int  px30  = 30;
int  px46  = 46;
int  px100 = 100;
int  px300 = 300;
void initPxValues(int scale)
{
    px0   = ConvertScale(0, scale);
    px2   = ConvertScale(2, scale);
    px3   = ConvertScale(3, scale);
    px4   = ConvertScale(4, scale);
    px5   = ConvertScale(5, scale);
    px8   = ConvertScale(8, scale);
    px10  = ConvertScale(10, scale);
    px12  = ConvertScale(12, scale);
    px14  = ConvertScale(14, scale);
    px15  = ConvertScale(15, scale);
    px30  = ConvertScale(30, scale);
    px46  = ConvertScale(46, scale);
    px100 = ConvertScale(100, scale);
    px300 = ConvertScale(300, scale);
}

}  // namespace

void init_StyleBasic(int scale)
{
    if (inited) return;
    inited = true;

    init_Palette(scale);

    initPxValues(scale);

    _fsize                 = px15;
    _defaultFont           = {px15, 0, 0};
    _semiboldFont          = {px15, 16, 0};
    _defaultMenu           = {px2,
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
    _defaultTitleBarButton = {st::titleButton, st::titleButtonOver, {":", 0}, px30, px46};
    _closeButton           = {st::closeButtonC, st::closeButtonCOver, {":icons/close", 0}, px30, px46};
    _restoreButton         = {st::titleButton, st::titleButtonOver, {":icons/restore", 0}, px30, px46};
    _maximizeButton        = {st::titleButton, st::titleButtonOver, {":icons/max", 0}, px30, px46};
    _minimizeButton        = {st::titleButton, st::titleButtonOver, {":icons/min", 0}, px30, px46};
    _defaultTitleBar       = {st::windowColor, st::closeButton, st::maximizeButton, st::restoreButton, st::minimizeButton};
    _defaultMargin         = px8;
    _mar                   = {px8, px8, px8, px8};
    _TextStyle             = {px14, 1, 0};
    _defaultIconButton     = {st::windowColor,
                          st::windowColorOver,
                          st::windowActiveTextFg,
                          st::windowActiveTextFg,
                          {":icons/max", 0},
                          st::mar,
                          px5,
                          {px15, 0, 0},
                          {px15, 0, 0},
                          500};
    _reactionIconButton    = {st::windowColor,
                           st::windowColorOver,
                           st::windowActiveTextFg,
                           st::windowActiveTextFg,
                           {":icons/max", 0},
                           {px4, px4, px4, px4},
                           px4,
                           {px12, 0, 0},
                           {px12, 0, 0},
                           500};
    _smileIconButton       = {st::windowColor,
                        st::windowColorOver,
                        st::windowActiveTextFg,
                        st::windowActiveTextFg,
                        {":reactions/smile.png", 1},
                        {px4, px4, px4, px4},
                        px4,
                        {px12, 0, 0},
                        {px12, 0, 0},
                        500};
    _dislikeIconButton     = {st::windowColor,
                          st::windowColorOver,
                          st::windowActiveTextFg,
                          st::windowActiveTextFg,
                          {":reactions/dislike.png", 1},
                          {px4, px4, px4, px4},
                          px4,
                          {px12, 0, 0},
                          {px12, 0, 0},
                          500};
    _likeIconButton        = {st::windowColor,
                       st::windowColorOver,
                       st::windowActiveTextFg,
                       st::windowActiveTextFg,
                       {":reactions/like.png", 1},
                       {px4, px4, px4, px4},
                       px4,
                       {px12, 0, 0},
                       {px12, 0, 0},
                       500};
    _fireIconButton        = {st::windowColor,
                       st::windowColorOver,
                       st::windowActiveTextFg,
                       st::windowActiveTextFg,
                       {":reactions/fire.png", 1},
                       {px4, px4, px4, px4},
                       px4,
                       {px12, 0, 0},
                       {px12, 0, 0},
                       500};
    _catIconButton         = {st::windowColor,
                      st::windowColorOver,
                      st::windowActiveTextFg,
                      st::windowActiveTextFg,
                      {":reactions/cat.png", 1},
                      {px4, px4, px4, px4},
                      px4,
                      {px12, 0, 0},
                      {px12, 0, 0},
                      500};
    _defaultLinkButton     = {st::linkButtonColor, st::linkButtonOverColor, {px15, 0, 0}, {px15, 2, 0}};
    _defaultFlatButton = {st::mar,      px5,         st::windowActiveTextFg, st::windowActiveTextFg, st::windowColor, st::windowColorOver,
                          {px15, 0, 0}, {px15, 0, 0}};
    _stylingButton     = {st::mar,      px5,         st::windowActiveTextFg, st::windowActiveTextFg, st::window, st::windowColorOver,
                      {px15, 0, 0}, {px15, 0, 0}};
    _boldnessButton    = {st::mar,      px5,         st::windowActiveTextFg, st::windowActiveTextFg, st::window, st::windowColorOver,
                       {px15, 1, 0}, {px15, 1, 0}};
    _italicButton      = {st::mar,      px5,         st::windowActiveTextFg, st::windowActiveTextFg, st::window, st::windowColorOver,
                     {px15, 2, 0}, {px15, 2, 0}};
    _underlineButton   = {st::mar,      px5,         st::windowActiveTextFg, st::windowActiveTextFg, st::window, st::windowColorOver,
                        {px15, 4, 0}, {px15, 4, 0}};
    _defaultScrollArea = {px15, px10, px10, px2, st::scrollAreaColor, st::scrollAreaColorOver, st::scrollAreaBar, st::scrollAreaBarOver,
                          px3,  500};
    _defaultTextEdit   = {{px15, 0, 0}, st::windowActiveTextFg, st::windowColorOver, st::windowActiveTextFg, st::windowColor, st::mar};
    _messageTextEdit   = {{px12, 0, 0},           st::windowActiveTextFg, st::windowColorOver,
                        st::windowActiveTextFg, st::transparent,        {px0, px0, px0, px0}};
    _messageButton     = {
        {px4, px4, px4, px4}, px4,         st::windowActiveTextFg, st::windowActiveTextFg, st::windowColor, st::windowColorOver,
        {px12, 0, 0},         {px12, 0, 0}};
    _defaultMessageWidget = {px4, {px12, 1, 0}, {px12, 16, 0}, {px12, 0, 0}, px2, st::messageButton, st::messageTextEdit};
}

}  // namespace internal
}  // namespace Style
