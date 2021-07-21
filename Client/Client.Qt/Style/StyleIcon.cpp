#include "StyleIcon.hpp"

#include <QFile>
#include <QPainter>

#include "StyleScale.hpp"

namespace Style
{
namespace internal
{
IconData::IconData(const QString& filename)
{
    _image = QImage();
    _image.load(filename);
    _image.setDevicePixelRatio(devicePixelRatio());

    _pixmap = QPixmap();
    _size   = QSize();
    if (!_image.isNull())
    {
        _pixmap = QPixmap::fromImage(_image);
        _size   = _image.size();
    }
}

QImage IconData::instance() const
{
    if (!_image.isNull())
    {
        return _image;
    }
    else
        return QImage();
}

QSize internal::IconData::size() const { return _size; }

Icon::Icon(const QString& file)
{
    QString filename = file;
    for (int scale = Style::getDpiScale(); scale >= 100; scale -= 5)
    {
        QFile fileinfo(file + QString("-%1.png").arg(scale));
        if (fileinfo.exists())
        {
            filename = fileinfo.fileName();
            break;
        }
    }
    _data = new IconData(filename);
}

}  // namespace internal
}  // namespace Style
