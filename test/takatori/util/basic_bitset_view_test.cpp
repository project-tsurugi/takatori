#include <takatori/util/basic_bitset_view.h>

#include <gtest/gtest.h>

namespace takatori::util {

class basic_bitset_view_test : public ::testing::Test {};

static_assert(std::is_trivially_copyable_v<basic_bitset_view<char>>);
static_assert(std::is_trivially_destructible_v<basic_bitset_view<char>>);

static_assert(std::is_constructible_v<basic_bitset_view<char const>, basic_bitset_view<char>>);

static_assert(std::is_convertible_v<
        typename basic_bitset_view<char>::iterator,
        typename basic_bitset_view<char>::const_iterator>);

static_assert(std::is_same_v<
        typename basic_bitset_view<char>::const_iterator,
        typename basic_bitset_view<char const>::iterator>);

TEST_F(basic_bitset_view_test, simple) {
    std::array<char, 2> buf { static_cast<char>(0x0f), static_cast<char>(0xf0) };
    basic_bitset_view<char> view { buf.data(), 16 };

    EXPECT_EQ(view.block_data(), buf.data());
    EXPECT_EQ(view.size(), 16);

    for (std::size_t i = 0; i < 4; ++i) {
        auto&& bit = view[i];
        EXPECT_TRUE(bit);
        bit = !bit;
    }
    for (std::size_t i = 4; i < 12; ++i) {
        auto&& bit = view[i];
        EXPECT_FALSE(bit);
        bit = !bit;
    }
    for (std::size_t i = 12; i < 16; ++i) {
        auto&& bit = view[i];
        EXPECT_TRUE(bit);
        bit = !bit;
    }
    EXPECT_EQ(buf[0], static_cast<char>(0xf0));
    EXPECT_EQ(buf[1], static_cast<char>(0x0f));
}

TEST_F(basic_bitset_view_test, const_view) {
    std::array<char, 2> buf { static_cast<char>(0x0f), static_cast<char>(0xf0) };
    basic_bitset_view<char> view { buf.data(), 16 };

    for (std::size_t i = 0; i < 4; ++i) {
        auto&& bit = view[i];
        EXPECT_TRUE(bit);
        bit = !bit;
    }
    for (std::size_t i = 4; i < 12; ++i) {
        auto&& bit = view[i];
        EXPECT_FALSE(bit);
        bit = !bit;
    }
    for (std::size_t i = 12; i < 16; ++i) {
        auto&& bit = view[i];
        EXPECT_TRUE(bit);
        bit = !bit;
    }
    EXPECT_EQ(buf[0], static_cast<char>(0xf0));
    EXPECT_EQ(buf[1], static_cast<char>(0x0f));
}

TEST_F(basic_bitset_view_test, iterator) {
    std::array<char, 2> buf { static_cast<char>(0x0f), static_cast<char>(0xf0) };
    basic_bitset_view<char> view { buf.data(), 16 };

    auto iter = view.begin();
    for (std::size_t i = 0; i < 4; ++i) {
        EXPECT_NE(iter, view.end());
        EXPECT_TRUE(*iter);
        *iter = !*iter;
        ++iter;
    }
    for (std::size_t i = 4; i < 12; ++i) {
        EXPECT_NE(iter, view.end());
        EXPECT_FALSE(*iter);
        *iter = !*iter;
        ++iter;
    }
    for (std::size_t i = 12; i < 16; ++i) {
        EXPECT_NE(iter, view.end());
        EXPECT_TRUE(*iter);
        *iter = !*iter;
        ++iter;
    }
    EXPECT_EQ(iter, view.end());
    EXPECT_EQ(buf[0], static_cast<char>(0xf0));
    EXPECT_EQ(buf[1], static_cast<char>(0x0f));
}

TEST_F(basic_bitset_view_test, const_iterator) {
    std::array<char, 2> buf { static_cast<char>(0x0f), static_cast<char>(0xf0) };
    basic_bitset_view<char> view { buf.data(), 16 };

    auto iter = view.cbegin();

    for (std::size_t i = 0; i < 4; ++i) {
        EXPECT_NE(iter, view.end());
        EXPECT_TRUE(*iter);
        ++iter;
    }
    for (std::size_t i = 4; i < 12; ++i) {
        EXPECT_NE(iter, view.end());
        EXPECT_FALSE(*iter);
        ++iter;
    }
    for (std::size_t i = 12; i < 16; ++i) {
        EXPECT_NE(iter, view.end());
        EXPECT_TRUE(*iter);
        ++iter;
    }
    EXPECT_EQ(iter, view.end());
}

TEST_F(basic_bitset_view_test, out) {
    std::array<char, 2> buf { static_cast<char>(0x30), static_cast<char>(0xa5) };
    basic_bitset_view<char> view { buf.data(), 16 };
    std::cout << view << std::endl;
}

} // namespace takatori::util
