#include <takatori/util/details/basic_bit_reference.h>

#include <gtest/gtest.h>

namespace takatori::util::details {

class basic_bit_reference_test : public ::testing::Test {};

static_assert(std::is_trivially_copyable_v<basic_bit_reference<char>>);
static_assert(std::is_trivially_destructible_v<basic_bit_reference<char>>);

static_assert(std::is_same_v<typename basic_bit_reference<char>::block_pointer, char*>);
static_assert(std::is_same_v<typename basic_bit_reference<unsigned char>::block_pointer, unsigned char*>);
static_assert(std::is_same_v<typename basic_bit_reference<char const>::block_pointer, char const*>);

static_assert(std::is_constructible_v<basic_bit_reference<char const>, basic_bit_reference<char>>);
static_assert(!std::is_constructible_v<basic_bit_reference<char>, basic_bit_reference<char const>>);

static_assert(std::is_assignable_v<basic_bit_reference<char>, bool>);

using bit_reference = basic_bit_reference<char>;

using const_bit_reference = basic_bit_reference<char const>;

TEST_F(basic_bit_reference_test, simple) {
    // 0b0100'0001
    char bits = 'A';

    {
        bit_reference ref { &bits, 0 };
        EXPECT_TRUE(ref); // 1

        ref = false;
        EXPECT_FALSE(ref);
    }
    {
        bit_reference ref { &bits, 1 };
        EXPECT_FALSE(ref); // 0

        ref = true;
        EXPECT_TRUE(ref);
    }
    {
        bit_reference ref { &bits, 2 };
        EXPECT_FALSE(ref); // 0

        ref = true;
        EXPECT_TRUE(ref);
    }
    {
        bit_reference ref { &bits, 3 };
        EXPECT_FALSE(ref); // 0

        ref = true;
        EXPECT_TRUE(ref);
    }
    {
        bit_reference ref { &bits, 4 };
        EXPECT_FALSE(ref); // 0

        ref = true;
        EXPECT_TRUE(ref);
    }
    {
        bit_reference ref { &bits, 5 };
        EXPECT_FALSE(ref); // 0

        ref = true;
        EXPECT_TRUE(ref);
    }
    {
        bit_reference ref { &bits, 6 };
        EXPECT_TRUE(ref); // 1

        ref = false;
        EXPECT_FALSE(ref);
    }
    {
        bit_reference ref { &bits, 7 };
        EXPECT_FALSE(ref); // 0

        ref = true;
        EXPECT_TRUE(ref);
    }

    EXPECT_EQ(bits, static_cast<char>(0b1011'1110));
}

TEST_F(basic_bit_reference_test, const_ref) {
    // 0b0100'0001
    char const bits = 'A';

    {
        const_bit_reference ref { &bits, 0 };
        EXPECT_TRUE(ref); // 1
    }
    {
        const_bit_reference ref { &bits, 1 };
        EXPECT_FALSE(ref); // 0
    }
    {
        const_bit_reference ref { &bits, 2 };
        EXPECT_FALSE(ref); // 0
    }
    {
        const_bit_reference ref { &bits, 3 };
        EXPECT_FALSE(ref); // 0
    }
    {
        const_bit_reference ref { &bits, 4 };
        EXPECT_FALSE(ref); // 0
    }
    {
        const_bit_reference ref { &bits, 5 };
        EXPECT_FALSE(ref); // 0
    }
    {
        const_bit_reference ref { &bits, 6 };
        EXPECT_TRUE(ref); // 1
    }
    {
        const_bit_reference ref { &bits, 7 };
        EXPECT_FALSE(ref); // 0
    }
}

TEST_F(basic_bit_reference_test, conversion) {
    char c { 'A' };
    basic_bit_reference<char> ref { &c };
    basic_bit_reference<char const> cref { ref };
    EXPECT_EQ(ref.block_data(), cref.block_data());
    EXPECT_EQ(ref.bit_position(), cref.bit_position());
}

TEST_F(basic_bit_reference_test, wide) {
    std::uint32_t bits = 0xffff;

    for (std::size_t i = 0; i < 16; ++i) {
        basic_bit_reference<std::uint32_t> ref { &bits, i };
        EXPECT_EQ(ref.block_data(), &bits);
        EXPECT_TRUE(ref);
    }

    for (std::size_t i = 16; i < 32; ++i) {
        basic_bit_reference<std::uint32_t> ref { &bits, i };
        EXPECT_EQ(ref.block_data(), &bits);
        EXPECT_FALSE(ref);
    }

    basic_bit_reference<std::uint32_t> ref { &bits, 32 };
    EXPECT_EQ(ref.block_data(), &bits + 1);
}

} // namespace takatori::util::details
