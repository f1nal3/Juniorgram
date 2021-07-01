#pragma once

namespace Utility
{
    /** @enum Database type enum. */
    enum class DatabaseType : std::uint8_t
    {
        DB_POSTGRE,
        DB_LITE,
        DB_SQLCIPHER
    };
    /** @enum SQL statements enum. */
    enum class SQLStatement : std::uint8_t
    {
        ST_SELECT,   /// SELECT statement
        ST_INSERT,   /// INSERT statement
        ST_UPDATE,   /// UPDATE statement
        ST_DELETE    /// DELETE statement
    };
    /** @enum SQL join types enum. */
    enum class SQLJoinType : std::uint8_t
    {
        J_INNER,  /// Standart inner sql join
        J_LEFT,   /// Left sql join
        J_RIGHT,  /// Right sql join
        J_FULL    /// Full sql join
    };

    template <typename T, typename ...Args>
    T* make_statement(Args&& ...args)
    {
        return new T(std::forward<Args>(args)...);
    }
    
    template <typename T>
    struct is_char : public std::disjunction<std::is_same<char, typename std::decay_t<T>>,
                                             std::is_same<const char, typename std::decay_t<T>>,
                                             std::is_same<wchar_t, typename std::decay_t<T>>,
                                             std::is_same<const wchar_t, typename std::decay_t<T>>>
    {
    };
    
    template <typename T>
    struct is_c_string : public std::disjunction<std::is_same<char*, typename std::decay_t<T>>,
                                                 std::is_same<const char*, typename std::decay_t<T>>,
                                                 std::is_same<wchar_t*, typename std::decay_t<T>>,
                                                 std::is_same<const wchar_t*, typename std::decay_t<T>>>
    {
    };
    
    template <typename T>
    struct is_std_string
        : public std::disjunction<std::is_same<std::string, typename std::decay_t<T>>,
                                  std::is_same<const std::string, typename std::decay_t<T>>,
                                  std::is_same<std::wstring, typename std::decay_t<T>>,
                                  std::is_same<const std::wstring, typename std::decay_t<T>>,
                                  std::is_same<std::string_view, typename std::decay_t<T>>,
                                  std::is_same<const std::string_view, typename std::decay_t<T>>,
                                  std::is_same<std::wstring_view, typename std::decay_t<T>>,
                                  std::is_same<const std::wstring_view, typename std::decay_t<T>>>
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
    
    template <typename T>
    auto CheckForSQLSingleQuotesProblem(T&& arg)
    {
        if constexpr (is_char_or_string_v<decltype(arg)>)
        {
            std::string tempStr;
            tempStr += std::forward<T>(arg);
    
            // Checking for inner quotes.
            std::size_t pos = tempStr.find('\'');
            while (pos != std::string::npos)
            {
                tempStr.insert(pos, "'");
    
                pos = tempStr.find('\'', pos + 2);
            }
    
            // Wrapping whole string with quotes.
            tempStr.insert(0, "'").push_back('\'');
    
            return tempStr;
        }
        else
        {
            return arg;
        }
    }
}  // namespace DataAccess
