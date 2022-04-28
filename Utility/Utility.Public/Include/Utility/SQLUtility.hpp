#pragma once
#include <string>

#include "Utility/WarningSuppression.hpp"

namespace Utility
{
/**
 * @enum DatabaseType
 * @brief Database type enum.
 */
enum class DatabaseType : std::uint8_t
{
    DB_POSTGRE,
    DB_LITE
};

/**
 * @enum SQLStatement
 * @brief SQL statements enum.
 */
enum class SQLStatement : std::uint8_t
{
    ST_SELECT,  /// SELECT statement.
    ST_INSERT,  /// INSERT statement.
    ST_UPDATE,  /// UPDATE statement.
    ST_DELETE   /// DELETE statement.
};

/**
 * @enumSQLJoinType
 * @brief SQL join types enum.
 */
enum class SQLJoinType : std::uint8_t
{
    J_INNER,  /// Standard inner SQL join.
    J_LEFT,   /// Left SQL join.
    J_RIGHT,  /// Right SQL join.
    J_FULL    /// Full SQL join.
};

template <typename T, typename... Args>
T* make_statement(Args&&... args)
{
    return new T(std::forward<Args>(args)...);
}

template <typename T>
struct is_char
    : public std::disjunction<std::is_same<char, typename std::decay_t<T>>, std::is_same<const char, typename std::decay_t<T>>,
                              std::is_same<wchar_t, typename std::decay_t<T>>, std::is_same<const wchar_t, typename std::decay_t<T>>>
{
};

template <typename T>
struct is_c_string
    : public std::disjunction<std::is_same<char*, typename std::decay_t<T>>, std::is_same<const char*, typename std::decay_t<T>>,
                              std::is_same<wchar_t*, typename std::decay_t<T>>, std::is_same<const wchar_t*, typename std::decay_t<T>>>
{
};

template <typename T>
struct is_std_string
    : public std::disjunction<
          std::is_same<std::string, typename std::decay_t<T>>, std::is_same<const std::string, typename std::decay_t<T>>,
          std::is_same<std::wstring, typename std::decay_t<T>>, std::is_same<const std::wstring, typename std::decay_t<T>>,
          std::is_same<std::string_view, typename std::decay_t<T>>, std::is_same<const std::string_view, typename std::decay_t<T>>,
          std::is_same<std::wstring_view, typename std::decay_t<T>>, std::is_same<const std::wstring_view, typename std::decay_t<T>>>
{
};

template <typename T>
struct is_string : public std::disjunction<is_c_string<T>, is_std_string<T>>
{
};

template <typename T>
struct is_char_or_string : public std::disjunction<is_char<T>, is_string<T>>
{
};

template <typename T>
inline constexpr bool is_char_v = is_char<T>::value;
template <typename T>
inline constexpr bool is_string_v = is_string<T>::value;
template <typename T>
inline constexpr bool is_char_or_string_v = is_char_or_string<T>::value;

suppressWarning(4505, "-Winit-self")
    /// Checking for inner quotes.
    [[maybe_unused]] static void innerQuotes(std::string& arg)
{
    std::size_t pos = arg.find('\'');
    while (pos != std::string::npos)
    {
        arg.insert(pos, "'");

        pos = arg.find('\'', pos + 2);
    }
}

/// Wrapping whole string with quotes.
[[maybe_unused]] static void wrapStringByQuotes(std::string& arg) { arg.insert(0, "'").push_back('\''); }
restoreWarning

    template <typename T>
    auto CheckForSQLSingleQuotesProblem(T&& arg)
{
    if constexpr (is_char_or_string_v<decltype(arg)>)
    {
        std::string tempStr;
        tempStr += std::forward<T>(arg);

        innerQuotes(tempStr);

        wrapStringByQuotes(tempStr);

        return tempStr;
    }
    else
    {
        return arg;
    }
}
}  /// namespace Utility
