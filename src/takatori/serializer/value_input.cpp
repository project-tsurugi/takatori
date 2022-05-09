#include <takatori/serializer/value_input.h>

#include <optional>
#include <stdexcept>

#include "base128v.h"
#include "details/value_io_constants.h"

#include <takatori/util/assertion.h>
#include <takatori/util/exception.h>
#include <takatori/util/fail.h>
#include <takatori/util/string_builder.h>

namespace takatori::serializer {

using namespace details;

using util::buffer_view;
using util::const_buffer_view;
using util::const_bitset_view;

using byte_type = buffer_view::value_type;

using decimal_type = ::fpdecimal::Decimal;

[[nodiscard]] static bool is_little_endian() {
    // NOTE: C++17 does not contain std::endian
    std::array<char, sizeof(std::int32_t)> bytes {};
    std::int32_t value = 1;
    std::memcpy(&bytes, &value, bytes.size());
    return bytes[0] != 0;
}

static void requires_entry(
        entry_type expect,
        buffer_view::const_iterator position,
        buffer_view::const_iterator end) {
    if (auto ret = peek_type(position, end); ret != expect) {
        using ::takatori::util::string_builder;
        using ::takatori::util::throw_exception;
        throw_exception(std::runtime_error(string_builder {}
                << "inconsistent entry type: "
                << "retrieved '" << ret << "', "
                << "but expected is '" << expect << "'"
                << string_builder::to_string));
    }
}

template<class T>
static std::optional<T> extract(
        buffer_view::value_type first,
        std::uint32_t header,
        std::uint32_t mask,
        T min_value) {
    auto unsigned_value = static_cast<unsigned char>(first);
    if (header <= unsigned_value && unsigned_value <~ header + mask) {
        return { static_cast<T>(unsigned_value - header) + min_value };
    }
    return std::nullopt;
}

static std::int64_t read_sint(buffer_view::const_iterator& position, buffer_view::const_iterator end) {
    if (auto result = base128v::read_signed(position, end)) {
        return *result;
    }
    throw_buffer_underflow();
}

static std::uint64_t read_uint(buffer_view::const_iterator& position, buffer_view::const_iterator end) {
    if (auto result = base128v::read_unsigned(position, end)) {
        return *result;
    }
    throw_buffer_underflow();
}

static std::uint32_t read_size(buffer_view::const_iterator& position, buffer_view::const_iterator end) {
    auto size = read_uint(position, end);
    if (size >= limit_size) {
        throw_size_out_of_range(size, limit_size);
    }
    return static_cast<std::uint32_t>(size);
}

static const_buffer_view read_bytes(
        std::size_t size,
        buffer_view::const_iterator& position,
        buffer_view::const_iterator end) {
    if (std::distance(position, end) < static_cast<buffer_view::difference_type>(size)) {
        throw_buffer_underflow();
    }
    const_buffer_view result { position, size };
    position += size;
    return result;
}

template<class T>
static T read_fixed(buffer_view::const_iterator& position, buffer_view::const_iterator end) {
    static_assert(std::is_integral_v<T>);
    static_assert(std::is_unsigned_v<T>);
    if (std::distance(position, end) < static_cast<buffer_view::difference_type>(sizeof(T))) {
        throw_buffer_underflow();
    }
    T result { 0 };
    for (std::size_t i = 0; i < sizeof(T); ++i) {
        T value { static_cast<unsigned char>(*position) };
        result |= value << (i * 8U);
        ++position;
    }
    return result;
}

void read_end_of_contents(buffer_view::const_iterator& position, buffer_view::const_iterator end) {
    requires_entry(entry_type::end_of_contents, position, end);
    ++position;
}

void read_null(buffer_view::const_iterator& position, buffer_view::const_iterator end) {
    requires_entry(entry_type::null, position, end);
    ++position;
}

entry_type peek_type(buffer_view::const_iterator position, buffer_view::const_iterator end) {
    if (position == end) {
        throw_buffer_underflow();
    }
    std::uint32_t head = static_cast<unsigned char>(*position);
    if (head <= header_embed_positive_int + mask_embed_positive_int) {
        return entry_type::int_;
    }
    static_assert(header_embed_positive_int + mask_embed_positive_int + 1 == header_embed_character);
    if (head <= header_embed_character + mask_embed_character) {
        return entry_type::character;
    }
    static_assert(header_embed_character + mask_embed_character + 1 == header_embed_row);
    if (head <= header_embed_row + mask_embed_row) {
        return entry_type::row;
    }
    static_assert(header_embed_row + mask_embed_row + 1 == header_embed_array);
    if (head <= header_embed_array + mask_embed_array) {
        return entry_type::array;
    }
    static_assert(header_embed_array + mask_embed_array + 1 == header_embed_negative_int);
    if (head <= header_embed_negative_int + mask_embed_negative_int) {
        return entry_type::int_;
    }
    static_assert(header_embed_negative_int + mask_embed_negative_int + 1 == header_embed_octet);
    if (head <= header_embed_octet + mask_embed_octet) {
        return entry_type::octet;
    }
    static_assert(header_embed_bit + mask_embed_bit + 1 == header_unknown);
    if (head <= header_embed_bit + mask_embed_bit) {
        return entry_type::bit;
    }
    switch (head) {
        case header_int: return entry_type::int_;
        case header_float4: return entry_type::float4;
        case header_float8: return entry_type::float8;
        case header_decimal16: return entry_type::decimal;
        case header_character: return entry_type::character;
        case header_octet: return entry_type::octet;
        case header_bit: return entry_type::bit;
        case header_date: return entry_type::date;
        case header_time_of_day: return entry_type::time_of_day;
        case header_time_point: return entry_type::time_point;
        case header_datetime_interval: return entry_type::datetime_interval;
        case header_row: return entry_type::row;
        case header_array: return entry_type::array;
        case header_clob: return entry_type::clob;
        case header_blob: return entry_type::blob;
        case header_end_of_contents: return entry_type::end_of_contents;

        case header_decimal4:
        case header_decimal8:
            throw_unsupported_entry(static_cast<unsigned char>(head));

        default:
            throw_unrecognized_entry(static_cast<unsigned char>(head));
    }
}

std::int64_t read_int(buffer_view::const_iterator& position, buffer_view::const_iterator end) {
    requires_entry(entry_type::int_, position, end);
    buffer_view::const_iterator iter = position;
    auto first = *iter;
    if (auto value = extract(
            first,
            header_embed_positive_int,
            mask_embed_positive_int,
            min_embed_positive_int_value)) {
        position = iter;
        return *value;
    }
    if (auto value = extract(
            first,
            header_embed_negative_int,
            mask_embed_negative_int,
            min_embed_negative_int_value)) {
        position = iter;
        return *value;
    }

    BOOST_ASSERT(static_cast<unsigned char>(first) == header_int); // NOLINT
    ++iter;
    auto result = read_sint(iter, end);
    position = iter;
    return result;
}

float peek_float4(buffer_view::const_iterator& position, buffer_view::const_iterator end) {
    requires_entry(entry_type::float4, position, end);
    buffer_view::const_iterator iter = position;
    ++iter;

    auto bits = read_fixed<std::uint32_t>(position, end);
    float result {};
    std::memcpy(&result, &bits, sizeof(result));
    position = iter;
    return result;
}

double read_float8(buffer_view::const_iterator& position, buffer_view::const_iterator end) {
    requires_entry(entry_type::float8, position, end);
    buffer_view::const_iterator iter = position;
    ++iter;

    auto bits = read_fixed<std::uint64_t>(position, end);
    double result {};
    std::memcpy(&result, &bits, sizeof(result));
    position = iter;
    return result;
}

decimal_type read_decimal(buffer_view::const_iterator& position, buffer_view::const_iterator end) {
    // int encoded
    if (peek_type(position, end) == entry_type::int_) {
        auto value = read_int(position, end);
        return decimal_type { value };
    }

    // decimal encoded
    requires_entry(entry_type::decimal, position, end);
    buffer_view::const_iterator iter = position;
    ++iter;

    decQuad result {};
    if (is_little_endian()) {
        for (std::size_t i = 0; i < 4; ++i) {
            result.words[4 - i - 1] = read_fixed<std::uint32_t>(iter, end); // NOLINT
        }
    } else {
        for (auto&& word : result.words) { // NOLINT
            word = read_fixed<std::uint32_t>(iter, end);
        }
    }
    position = iter;
    return decimal_type { result };
}

std::string_view read_character(buffer_view::const_iterator& position, buffer_view::const_iterator end) {
    requires_entry(entry_type::character, position, end);
    buffer_view::const_iterator iter = position;
    std::size_t size {};
    auto first = *iter;
    if (auto value = extract(
            first,
            header_embed_character,
            mask_embed_character,
            min_embed_character_size)) {
        size = *value;
    } else {
        BOOST_ASSERT(static_cast<unsigned char>(first) == header_character); // NOLINT
        ++iter;
        size = read_size(iter, end);
    }

    auto result = read_bytes(size, iter, end);
    position = iter;
    return { result.data(), result.size() };
}

std::string_view read_octet(buffer_view::const_iterator& position, buffer_view::const_iterator end) {
    requires_entry(entry_type::octet, position, end);
    buffer_view::const_iterator iter = position;
    std::size_t size {};
    auto first = *iter;
    if (auto value = extract(
            first,
            header_embed_octet,
            mask_embed_octet,
            min_embed_octet_size)) {
        size = *value;
    } else {
        BOOST_ASSERT(static_cast<unsigned char>(first) == header_octet); // NOLINT
        ++iter;
        size = read_size(iter, end);
    }

    auto result = read_bytes(size, iter, end);
    position = iter;
    return { result.data(), result.size() };
}

const_bitset_view read_bit(buffer_view::const_iterator& position, buffer_view::const_iterator end) {
    requires_entry(entry_type::bit, position, end);
    buffer_view::const_iterator iter = position;
    std::size_t size {};
    auto first = *iter;
    if (auto value = extract(
            first,
            header_embed_bit,
            mask_embed_bit,
            min_embed_bit_size)) {
        size = *value;
    } else {
        BOOST_ASSERT(static_cast<unsigned char>(first) == header_bit); // NOLINT
        ++iter;
        size = read_size(iter, end);
    }

    std::size_t block_size = (size + 7) / 8;
    auto result = read_bytes(block_size, iter, end);
    position = iter;
    return { result.data(), size };
}

// FIXME: impl temporal

std::size_t read_array_begin(buffer_view::const_iterator& position, buffer_view::const_iterator end) {
    requires_entry(entry_type::array, position, end);
    buffer_view::const_iterator iter = position;
    auto first = *iter;
    if (auto size = extract(
            first,
            header_embed_array,
            mask_embed_array,
            min_embed_array_size)) {
        return *size;
    }

    BOOST_ASSERT(static_cast<unsigned char>(first) == header_array); // NOLINT
    ++iter;
    auto size = read_size(iter, end);
    position = iter;
    return size;
}

std::size_t read_row_begin(buffer_view::const_iterator& position, buffer_view::const_iterator end) {
    requires_entry(entry_type::row, position, end);
    buffer_view::const_iterator iter = position;
    auto first = *iter;
    if (auto size = extract(
            first,
            header_embed_row,
            mask_embed_row,
            min_embed_row_size)) {
        return *size;
    }

    BOOST_ASSERT(static_cast<unsigned char>(first) == header_row); // NOLINT
    ++iter;
    auto size = read_size(iter, end);
    position = iter;
    return size;
}

// FIXME: impl blob, clob

} // namespace takatori::serializer
