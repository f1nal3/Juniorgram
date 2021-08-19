#pragma once

#include <QBrush>
#include <QColor>
#include <QPen>

namespace Style
{
namespace internal
{
class Icon;

/// Icon type
enum IconType
{
    /// Contrasted: black or white depends on background
    Contrasted = 0,
    /// Colored: colored on any background
    Colored = 1
};

/**
 * @class IconData
 * @brief Stores icon data
 */
class IconData
{
public:
    /// Pixmap of icon
    [[nodiscard]] const QPixmap* pixmap() const { return &_pixmap; }
    /// Size of icon
    [[nodiscard]] QSize size() const;
    /// Icon as image
    [[nodiscard]] QImage instance() const;

private:
    IconData(const QString& file, IconType type, const QColor& activeTextFg = Qt::white);
    IconData(const IconData& other) = default;
    IconData& operator=(const IconData& other) = default;

    QPixmap _pixmap;
    QImage  _image;
    QSize   _size;

    friend class Icon;
};

/**
 * @class Icon
 * @brief Pointer to icon data
 */
class Icon
{
public:
    /// Empty initialization
    Icon(Qt::Initialization = Qt::Uninitialized) {}
    /// File initialization
    Icon(const QString& file, IconType type = Colored);
    /// File initialization
    Icon(const QString& file, int type);
    /// Copy constructor
    Icon(const Icon& other) = default;
    /// Copy assigment
    Icon& operator=(const Icon& other) = default;

    /// Pixmap of icon
    operator const QPixmap*() const { return _data->pixmap(); }

    /// Size of icon
    [[nodiscard]] QSize size() const { return _data->size(); }

    /// Refer to icon data
    IconData* operator->() const { return _data; }
    /// Refer to icon data
    IconData* v() const { return _data; }

    /// Does data exist
    explicit operator bool() const { return !!_data; }

private:
    friend class IconData;
    Icon(IconData* data) : _data(data) {}

    IconData* _data = nullptr;
};
}  // namespace internal
}  // namespace Style
