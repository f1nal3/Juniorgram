#pragma once

#include <QFont>
#include <QFontMetrics>

namespace Style::internal
{
/// Initialize all fonts
void StartFonts();
/// Get other version of Font
[[nodiscard]] QString GetFontOverride(int32_t flags = 0);

/// Destroys fonts
void destroyFonts();
/// Registers font
int registerFontFamily(const QString& family);

/**
 * @brief Get a font with specified parameters
 * @param flags Font flags
 * @param size Pixel size
 * @return Requested font
 */
[[nodiscard]] QFont ResolveFont(uint32_t flags, int size);
class FontData;

/**
 * @class Font
 * @brief Pointer to font data
 */
class Font
{
public:
    /// Empty initialization
    Font(Qt::Initialization = Qt::Uninitialized) : ptr(0) {}
    /// Create font from family with specified flags
    Font(int size, uint32_t flags, const QString& family);
    /// Create font from registered family with specified flags
    Font(int size, uint32_t flags, int family);

    /// Refer to data
    FontData* operator->() const { return ptr; }
    /// Refer to data
    FontData* v() const { return ptr; }

    /// Does data exist
    explicit operator bool() const { return !!ptr; }

    /// Convert to QFont
    explicit operator const QFont&() const;

private:
    FontData* ptr;

    void        init(int size, uint32_t flags, int family, Font* modified);
    friend void startManager();

    Font(FontData* p) : ptr(p) {}
    Font(int size, uint32_t flags, int family, Font* modified);
    friend class FontData;
};

/// Font flags
enum FontFlags
{
    FontBold      = 0x01,
    FontItalic    = 0x02,
    FontUnderline = 0x04,
    FontStrikeOut = 0x08,
    FontSemibold  = 0x10,
    FontMonospace = 0x20,

    FontDifferentFlags = 0x40,
};

/**
 * @class Font data
 * @brief Holds font data
 */
class FontData
{
public:
    /**
     * @brief Get width of string
     * @param str String
     * @return Width in pixels
     */
    int width(const QString& str) const { return m.horizontalAdvance(str); }
    /**
     * @brief Get width of part of a string
     * @param str String
     * @param from First element
     * @param to End element
     * @return Width in pixels
     */
    int width(const QString& str, int32_t from, int32_t to) const { return width(str.mid(from, to)); }
    /**
     * @brief Get width of a char
     * @param ch Char
     * @return Width in pixels
     */
    int     width(QChar ch) const { return m.horizontalAdvance(ch); }
    QString elided(const QString& str, int width, Qt::TextElideMode mode = Qt::ElideRight) const { return m.elidedText(str, mode, width); }

    /// Get bold version of font
    Font bold(bool set = true) const;
    /// Get italic version of font
    Font italic(bool set = true) const;
    /// Get underline version of font
    Font underline(bool set = true) const;
    /// Get striked version of font
    Font strikeout(bool set = true) const;
    /// Get semibold version of font
    Font semibold(bool set = true) const;
    /// Get monospaced version of font
    Font monospace(bool set = true) const;

    /// Pixel size
    int size() const;
    /// Font flags
    uint32_t flags() const;
    /// Font family in base
    int family() const;

    /// QFont for qt
    QFont f;
    /// Font metrics for measure
    QFontMetrics m;
    /// Font info
    int32_t height, ascent, descent, spacew, elidew;

private:
    mutable Font modified[FontDifferentFlags];

    Font otherFlagsFont(uint32_t flag, bool set) const;
    FontData(int size, uint32_t flags, int family, Font* other);

    friend class Font;
    int      _size;
    uint32_t _flags;
    int      _family;
};

/// Compare on sameness
inline bool operator==(const Font& a, const Font& b) { return a.v() == b.v(); }
/// Compare to not sameness
inline bool operator!=(const Font& a, const Font& b) { return a.v() != b.v(); }

inline Font::operator const QFont&() const { return ptr->f; }

}  // namespace Style::internal
