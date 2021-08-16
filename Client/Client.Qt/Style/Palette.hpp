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

/**
 * @class Palette
 * @brief Class for holding application color palette
 */
class Palette
{
public:
    /// Constructor for palette
    Palette() = default;
    /// Remove copy constructor
    Palette(const Palette& other) = delete;

    /// TODO: implement this
    QByteArray save() const;
    /// TODO: implement this
    bool load(const QByteArray& cache);

    /// Set result
    enum class SetResult
    {
        Ok,
        KeyNotFound,
        ValueNotFound,
        Duplicate,
    };
    /// Reset palette
    void reset()
    {
        clear();
        finalize();
    }

    /// Created not inited, should be finalized before usage.
    void finalize();

    /// Index of color
    int indexOfColor(color c) const;
    /// Color at index
    color colorAtIndex(int index) const;

    /// Transparent color
    inline const color& get_transparent() const { return _colors[0]; };
    /// windowActiveTextFg
    [[nodiscard]] inline const color& get_windowActiveTextFg() const { return _colors[1]; };
    /// window
    [[nodiscard]] inline const color& get_window() const { return _colors[2]; };
    /// windowColor
    [[nodiscard]] inline const color& get_windowColor() const { return _colors[3]; };
    /// windowColorOver
    [[nodiscard]] inline const color& get_windowColorOver() const { return _colors[4]; };
    /// titleButton
    [[nodiscard]] inline const color& get_titleButton() const { return _colors[5]; };
    /// titleButtonOver
    [[nodiscard]] inline const color& get_titleButtonOver() const { return _colors[6]; };
    /// closeButtonC
    [[nodiscard]] inline const color& get_closeButtonC() const { return _colors[7]; };
    /// closeButtonCOver
    [[nodiscard]] inline const color& get_closeButtonCOver() const { return _colors[8]; };
    /// linkButtonColor
    [[nodiscard]] inline const color& get_linkButtonColor() const { return _colors[9]; };
    /// linkButtonOverColor
    [[nodiscard]] inline const color& get_linkButtonOverColor() const { return _colors[10]; };
    /// scrollArea
    [[nodiscard]] inline const color& get_scrollArea() const { return _colors[11]; };
    /// scrollAreaColor
    [[nodiscard]] inline const color& get_scrollAreaColor() const { return _colors[12]; };
    /// scrollAreaBar
    [[nodiscard]] inline const color& get_scrollAreaBar() const { return _colors[13]; };
    /// scrollAreaColorOver
    [[nodiscard]] inline const color& get_scrollAreaColorOver() const { return _colors[14]; };
    /// scrollAreaBarOver
    [[nodiscard]] inline const color& get_scrollAreaBarOver() const { return _colors[15]; };
    /// spinnerLineColor
    [[nodiscard]] inline const color& get_spinnerLineColor() const { return _colors[16]; };

    /// Assign operator for Palette
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

    /// Destructor for palette
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
