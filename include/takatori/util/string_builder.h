#pragma once

#include <ostream>
#include <sstream>
#include <string>

namespace takatori::util {

/**
 * @brief builds character strings using operator<<.
 * @example
 *      @code
 *      using sb = ::takatori::util::string_builder
 *      throw std::domain_error(sb {}
 *              << "error at "
 *              << some_value
 *              << sb::to_string);
 *      @endcode
 */
class string_builder {
public:
    /**
     * @brief an operator tag to return the built string.
     * @see to_string
     */
    struct to_string_t {
        /**
         * @brief creates a new instance.
         */
        explicit constexpr to_string_t() = default;
    };

    /**
     * @brief creates a new instance.
     */
    string_builder() = default;

    /// @brief an operator tag to return the built string.
    static constexpr to_string_t to_string {};

    /**
     * @brief appends string representation of the given value.
     * @tparam T the value type
     * @param value the target value
     */
    template<class T>
    void append(T&& value) {
        buffer_ << std::forward<T>(value);
    }

    /**
     * @brief returns the built string.
     * @return the built string
     */
    std::string str() const;

    /**
     * @brief returns the internal buffer.
     * @return the buffer
     */
    std::ostringstream& buffer() noexcept;

    /// @copydoc buffer()
    std::ostringstream const& buffer() const noexcept;

private:
    std::ostringstream buffer_;
};

/**
 * @brief appends string representation of the given value.
 * @tparam T the value type
 * @param builder the target string builder
 * @param value the target value
 * @return the string builder
 */
template<class T>
string_builder& operator<<(string_builder& builder, T&& value) {
    builder.append(std::forward<T>(value));
    return builder;
}

/// @copydoc operator<<(string_builder&, T&&)
template<class T>
string_builder&& operator<<(string_builder&& builder, T&& value) {
    builder.append(std::forward<T>(value));
    return std::move(builder);
}

/**
 * @brief appends string representation of the given C-style string.
 * @tparam N the array size
 * @param builder the target string builder
 * @param value the target value
 * @return the string builder
 */
template<std::size_t N>
string_builder& operator<<(string_builder& builder, char const (&value)[N]) { // NOLINT
    builder.append(&value[0]);
    return builder;
}

/// @copydoc operator<<(string_builder&, char const(&)[N])
template<std::size_t N>
string_builder&& operator<<(string_builder&& builder, char const (&value)[N]) { // NOLINT
    builder.append(&value[0]);
    return std::move(builder);
}

/**
 * @brief returns the built string.
 * @param builder the target string builder
 * @return the built string
 */
std::string operator<<(string_builder& builder, string_builder::to_string_t);

/// @copydoc operator<<(string_builder&, string_builder::to_string_t)
std::string operator<<(string_builder const& builder, string_builder::to_string_t);

/// @copydoc operator<<(string_builder&, string_builder::to_string_t)
std::string operator<<(string_builder&& builder, string_builder::to_string_t);

} // namespace takatori::util
