#include "StyleIcon.hpp"

#include <QBitmap>
#include <QDebug>
#include <QFile>
#include <QPainter>

#include "StyleScale.hpp"

namespace Style
{
namespace internal
{
IconData::IconData(const QString& filename, const QColor& activeTextFg)
{
    _image = QImage();
    _image.load(filename);
    if (_image.isNull()) return;
    {
        auto   alpha     = QPixmap::fromImage(_image.createAlphaMask());
        QImage fakeImage = QImage(_image.size(), _image.format());
        fakeImage.setDevicePixelRatio(devicePixelRatio());
        fakeImage.fill(Qt::transparent);
        QPainter p(&fakeImage);
        p.setClipRegion(QRegion(alpha));
        p.setPen(Qt::NoPen);
        p.setBrush(activeTextFg);
        p.drawRect(_image.rect());
        _image = fakeImage;
    }

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
