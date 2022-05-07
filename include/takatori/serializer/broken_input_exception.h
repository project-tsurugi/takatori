#pragma once

#include <stdexcept>

namespace takatori::serializer {

/**
 * @brief an exception occurs when the value_input reached broken input.
 */
class broken_input_exception : public std::runtime_error {
public:
    /**
     * @brief the reason code of individual erroneous situations.
     */
    enum class reason_code {
        /// @brief reached end of buffer before reading value is completed.
        buffer_underflow,
    };

    /**
     * @brief creates a new instance.
     * @param reason the reason code
     * @param message the error message
     */
    explicit broken_input_exception(reason_code reason, char const* message);

    /**
     * @brief creates a new instance.
     * @param reason the reason code
     * @param message the error message
     */
    explicit broken_input_exception(reason_code reason, std::string const& message);

    /**
     * @brief returns the reason code of this exception.
     * @return the reason code
     */
    [[nodiscard]] reason_code reason() const noexcept;

private:
    reason_code reason_;
};

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
constexpr std::string_view to_string_view(broken_input_exception::reason_code value) noexcept {
    using namespace std::string_view_literals;
    using kind = broken_input_exception::reason_code;
    switch (value) {
        case kind::buffer_underflow: return "buffer_underflow"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
std::ostream& operator<<(std::ostream& out, broken_input_exception::reason_code value);

} // namespace takatori::serializer
