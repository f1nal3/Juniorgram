#include "StyleFont.hpp"

#include <QFontDatabase>
#include <QFontInfo>
#include <QMap>

namespace Style
{
namespace internal
{
namespace
{
QMap<QString, int>        fontFamilyMap;
QList<QString>            fontFamilies;
QMap<uint32_t, FontData*> fontsMap;

uint32_t fontKey(int size, uint32_t flags, int family) { return (((uint32_t(family) << 12) | uint32_t(size)) << 6) | flags; }

bool ValidateFont(const QString& familyName, int flags = 0)
{
    QFont checkFont(familyName);
    checkFont.setBold(flags & Style::internal::FontBold);
    checkFont.setItalic(flags & Style::internal::FontItalic);
    checkFont.setUnderline(flags & Style::internal::FontUnderline);
    auto realFamily = QFontInfo(checkFont).family();
    if (realFamily.trimmed().compare(familyName, Qt::CaseInsensitive))
    {
        return false;
    }

    auto metrics = QFontMetrics(checkFont);
    if (!metrics.height())
    {
        return false;
    }

    return true;
}

bool LoadCustomFont(const QString& filePath, const QString& familyName, int flags = 0)
{
    auto regularId = QFontDatabase::addApplicationFont(filePath);
    if (regularId < 0)
    {
        return false;
    }

    const auto found = [&] {
        for (auto& family : QFontDatabase::applicationFontFamilies(regularId))
        {
            if (!family.trimmed().compare(familyName, Qt::CaseInsensitive))
            {
                return true;
            }
        }
        return false;
    }();
    if (!found)
    {
        return false;
    }

    return ValidateFont(familyName, flags);
}
[[nodiscard]] QString SystemMonospaceFont()
{
    const auto type = QFontDatabase::FixedFont;
    return QFontDatabase::systemFont(type).family();
}

[[nodiscard]] QString ManualMonospaceFont()
{
    const auto kTryFirst = std::initializer_list<QString>{"Consolas", "Liberation Mono", "Menlo", "Courier"};
    for (const auto& family : kTryFirst)
    {
        const auto resolved = QFontInfo(QFont(family)).family();
        if (!resolved.trimmed().compare(family, Qt::CaseInsensitive))
        {
            return family;
        }
    }
    return QString();
}
bool Started = false;
}  // namespace

void StartFonts()
{
    if (Started)
    {
        return;
    }
    Started = true;

    LoadCustomFont(":fonts/NotoSans-Regular.ttf", "Noto Sans");
    LoadCustomFont(":fonts/NotoSans-Bold.ttf", "Noto Sans", FontBold);
    LoadCustomFont(":fonts/NotoSans-BoldItalic.ttf", "Noto Sans", FontBold | FontItalic);
    LoadCustomFont(":fonts/NotoSans-Italic.ttf", "Noto Sans", FontItalic);
}

QString MonospaceFont()
{
    static const auto family = [&]() -> QString {
        const auto manual = ManualMonospaceFont();
        const auto system = SystemMonospaceFont();

#if defined Q_OS_WIN || defined Q_OS_MAC
        // Prefer our monospace font.
        const auto useSystem = manual.isEmpty();
#else   // Q_OS_WIN || Q_OS_MAC
        // Prefer system monospace font.
        const auto metrics   = QFontMetrics(QFont(system));
        const auto useSystem = manual.isEmpty() || (metrics.horizontalAdvance(QChar('i')) == metrics.horizontalAdvance(QChar('W')));
#endif  // Q_OS_WIN || Q_OS_MAC
        return useSystem ? system : manual;
    }();

    return family;
}

struct CustomFont
{
    QString family;
    QString style;
};

auto BoldFont    = CustomFont();
auto RegularFont = CustomFont();

enum
{
    FontTypeRegular = 0,
    FontTypeRegularItalic,
    FontTypeBold,
    FontTypeBoldItalic,
    FontTypeSemibold,
    FontTypeSemiboldItalic,

    FontTypesCount,
};

QString Overrides[FontTypesCount];
QString GetPossibleEmptyOverride(int32_t flags)
{
    flags                 = flags & (FontBold | FontSemibold | FontItalic);
    int32_t flagsBold     = flags & (FontBold | FontItalic);
    int32_t flagsSemibold = flags & (FontSemibold | FontItalic);
    if (flagsSemibold == (FontSemibold | FontItalic))
    {
        return Overrides[FontTypeSemiboldItalic];
    }
    else if (flagsSemibold == FontSemibold)
    {
        return Overrides[FontTypeSemibold];
    }
    else if (flagsBold == (FontBold | FontItalic))
    {
        return Overrides[FontTypeBoldItalic];
    }
    else if (flagsBold == FontBold)
    {
        return Overrides[FontTypeBold];
    }
    else if (flags == FontItalic)
    {
        return Overrides[FontTypeRegularItalic];
    }
    else if (flags == 0)
    {
        return Overrides[FontTypeRegular];
    }
    return QString();
}

QString GetFontOverride(int32_t flags)
{
    const auto result = GetPossibleEmptyOverride(flags);
    return result.isEmpty() ? "Noto Sans" : result;
}

QFont ResolveFont(uint32_t flags, int size)
{
    static auto Database = QFontDatabase();

    const auto  bold      = ((flags & FontBold) || (flags & FontSemibold));
    const auto  italic    = (flags & FontItalic);
    const auto& custom    = bold ? BoldFont : RegularFont;
    const auto  useCustom = !custom.family.isEmpty();

    auto result = QFont();
    if (flags & FontMonospace)
    {
        result.setFamily(MonospaceFont());
    }
    else if (useCustom)
    {
        const auto sizes = Database.smoothSizes(custom.family, custom.style);
        const auto good  = sizes.isEmpty() ? Database.pointSizes(custom.family, custom.style) : sizes;
        const auto point = good.isEmpty() ? size : good.front();
        result           = Database.font(custom.family, custom.style, point);
    }
    else
    {
        result.setFamily(GetFontOverride(flags));
        if (bold)
        {
            result.setWeight(QFont::Bold);

            if (flags & FontItalic)
            {
                result.setStyleName("Semibold Italic");
            }
            else
            {
                result.setStyleName("Semibold");
            }
        }
    }
    if (italic)
    {
        result.setItalic(true);
    }

    result.setUnderline(flags & FontUnderline);
    result.setStrikeOut(flags & FontStrikeOut);
    result.setPixelSize(size);

    return result;
}

void destroyFonts()
{
    for (auto fontData : std::as_const(fontsMap))
    {
        delete fontData;
    }
    fontsMap.clear();
}

int registerFontFamily(const QString& family)
{
    auto result = fontFamilyMap.value(family, -1);
    if (result < 0)
    {
        result = fontFamilies.size();
        fontFamilyMap.insert(family, result);
        fontFamilies.append(family);
    }
    return result;
}

FontData::FontData(int size, uint32_t flags, int family, Font* other)
    : f(ResolveFont(flags, size)), m(f), _size(size), _flags(flags), _family(family)
{
    if (other)
    {
        memcpy(modified, other, sizeof(modified));
    }
    else
    {
        for (auto& item : modified) item = nullptr;
    }
    modified[_flags] = Font(this);

    height  = m.height();
    ascent  = m.ascent();
    descent = m.descent();
    spacew  = width(QLatin1Char(' '));
    elidew  = width("...");
}

Font FontData::bold(bool set) const { return otherFlagsFont(FontBold, set); }

Font FontData::italic(bool set) const { return otherFlagsFont(FontItalic, set); }

Font FontData::underline(bool set) const { return otherFlagsFont(FontUnderline, set); }

Font FontData::strikeout(bool set) const { return otherFlagsFont(FontStrikeOut, set); }

Font FontData::semibold(bool set) const { return otherFlagsFont(FontSemibold, set); }

Font FontData::monospace(bool set) const { return otherFlagsFont(FontMonospace, set); }

int FontData::size() const { return _size; }

uint32_t FontData::flags() const { return _flags; }

int FontData::family() const { return _family; }

Font FontData::otherFlagsFont(uint32_t flag, bool set) const
{
    uint32_t newFlags = set ? (_flags | flag) : (_flags & ~flag);
    if (!modified[newFlags].v())
    {
        modified[newFlags] = Font(_size, newFlags, _family, modified);
    }
    return modified[newFlags];
}

Font::Font(int size, uint32_t flags, const QString& family)
{
    if (fontFamilyMap.isEmpty())
    {
        for (uint32_t i = 0, s = fontFamilies.size(); i != s; ++i)
        {
            fontFamilyMap.insert(fontFamilies.at(i), i);
        }
    }

    auto i = fontFamilyMap.constFind(family);
    if (i == fontFamilyMap.cend())
    {
        fontFamilies.append(family);
        i = fontFamilyMap.insert(family, fontFamilies.size() - 1);
    }
    init(size, flags, i.value(), nullptr);
}

Font::Font(int size, uint32_t flags, int family) { init(size, flags, family, nullptr); }

Font::Font(int size, uint32_t flags, int family, Font* modified) { init(size, flags, family, modified); }

void Font::init(int size, uint32_t flags, int family, Font* modified)
{
    uint32_t key = fontKey(size, flags, family);
    auto     i   = fontsMap.constFind(key);
    if (i == fontsMap.cend())
    {
        i = fontsMap.insert(key, new FontData(size, flags, family, modified));
    }
    ptr = i.value();
}
}  // namespace internal
}  // namespace Style
