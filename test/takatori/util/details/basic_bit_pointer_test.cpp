#include <takatori/util/details/basic_bit_pointer.h>

#include <gtest/gtest.h>

namespace takatori::util::details {

class basic_bit_pointer_test : public ::testing::Test {};

static_assert(std::is_trivially_copyable_v<basic_bit_pointer<char>>);
static_assert(std::is_trivially_destructible_v<basic_bit_pointer<char>>);

static_assert(std::is_same_v<typename basic_bit_pointer<char>::block_pointer, char*>);
static_assert(std::is_same_v<typename basic_bit_pointer<unsigned char>::block_pointer, unsigned char*>);
static_assert(std::is_same_v<typename basic_bit_pointer<char const>::block_pointer, char const*>);

static_assert(std::is_constructible_v<basic_bit_pointer<char const>, basic_bit_pointer<char>>);
static_assert(!std::is_constructible_v<basic_bit_pointer<char>, basic_bit_pointer<char const>>);
static_assert(!std::is_constructible_v<basic_bit_pointer<char>, basic_bit_pointer<int>>);
static_assert(!std::is_constructible_v<basic_bit_pointer<unsigned char>, basic_bit_pointer<unsigned int>>);
static_assert(!std::is_constructible_v<basic_bit_pointer<unsigned int>, basic_bit_pointer<unsigned char>>);

using bit_pointer = basic_bit_pointer<char>;

TEST_F(basic_bit_pointer_test, increment) {
    // 0b0100'0001
    char bits = 'A';
    bit_pointer ptr { &bits };

    EXPECT_EQ(ptr.block_data(), &bits);
    EXPECT_EQ(ptr.bit_position(), 0);
    EXPECT_TRUE(*ptr++); // 1

    EXPECT_EQ(ptr.block_data(), &bits);
    EXPECT_EQ(ptr.bit_position(), 1);
    EXPECT_FALSE(*ptr++); // 0

    EXPECT_EQ(ptr.block_data(), &bits);
    EXPECT_EQ(ptr.bit_position(), 2);
    EXPECT_FALSE(*ptr++); // 0

    EXPECT_EQ(ptr.block_data(), &bits);
    EXPECT_EQ(ptr.bit_position(), 3);
    EXPECT_FALSE(*ptr++); // 0


    EXPECT_EQ(ptr.block_data(), &bits);
    EXPECT_EQ(ptr.bit_position(), 4);
    EXPECT_FALSE(*ptr++); // 0

    EXPECT_EQ(ptr.block_data(), &bits);
    EXPECT_EQ(ptr.bit_position(), 5);
    EXPECT_FALSE(*ptr++); // 0

    EXPECT_EQ(ptr.block_data(), &bits);
    EXPECT_EQ(ptr.bit_position(), 6);
    EXPECT_TRUE(*ptr++); // 1

    EXPECT_EQ(ptr.block_data(), &bits);
    EXPECT_EQ(ptr.bit_position(), 7);
    EXPECT_FALSE(*ptr++); // 0

    EXPECT_EQ(ptr.block_data(), &bits + 1);
    EXPECT_EQ(ptr.bit_position(), 0);
}

TEST_F(basic_bit_pointer_test, decrement) {
    // 0b0100'0001
    char bits = 'A';
    bit_pointer ptr { &bits, 8 };

    EXPECT_EQ(ptr.block_data(), &bits + 1);
    EXPECT_EQ(ptr.bit_position(), 0);

    EXPECT_FALSE(*--ptr); // 0
    EXPECT_EQ(ptr.block_data(), &bits);
    EXPECT_EQ(ptr.bit_position(), 7);

    EXPECT_TRUE(*--ptr); // 1
    EXPECT_EQ(ptr.block_data(), &bits);
    EXPECT_EQ(ptr.bit_position(), 6);

    EXPECT_FALSE(*--ptr); // 0
    EXPECT_EQ(ptr.block_data(), &bits);
    EXPECT_EQ(ptr.bit_position(), 5);

    EXPECT_FALSE(*--ptr); // 0
    EXPECT_EQ(ptr.block_data(), &bits);
    EXPECT_EQ(ptr.bit_position(), 4);


    EXPECT_FALSE(*--ptr); // 0
    EXPECT_EQ(ptr.block_data(), &bits);
    EXPECT_EQ(ptr.bit_position(), 3);

    EXPECT_FALSE(*--ptr); // 0
    EXPECT_EQ(ptr.block_data(), &bits);
    EXPECT_EQ(ptr.bit_position(), 2);

    EXPECT_FALSE(*--ptr); // 0
    EXPECT_EQ(ptr.block_data(), &bits);
    EXPECT_EQ(ptr.bit_position(), 1);

    EXPECT_TRUE(*--ptr); // 1
    EXPECT_EQ(ptr.block_data(), &bits);
    EXPECT_EQ(ptr.bit_position(), 0);
}

TEST_F(basic_bit_pointer_test, conversion) {
    char c { 'A' };
    basic_bit_pointer<char> ptr { &c };
    basic_bit_pointer<char const> const_ptr { ptr };
    EXPECT_EQ(ptr, const_ptr);
}

TEST_F(basic_bit_pointer_test, wide) {
    std::uint32_t bits = 0xffff;

    basic_bit_pointer<std::uint32_t> ptr { &bits };

    for (std::size_t i = 0; i < 16; ++i) {
        EXPECT_EQ(ptr.block_data(), &bits);
        EXPECT_TRUE(*ptr);
        ++ptr;
    }

    for (std::size_t i = 16; i < 32; ++i) {
        EXPECT_EQ(ptr.block_data(), &bits);
        EXPECT_FALSE(*ptr);
        ++ptr;
    }

    EXPECT_EQ(ptr.block_data(), &bits + 1);
}

TEST_F(basic_bit_pointer_test, output) {
    char c { 'A' };
    basic_bit_pointer<char> ptr { &c, 4 };
    std::cout << ptr << std::endl;
}

} // namespace takatori::util::details
