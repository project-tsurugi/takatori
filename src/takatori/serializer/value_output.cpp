#include <takatori/serializer/value_output.h>

#include <limits>
#include <stdexcept>

#include <cstdint>

#include "base128v.h"
#include "details/value_io_constants.h"

#include <takatori/util/assertion.h>
#include <takatori/util/exception.h>

namespace takatori::serializer {

using namespace details;

using util::buffer_view;
using util::const_bitset_view;
using util::throw_exception;

using byte_type = buffer_view::value_type;

using decimal_type = ::fpdecimal::Decimal;

[[nodiscard]] static std::size_t buffer_remaining(
        buffer_view::const_iterator position,
        buffer_view::const_iterator end) {
    auto diff = std::distance(position, end);
    if (diff < 0) {
        return 0;
    }
    return static_cast<std::size_t>(diff);
}

static void write_fixed8(
        std::uint64_t value,
        buffer_view::iterator& position,
        buffer_view::const_iterator end) {
    BOOST_ASSERT(position < end); // NOLINT
	(void) end;
    *position = static_cast<byte_type>(value);
    ++position;
}

template<class T>
static void write_fixed(
        T value,
        buffer_view::iterator& position,
        buffer_view::const_iterator end) {
    for (std::uint64_t i = 1; i <= sizeof(T); ++i) {
        write_fixed8(value >> ((sizeof(T) - i) * 8U), position, end);
    }
}

static void write_bytes(
        byte_type const* data,
        std::size_t count,
        buffer_view::iterator& position,
        buffer_view::const_iterator end) {
    BOOST_ASSERT(position + count <= end); // NOLINT
	(void) end;
    std::memcpy(position, data, count);
    position += count;
}

bool write_end_of_contents(
        buffer_view::iterator& position,
        buffer_view::const_iterator end) {
    if (buffer_remaining(position, end) < 1) {
        return false;
    }
    write_fixed8(header_end_of_contents, position, end);
    return true;
}

bool write_null(
        buffer_view::iterator& position,
        buffer_view::const_iterator end) {
    if (buffer_remaining(position, end) < 1) {
        return false;
    }
    write_fixed8(header_unknown, position, end);
    return true;
}

bool write_int(
        std::int64_t value,
        buffer_view::iterator& position,
        buffer_view::const_iterator end) {
    if (min_embed_positive_int_value <= value && value <= max_embed_positive_int_value) {
        // embed positive int
        if (buffer_remaining(position, end) < 1) {
            return false;
        }
        write_fixed8(
                static_cast<std::int64_t>(header_embed_positive_int) + value - min_embed_positive_int_value,
                position,
                end);
    } else if (min_embed_negative_int_value <= value && value <= max_embed_negative_int_value) {
        // embed negative int
        if (buffer_remaining(position, end) < 1) {
            return false;
        }
        write_fixed8(
                static_cast<std::int64_t>(header_embed_negative_int) + value - min_embed_negative_int_value,
                position,
                end);
    } else {
        // normal int
        if (buffer_remaining(position, end) < 1 + base128v::size_signed(value)) {
            return false;
        }

        write_fixed8(header_int, position, end);
        base128v::write_signed(value, position, end);
    }
    return true;
}

bool write_float4(
        float value,
        buffer_view::iterator& position,
        buffer_view::const_iterator end) {
    if (buffer_remaining(position, end) < 1 + 4) {
        return false;
    }
    std::uint32_t bits {};
    std::memcpy(&bits, &value, sizeof(bits));

    write_fixed8(header_float4, position, end);
    write_fixed<std::uint32_t>(bits, position, end);
    return true;
}

bool write_float8(
        double value,
        buffer_view::iterator& position,
        buffer_view::const_iterator end) {
    if (buffer_remaining(position, end) < 1 + 8) {
        return false;
    }
    std::uint64_t bits {};
    std::memcpy(&bits, &value, sizeof(bits));

    write_fixed8(header_float8, position, end);
    write_fixed<std::uint64_t>(bits, position, end);
    return true;
}

bool write_decimal(
        decimal_type value,
        buffer_view::iterator& position,
        buffer_view::const_iterator end) {
    if (value.exponent() == 0
            && std::numeric_limits<std::int64_t>::min() <= value
            && value <= std::numeric_limits<std::int64_t>::max()) {
        // write as int
        auto guard = decimal_type::escape_status();
        if (auto int_value = value.as<std::int64_t>()) {
            return write_int(int_value.value_or(0), position, end);
        }
        // fall-through if conversion to long is failed
    }
    {
        // write as normal decimal
        if (buffer_remaining(position, end) < 1 + 8 + 8) {
            return false;
        }
        auto bytes = value.to_bytes();
        write_fixed8(header_decimal16, position, end);
        write_bytes(
                reinterpret_cast<byte_type const*>(bytes.data()), // NOLINT
                bytes.size(),
                position,
                end);
        return true;
    }
}

bool write_character(
        std::string_view value,
        buffer_view::iterator& position,
        buffer_view::const_iterator end) {
    auto size = value.size();

    if (min_embed_character_size <= size && size <= max_embed_character_size) {
        // for short character string
        if (buffer_remaining(position, end) < 1 + size) {
            return false;
        }
        write_fixed8(
                static_cast<std::int64_t>(header_embed_character) + size - min_embed_character_size,
                position,
                end);
    } else {
        // for long character string
        if (buffer_remaining(position, end) < 1 + base128v::size_unsigned(size) + size) {
            return false;
        }
        write_fixed8(header_character, position, end);
        base128v::write_unsigned(size, position, end);
    }
    write_bytes(value.data(), value.size(), position, end);
    return true;
}

bool write_octet(
        std::string_view value,
        buffer_view::iterator& position,
        buffer_view::const_iterator end) {
    auto size = value.size();

    if (min_embed_octet_size <= size && size <= max_embed_octet_size) {
        // for short octet string
        if (buffer_remaining(position, end) < 1 + size) {
            return false;
        }
        write_fixed8(
                static_cast<std::int64_t>(header_embed_octet) + size - min_embed_octet_size,
                position,
                end);
    } else {
        // for long octet string
        if (buffer_remaining(position, end) < 1 + base128v::size_unsigned(size) + size) {
            return false;
        }
        write_fixed8(header_octet, position, end);
        base128v::write_unsigned(size, position, end);
    }
    write_bytes(value.data(), value.size(), position, end);
    return true;
}

bool write_bit(
        const_bitset_view value,
        buffer_view::iterator& position,
        buffer_view::const_iterator end) {
    auto bit_size = value.size();
    auto byte_size = value.block_size();

    if (min_embed_bit_size <= bit_size && bit_size <= max_embed_bit_size) {
        // for short bit string
        if (buffer_remaining(position, end) < 1 + byte_size) {
            return false;
        }
        write_fixed8(
                static_cast<std::int64_t>(header_embed_bit) + bit_size - min_embed_bit_size,
                position,
                end);
    } else {
        // for long bit string
        if (buffer_remaining(position, end) < 1 + base128v::size_unsigned(bit_size) + byte_size) {
            return false;
        }
        write_fixed8(header_bit, position, end);
        base128v::write_unsigned(bit_size, position, end);
    }
    auto rest_bits = value.size() % 8;
    if (rest_bits == 0) {
        // write all blocks
        write_bytes(value.block_data(), value.block_size(), position, end);
    } else {
        // write blocks except the last
        write_bytes(value.block_data(), value.block_size() - 1, position, end);

        auto last = static_cast<std::uint8_t>(*--value.end());
        write_fixed8(last & ~(0xffU << rest_bits), position, end);
    }
    return true;
}

bool write_bit(
        std::string_view blocks,
        std::size_t number_of_bits,
        buffer_view::iterator& position,
        buffer_view::const_iterator end) {
    if (number_of_bits > blocks.size() * 8) {
        throw_exception(std::out_of_range("too large number of bits"));
    }
    util::const_bitset_view bits { blocks.data(), number_of_bits };
    BOOST_ASSERT(bits.block_size() <= blocks.size()); // NOLINT
    return write_bit(bits, position, end);
}

bool write_date(
        datetime::date value,
        util::buffer_view::iterator& position,
        util::buffer_view::const_iterator end) {
    if (buffer_remaining(position, end) < 1 + base128v::size_signed(value.days_since_epoch())) {
        return false;
    }
    write_fixed8(header_date, position, end);
    base128v::write_signed(value.days_since_epoch(), position, end);
    return true;
}

bool write_time_of_day(
        datetime::time_of_day value,
        util::buffer_view::iterator& position,
        util::buffer_view::const_iterator end) {
    if (buffer_remaining(position, end) < 1 + base128v::size_unsigned(value.time_since_epoch().count())) {
        return false;
    }
    write_fixed8(header_time_of_day, position, end);
    base128v::write_unsigned(value.time_since_epoch().count(), position, end);
    return true;
}

bool write_time_point(
        datetime::time_point value,
        util::buffer_view::iterator& position,
        util::buffer_view::const_iterator end) {
    if (buffer_remaining(position, end) < 1
            + base128v::size_signed(value.seconds_since_epoch().count())
            + base128v::size_unsigned(value.subsecond().count())) {
        return false;
    }
    write_fixed8(header_time_point, position, end);
    base128v::write_signed(value.seconds_since_epoch().count(), position, end);
    base128v::write_unsigned(value.subsecond().count(), position, end);
    return true;
}

bool write_datetime_interval(
        datetime::datetime_interval value,
        util::buffer_view::iterator& position,
        util::buffer_view::const_iterator end) {
    if (buffer_remaining(position, end) < 1
            + base128v::size_signed(value.date().year())
            + base128v::size_signed(value.date().month())
            + base128v::size_signed(value.date().day())
            + base128v::size_signed(value.time().offset().count())) {
        return false;
    }
    write_fixed8(header_datetime_interval, position, end);
    base128v::write_signed(value.date().year(), position, end);
    base128v::write_signed(value.date().month(), position, end);
    base128v::write_signed(value.date().day(), position, end);
    base128v::write_signed(value.time().offset().count(), position, end);
    return true;
}

bool write_array_begin(
        std::size_t size,
        buffer_view::iterator& position,
        buffer_view::const_iterator end) {
    if (min_embed_array_size <= size && size <= max_embed_array_size) {
        // for short array
        if (buffer_remaining(position, end) < 1) {
            return false;
        }
        write_fixed8(
                static_cast<std::int64_t>(header_embed_array) + size - min_embed_array_size,
                position,
                end);
    } else {
        // for long array
        if (buffer_remaining(position, end) < 1 + base128v::size_unsigned(size)) {
            return false;
        }
        write_fixed8(header_array, position, end);
        base128v::write_unsigned(size, position, end);
    }
    return true;
}

bool write_row_begin(
        std::size_t size,
        buffer_view::iterator& position,
        buffer_view::const_iterator end) {
    if (min_embed_row_size <= size && size <= max_embed_row_size) {
        // for short row
        if (buffer_remaining(position, end) < 1) {
            return false;
        }
        write_fixed8(
                static_cast<std::int64_t>(header_embed_row) + size - min_embed_row_size,
                position,
                end);
    } else {
        // for long row
        if (buffer_remaining(position, end) < 1 + base128v::size_unsigned(size)) {
            return false;
        }
        write_fixed8(header_row, position, end);
        base128v::write_unsigned(size, position, end);
    }
    return true;
}

} // namespace takatori::serializer
