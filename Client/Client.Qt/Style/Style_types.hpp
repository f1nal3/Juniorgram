#pragma once

#include <QColor>
#include <QCursor>
#include <QFont>
#include <QPoint>
#include <QRect>
#include <QString>

#include "Style_color.hpp"
#include "Style_font.hpp"

namespace Style
{
using font    = internal::Font;
using color   = internal::Color;
using margins = QMargins;
using cursor  = Qt::CursorShape;
using align   = Qt::Alignment;

static constexpr cursor cur_default   = Qt::ArrowCursor;
static constexpr cursor cur_pointer   = Qt::PointingHandCursor;
static constexpr cursor cur_text      = Qt::IBeamCursor;
static constexpr cursor cur_cross     = Qt::CrossCursor;
static constexpr cursor cur_sizever   = Qt::SizeVerCursor;
static constexpr cursor cur_sizehor   = Qt::SizeHorCursor;
static constexpr cursor cur_sizebdiag = Qt::SizeBDiagCursor;
static constexpr cursor cur_sizefdiag = Qt::SizeFDiagCursor;
static constexpr cursor cur_sizeall   = Qt::SizeAllCursor;

static const align al_topleft     = (Qt::AlignTop | Qt::AlignLeft);
static const align al_top         = (Qt::AlignTop | Qt::AlignHCenter);
static const align al_topright    = (Qt::AlignTop | Qt::AlignRight);
static const align al_right       = (Qt::AlignVCenter | Qt::AlignRight);
static const align al_bottomright = (Qt::AlignBottom | Qt::AlignRight);
static const align al_bottom      = (Qt::AlignBottom | Qt::AlignHCenter);
static const align al_bottomleft  = (Qt::AlignBottom | Qt::AlignLeft);
static const align al_left        = (Qt::AlignVCenter | Qt::AlignLeft);
static const align al_center      = (Qt::AlignVCenter | Qt::AlignHCenter);

}  // namespace Style