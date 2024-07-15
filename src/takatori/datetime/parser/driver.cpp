#include "driver.h"

#include <charconv>

namespace takatori::datetime::parser {

void driver::success(success_type result) {
    result_ = result;
}

void driver::error(error_type error) {
    result_ = std::move(error);
}

driver::result_type& driver::result() noexcept {
    return result_;
}

driver::result_type const& driver::result() const noexcept {
    return result_;
}

std::optional<std::uint32_t> driver::parse_integer(std::string const& token) {
    std::uint32_t value {};
    auto const* begin = &*token.begin();
    auto const* end = &*token.end();
    auto result = std::from_chars(begin, end, value);
    if (result.ptr != end) {
        return {};
    }
    return value;
}

std::optional<std::pair<std::uint32_t, std::uint32_t>> driver::parse_decimal(std::string const& token) {
    auto dotAt = token.find('.');
    if (dotAt == std::string::npos) {
        return {};
    }
    std::uint32_t integral_part {};
    {
        auto const* begin = &*token.begin();
        auto const* end = &*(token.begin() + static_cast<std::string::difference_type>(dotAt));
        auto result = std::from_chars(begin, end, integral_part);
        if (result.ptr != end) {
            return {};
        }
    }
    std::uint32_t decimal_part {};
    {
        const std::size_t max_decimal_part_size = 9;
        auto decimal_part_size = token.size() - dotAt - 1;
        if (decimal_part_size > max_decimal_part_size) {
            return {};
        }
        if (decimal_part_size == 0) { // empty decimal part
            decimal_part = 0;
        } else {
            auto const* begin = &*(token.begin() + static_cast<std::string::difference_type>(dotAt + 1));
            auto const* end = &*token.end();
            auto result = std::from_chars(begin, end, decimal_part);
            if (result.ptr != end) {
                return {};
            }
            // adjust scale of decimal part
            for (std::size_t index = decimal_part_size; index < max_decimal_part_size; ++index) {
                decimal_part *= 10;
            }
        }
    }
    return std::make_pair(integral_part, decimal_part);
}

} // namespace takatori::datetime::parser
