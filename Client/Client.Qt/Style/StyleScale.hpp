#pragma once
namespace Style
{
/// Minimal dpi factor
inline constexpr auto minDPI = 100;
/// Maximal dpi factor
inline constexpr auto maxDPI = 300;
/// Default dpi factor
inline constexpr auto defDPI = 100;

/**
 * @brief Current dpi scale
 * @return DPI scale in percent 100%(96dpi)
 */
[[nodiscard]] int getDpiScale();
/**
 * @brief Current device pixel ratio
 * @return DPR
 */
[[nodiscard]] float devicePixelRatio();

/**
 * @brief Set a new dpi factor
 * @param factor DPI in percent 100%(96dpi)
 */
void setDpiScale(int factor);

/**
 * @brief Set a new DPR
 * @param factor DPR
 */
void setDevicePixelRatio(float factor);

/**
 * @brief Converts pixel value to a current scale
 * @param value Pixel value
 * @return Scaled pixel value
 */
int valueDPIScale(int value);

/**
 * @brief Converts pixel value to a specified scale
 * @param value Pixel value
 * @param scale Scale factor
 * @return Scaled pixel value
 */
int ConvertScale(int value, int scale);
}  // namespace Style
