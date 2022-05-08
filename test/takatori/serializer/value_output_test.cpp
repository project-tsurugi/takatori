#include <takatori/serializer/value_output.h>

#include <gtest/gtest.h>

#include <functional>
#include <limits>
#include <stdexcept>
#include <string>

#include <takatori/serializer/base128v.h>
#include <takatori/serializer/details/value_io_constants.h>

namespace takatori::serializer {

using buffer = util::buffer_view;

using namespace details;

class value_output_test : public ::testing::Test {
public:
    static std::string bytes(std::initializer_list<std::uint8_t> values) {
        std::string results {};
        results.resize(values.size());
        std::size_t index { 0 };
        for (auto c : values) {
            results[index++] = static_cast<char>(c);
        }
        return results;
    }

    static std::string sequence(std::int64_t header, std::initializer_list<std::string> rest) {
        std::string results {};
        results.append(1, static_cast<char>(header));
        for (auto&& str : rest) {
            results.append(str);
        }
        return results;
    }

    static std::string perform(
            std::function<bool(buffer::iterator&, buffer::iterator)> const& action,
            std::size_t buffer_size = 64) {
        std::string results {};
        results.resize(buffer_size);
        buffer buf { results.data(), results.size() };
        buffer::iterator iter = buf.begin();
        if (!action(iter, buf.end())) {
            throw std::runtime_error("underflow");
        }
        results.resize(std::distance(buf.begin(), iter));
        return results;
    }

    static std::string sint(std::int64_t value) {
        return perform([=] (auto& iter, auto end) { return base128v::write_signed(value, iter, end); });
    }

    static std::string uint(std::uint64_t value) {
        return perform([=] (auto& iter, auto end) { return base128v::write_unsigned(value, iter, end); });
    }

    template<class T, class U>
    static std::string fixed(U value) {
        static_assert(sizeof(T) == sizeof(U));
        T bits {};
        std::memcpy(&bits, &value, sizeof(T));

        std::string result {};
        result.resize(sizeof(bits));
        for (std::size_t i = 0; i < sizeof(bits); ++i) {
            result[i] = static_cast<char>(bits >> ((sizeof(bits) - i - 1U) * 8U));
        }
        return result;
    }
};

TEST_F(value_output_test, write_null) {
    EXPECT_EQ(
            bytes({ header_unknown }),
            perform([](auto& iter, auto end) { return write_null(iter, end); }));
}

TEST_F(value_output_test, write_int_embed_positive) {
    EXPECT_EQ(
            bytes({ header_embed_positive_int + 0 }),
            perform([](auto& iter, auto end) { return write_int(0, iter, end); }));
    EXPECT_EQ(
            bytes({ header_embed_positive_int + 63 }),
            perform([](auto& iter, auto end) { return write_int(63, iter, end); }));
}

TEST_F(value_output_test, write_int_embed_negative) {
    EXPECT_EQ(
            bytes({ header_embed_negative_int + 0 }),
            perform([](auto& iter, auto end) { return write_int(-16, iter, end); }));
    EXPECT_EQ(
            bytes({ header_embed_negative_int + 15 }),
            perform([](auto& iter, auto end) { return write_int(-1, iter, end); }));
}

TEST_F(value_output_test, write_int_full) {
    EXPECT_EQ(
            sequence(header_int, { sint(64) }),
            perform([](auto& iter, auto end) { return write_int(64, iter, end); }));
    EXPECT_EQ(
            sequence(header_int, { sint(-17) }),
            perform([](auto& iter, auto end) { return write_int(-17, iter, end); }));
    EXPECT_EQ(
            sequence(header_int, { sint(+1'000) }),
            perform([](auto& iter, auto end) { return write_int(+1'000, iter, end); }));
    EXPECT_EQ(
            sequence(header_int, { sint(-1'000) }),
            perform([](auto& iter, auto end) { return write_int(-1'000, iter, end); }));
    EXPECT_EQ(
            sequence(header_int, { sint(std::numeric_limits<std::int64_t>::max()) }),
            perform([](auto& iter, auto end) { return write_int(std::numeric_limits<std::int64_t>::max(), iter, end); }));
    EXPECT_EQ(
            sequence(header_int, { sint(std::numeric_limits<std::int64_t>::min()) }),
            perform([](auto& iter, auto end) { return write_int(std::numeric_limits<std::int64_t>::min(), iter, end); }));
}

TEST_F(value_output_test, write_float4) {
    EXPECT_EQ(
            sequence(header_float4, { bytes({ 0x3f, 0xa0, 0x00, 0x00 }) }),
            perform([](auto& iter, auto end) { return write_float4(1.25F, iter, end); }));
    EXPECT_EQ(
            sequence(header_float4, { fixed<std::uint32_t>(3.14F) }),
            perform([](auto& iter, auto end) { return write_float4(3.14F, iter, end); }));
}

TEST_F(value_output_test, write_float8) {
    EXPECT_EQ(
            sequence(header_float8, { fixed<std::uint64_t>(3.14) }),
            perform([](auto& iter, auto end) { return write_float8(3.14, iter, end); }));
}

TEST_F(value_output_test, write_decimal_int) {
    EXPECT_EQ(
            perform([](auto& iter, auto end) { return write_int(0, iter, end); }),
            perform([](auto& iter, auto end) { return write_decimal(0, iter, end); }));
    EXPECT_EQ(
            perform([](auto& iter, auto end) { return write_int(std::numeric_limits<std::int64_t>::max(), iter, end); }),
            perform([](auto& iter, auto end) { return write_decimal(std::numeric_limits<std::int64_t>::max(), iter, end); }));
    EXPECT_EQ(
            perform([](auto& iter, auto end) { return write_int(std::numeric_limits<std::int64_t>::min(), iter, end); }),
            perform([](auto& iter, auto end) { return write_decimal(std::numeric_limits<std::int64_t>::min(), iter, end); }));
}

TEST_F(value_output_test, write_decimal_full) {
    EXPECT_EQ(
            sequence(header_decimal16, { bytes({
                0xa2, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x94,
            }) }),
            perform([](auto& iter, auto end) { return write_decimal("-3.14", iter, end); }));
}

} // namespace takatori::serializer
