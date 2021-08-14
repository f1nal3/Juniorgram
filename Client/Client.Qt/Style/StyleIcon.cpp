#include "StyleIcon.hpp"

#include <QBitmap>
#include <QFile>
#include <QPainter>

#include "StyleScale.hpp"
#include "StyleTypes.hpp"
#include "Styles/Palette.hpp"

namespace Style::internal
{
IconData::IconData(const QString& filename, IconType type, const QColor& activeTextFg)
{
    _image = QImage();
    _image.load(filename);
    if (_image.isNull()) return;
    if (type == Contrasted)
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

Icon::Icon(const QString& file, IconType iconType)
{
    QString filename = file;
    if (iconType == Contrasted)
    {
        int scale = Style::getDpiScale();
        scale -= Style::getDpiScale() % 25;
        for (; scale >= 100; scale -= 25)
        {
            QFile fileInfo(file + QString("-w-%1.png").arg(scale));
            if (fileInfo.exists())
            {
                filename = fileInfo.fileName();
                break;
            }
        }
    }
    _data = new IconData(filename, iconType);
}
Icon::Icon(const QString& file, int type) : Icon(file, IconType(type)) {}

}  // namespace Style::internal
