#pragma once

#include <QBrush>
#include <QColor>
#include <QPen>

namespace Style
{
namespace internal
{
class Icon;

class IconData
{
public:
    [[nodiscard]] const QPixmap* pixmap() const { return &_pixmap; }
    [[nodiscard]] QSize          size() const;
    [[nodiscard]] QImage         instance() const;

private:
    IconData(const QString& file);
    IconData(const IconData& other) = default;
    IconData& operator=(const IconData& other) = default;

    QPixmap _pixmap;
    QImage  _image;
    QSize   _size;

    friend class Icon;
};

class Icon
{
public:
    Icon(Qt::Initialization = Qt::Uninitialized) {}
    Icon(const QString& file);
    Icon(const Icon& other) = default;
    Icon& operator=(const Icon& other) = default;

    operator const QPixmap*() const { return _data->pixmap(); }

    IconData* operator->() const { return _data; }
    IconData* v() const { return _data; }

    explicit operator bool() const { return !!_data; }

private:
    friend class IconData;
    Icon(IconData* data) : _data(data) {}

    IconData* _data = nullptr;
};
}  // namespace internal
}  // namespace Style
