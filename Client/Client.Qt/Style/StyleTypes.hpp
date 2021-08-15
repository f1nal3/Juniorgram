#pragma once

#include <QColor>
#include <QCursor>
#include <QFont>
#include <QPoint>
#include <QRect>
#include <QString>

#include "StyleColor.hpp"
#include "StyleFont.hpp"
#include "StyleIcon.hpp"

namespace Style
{
/// Icon type
using icon = internal::Icon;
/// Font type
using font = internal::Font;
/// Color type
using color = internal::Color;
/// Margins type
using margins = QMargins;
/// Cursor type
using cursor = Qt::CursorShape;
/// Align type
using align = Qt::Alignment;

/// https://doc.qt.io/qt-5/qcursor.html#a-note-for-x11-users
/// Default cursor
static constexpr cursor cur_default = Qt::ArrowCursor;
/// Pointer cursor
static constexpr cursor cur_pointer = Qt::PointingHandCursor;
/// Text cursor(I)
static constexpr cursor cur_text = Qt::IBeamCursor;
/// Cross cursor
static constexpr cursor cur_cross = Qt::CrossCursor;
/// Vertical sizing cursor
static constexpr cursor cur_sizever = Qt::SizeVerCursor;
/// Horizontal sizing cursor
static constexpr cursor cur_sizehor = Qt::SizeHorCursor;
/// Left-bottom/Right-top cursor
static constexpr cursor cur_sizebdiag = Qt::SizeBDiagCursor;
/// Left-top/Right-bottom cursor
static constexpr cursor cur_sizefdiag = Qt::SizeFDiagCursor;
/// SizeAll cursor
static constexpr cursor cur_sizeall = Qt::SizeAllCursor;

/// Top left align
static const align al_topleft = (Qt::AlignTop | Qt::AlignLeft);
/// Top align
static const align al_top = (Qt::AlignTop | Qt::AlignHCenter);
/// Top right align
static const align al_topright = (Qt::AlignTop | Qt::AlignRight);
/// Right align
static const align al_right = (Qt::AlignVCenter | Qt::AlignRight);
/// Bottom right align
static const align al_bottomright = (Qt::AlignBottom | Qt::AlignRight);
/// Bottom align
static const align al_bottom = (Qt::AlignBottom | Qt::AlignHCenter);
/// Bottom left align
static const align al_bottomleft = (Qt::AlignBottom | Qt::AlignLeft);
/// Left align
static const align al_left = (Qt::AlignVCenter | Qt::AlignLeft);
/// Center align
static const align al_center = (Qt::AlignVCenter | Qt::AlignHCenter);

}  // namespace Style
