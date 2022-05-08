#include <takatori/util/basic_buffer_view.h>

#include <gtest/gtest.h>

#include <string_view>

namespace takatori::util {

class basic_buffer_view_test : public ::testing::Test {};

static_assert(std::is_trivially_copyable_v<basic_buffer_view<char>>);
static_assert(std::is_trivially_destructible_v<basic_buffer_view<char>>);

static_assert(std::is_constructible_v<basic_buffer_view<char const>, basic_buffer_view<char>>);

static_assert(std::is_convertible_v<
        typename basic_buffer_view<char>::iterator,
        typename basic_buffer_view<char>::const_iterator>);

static_assert(std::is_same_v<
        typename basic_buffer_view<char>::const_iterator,
        typename basic_buffer_view<char const>::iterator>);

TEST_F(basic_buffer_view_test, simple) {
    std::string buf { "Hello!" };

    basic_buffer_view<char> view { buf.data(), buf.size() };

    EXPECT_EQ(view.data(), buf.data());
    EXPECT_EQ(view.size(), buf.size());

    EXPECT_EQ(view[0], 'H');
    EXPECT_EQ(view[1], 'e');
    EXPECT_EQ(view[2], 'l');
    EXPECT_EQ(view[3], 'l');
    EXPECT_EQ(view[4], 'o');
    EXPECT_EQ(view[5], '!');

    EXPECT_THROW({ (void) view.at(6); }, std::out_of_range);
}

TEST_F(basic_buffer_view_test, const_view) {
    using namespace std::string_view_literals;
    std::string_view buf = "Hello!"sv;

    basic_buffer_view<char const> view { buf.data(), buf.size() };

    EXPECT_EQ(view.data(), buf.data());
    EXPECT_EQ(view.size(), buf.size());

    EXPECT_EQ(view[0], 'H');
    EXPECT_EQ(view[1], 'e');
    EXPECT_EQ(view[2], 'l');
    EXPECT_EQ(view[3], 'l');
    EXPECT_EQ(view[4], 'o');
    EXPECT_EQ(view[5], '!');

    EXPECT_THROW({ (void) view.at(6); }, std::out_of_range);
}

TEST_F(basic_buffer_view_test, iterator) {
    std::string buf { "ABC" };
    basic_buffer_view<char> view { buf.data(), buf.size() };

    auto it = view.begin();
    static_assert(std::is_assignable_v<decltype(*it), char>);

    ASSERT_NE(it, view.end());
    EXPECT_EQ(*it, 'A');
    *it = '1';

    ++it;
    ASSERT_NE(it, view.end());
    EXPECT_EQ(*it, 'B');
    *it = '2';

    ++it;
    ASSERT_NE(it, view.end());
    EXPECT_EQ(*it, 'C');
    *it = '3';

    ++it;
    EXPECT_EQ(it, view.end());

    EXPECT_EQ(buf, "123");
}

TEST_F(basic_buffer_view_test, const_iterator) {
    std::string buf { "ABC" };
    basic_buffer_view<char> view { buf.data(), buf.size() };

    auto it = view.cbegin();
    static_assert(!std::is_assignable_v<decltype(*it), char>);

    ASSERT_NE(it, view.end());
    EXPECT_EQ(*it, 'A');

    ++it;
    ASSERT_NE(it, view.end());
    EXPECT_EQ(*it, 'B');

    ++it;
    ASSERT_NE(it, view.end());
    EXPECT_EQ(*it, 'C');

    ++it;
    EXPECT_EQ(it, view.end());
}

TEST_F(basic_buffer_view_test, wide) {
    std::array<std::int64_t, 4> buf { 1, 2, 3, 4 };

    basic_buffer_view<std::int64_t> view { buf.data(), buf.size() };

    EXPECT_EQ(view.data(), buf.data());
    EXPECT_EQ(view.size(), buf.size());

    EXPECT_EQ(view[0], 1);
    EXPECT_EQ(view[1], 2);
    EXPECT_EQ(view[2], 3);
    EXPECT_EQ(view[3], 4);

    auto iter = view.begin();
    for (auto&& v : buf) {
        EXPECT_NE(iter, view.end());
        EXPECT_EQ(*iter, v);
        *iter = v * 100;
        ++iter;
    }

    EXPECT_EQ(buf[0], 100);
    EXPECT_EQ(buf[1], 200);
    EXPECT_EQ(buf[2], 300);
    EXPECT_EQ(buf[3], 400);
}

TEST_F(basic_buffer_view_test, out) {
    std::string buf { "Hello!" };
    basic_buffer_view<char> view { buf.data(), buf.size() };
    std::cout << view << std::endl;
}

} // namespace takatori::util
