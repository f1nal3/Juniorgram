// WARNING! All changes made in this file will be lost!
// This file was created automatically
#pragma once

#include "Style/Style.hpp"

namespace Style
{
namespace internal
{
/// Palette initializer
void init_Palette(int scale);

}  // namespace internal

class Palette
{
public:
    Palette()                     = default;
    Palette(const Palette& other) = delete;

    QByteArray save() const;
    bool       load(const QByteArray& cache);

    enum class SetResult
    {
        Ok,
        KeyNotFound,
        ValueNotFound,
        Duplicate,
    };
    SetResult setColor(QLatin1String name, uchar r, uchar g, uchar b, uchar a);
    SetResult setColor(QLatin1String name, QLatin1String from);
    void      reset()
    {
        clear();
        finalize();
    }

    // Created not inited, should be finalized before usage.
    void finalize();

    int   indexOfColor(color c) const;
    color colorAtIndex(int index) const;

    inline const color&               get_transparent() const { return _colors[0]; };  // special color
    [[nodiscard]] inline const color& get_windowActiveTextFg() const { return _colors[1]; };
    [[nodiscard]] inline const color& get_window() const { return _colors[2]; };
    [[nodiscard]] inline const color& get_windowColor() const { return _colors[3]; };
    [[nodiscard]] inline const color& get_windowColorOver() const { return _colors[4]; };
    [[nodiscard]] inline const color& get_titleButton() const { return _colors[5]; };
    [[nodiscard]] inline const color& get_titleButtonOver() const { return _colors[6]; };
    [[nodiscard]] inline const color& get_closeButtonC() const { return _colors[7]; };
    [[nodiscard]] inline const color& get_closeButtonCOver() const { return _colors[8]; };
    [[nodiscard]] inline const color& get_linkButtonColor() const { return _colors[9]; };
    [[nodiscard]] inline const color& get_linkButtonOverColor() const { return _colors[10]; };
    [[nodiscard]] inline const color& get_scrollArea() const { return _colors[11]; };
    [[nodiscard]] inline const color& get_scrollAreaColor() const { return _colors[12]; };
    [[nodiscard]] inline const color& get_scrollAreaBar() const { return _colors[13]; };
    [[nodiscard]] inline const color& get_scrollAreaColorOver() const { return _colors[14]; };
    [[nodiscard]] inline const color& get_scrollAreaBarOver() const { return _colors[15]; };
    [[nodiscard]] inline const color& get_spinnerLineColor() const { return _colors[16]; };

    Palette& operator=(const Palette& other)
    {
        auto wasReady = _ready;
        for (int i = 0; i != kCount; ++i)
        {
            if (other._status[i] == Status::Loaded)
            {
                if (_status[i] == Status::Initial)
                {
                    new (data(i)) internal::ColorData(*other.data(i));
                }
                else
                {
                    *data(i) = *other.data(i);
                }
            }
            else if (_status[i] != Status::Initial)
            {
                data(i)->~ColorData();
                _status[i] = Status::Initial;
                _ready     = false;
            }
        }
        if (wasReady && !_ready)
        {
            finalize();
        }
        return *this;
    }

    ~Palette() { clear(); }

private:
    static constexpr auto kCount = 17;

    void clear()
    {
        for (int i = 0; i != kCount; ++i)
        {
            if (_status[i] != Status::Initial)
            {
                data(i)->~ColorData();
                _status[i] = Status::Initial;
                _ready     = false;
            }
        }
    }

    struct TempColorData
    {
        uchar r, g, b, a;
    };
    void compute(int index, int fallbackIndex, TempColorData value)
    {
        if (_status[index] == Status::Initial)
        {
            if (fallbackIndex >= 0 && _status[fallbackIndex] == Status::Loaded)
            {
                _status[index] = Status::Loaded;
                new (data(index)) internal::ColorData(*data(fallbackIndex));
            }
            else
            {
                _status[index] = Status::Created;
                new (data(index)) internal::ColorData(value.r, value.g, value.b, value.a);
            }
        }
    }

    internal::ColorData* data(int index) { return reinterpret_cast<internal::ColorData*>(_data) + index; }

    const internal::ColorData* data(int index) const { return reinterpret_cast<const internal::ColorData*>(_data) + index; }

    void setData(int index, const internal::ColorData& value)
    {
        if (_status[index] == Status::Initial)
        {
            new (data(index)) internal::ColorData(value);
        }
        else
        {
            *data(index) = value;
        }
        _status[index] = Status::Loaded;
    }

    enum class Status
    {
        Initial,
        Created,
        Loaded,
    };

    alignas(alignof(internal::ColorData)) char _data[sizeof(internal::ColorData) * kCount];

    color _colors[kCount] = {
        data(0), data(1),  data(2),  data(3),  data(4),  data(5),  data(6),  data(7),  data(8),
        data(9), data(10), data(11), data(12), data(13), data(14), data(15), data(16),
    };
    Status _status[kCount] = {Status::Initial};
    bool   _ready          = false;
};

namespace MainPalette
{
QByteArray         save();
bool               load(const QByteArray& cache);
Palette::SetResult setColor(QLatin1String name, uchar r, uchar g, uchar b, uchar a);
Palette::SetResult setColor(QLatin1String name, QLatin1String from);
void               apply(const Palette& other);
void               reset();
int                indexOfColor(color c);

struct row
{
    QLatin1String name;
    QLatin1String value;
    QLatin1String fallback;
    QLatin1String description;
};
QList<row> data();

}  // namespace MainPalette
}  // namespace Style

namespace st
{
extern const Style::color& transparent;  // special color
/// windowActiveTextFg: Style::color
extern const Style::color& windowActiveTextFg;
/// window: Style::color
extern const Style::color& window;
/// windowColor: Style::color
extern const Style::color& windowColor;
/// windowColorOver: Style::color
extern const Style::color& windowColorOver;
/// titleButton: Style::color
extern const Style::color& titleButton;
/// titleButtonOver: Style::color
extern const Style::color& titleButtonOver;
/// closeButtonC: Style::color
extern const Style::color& closeButtonC;
/// closeButtonCOver: Style::color
extern const Style::color& closeButtonCOver;
/// linkButtonColor: Style::color
extern const Style::color& linkButtonColor;
/// linkButtonOverColor: Style::color
extern const Style::color& linkButtonOverColor;
/// scrollArea: Style::color
extern const Style::color& scrollArea;
/// scrollAreaColor: Style::color
extern const Style::color& scrollAreaColor;
/// scrollAreaBar: Style::color
extern const Style::color& scrollAreaBar;
/// scrollAreaColorOver: Style::color
extern const Style::color& scrollAreaColorOver;
/// scrollAreaBarOver: Style::color
extern const Style::color& scrollAreaBarOver;
/// spinnerLineColor: Style::color
extern const Style::color& spinnerLineColor;
}  // namespace st
