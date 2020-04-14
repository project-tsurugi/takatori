#include <takatori/util/object_creator.h>

#include <string>

#include <gtest/gtest.h>

namespace takatori::util {

class object_creator_test : public ::testing::Test {};

static_assert(std::is_trivially_copyable_v<object_creator>);
static_assert(std::is_trivially_destructible_v<object_creator>);

static_assert(std::is_trivially_copyable_v<object_deleter>);
static_assert(std::is_trivially_destructible_v<object_deleter>);

TEST_F(object_creator_test, simple) {
    object_creator creator;

    auto* str = creator.create_object<std::string>("Hello");
    EXPECT_EQ(*str, "Hello");

    creator.delete_object(str);
}

TEST_F(object_creator_test, create_unique) {
    object_creator creator;

    auto str = creator.create_unique<std::string>("Hello");
    EXPECT_EQ(*str, "Hello");
}

TEST_F(object_creator_test, create_shared) {
    object_creator creator;

    auto str = creator.create_shared<std::string>("Hello");
    EXPECT_EQ(*str, "Hello");
}

TEST_F(object_creator_test, wrap_shared_nullptr) {
    object_creator creator;

    auto str = creator.wrap_shared<std::string>(nullptr);
    EXPECT_EQ(str, nullptr);
}

TEST_F(object_creator_test, allocator) {
    object_creator creator;

    using string = std::basic_string<char, std::char_traits<char>, object_allocator<char>>;
    string str { "Hello, custom allocator!", creator.allocator<char>() };
    EXPECT_EQ(str, "Hello, custom allocator!");
}

TEST_F(object_creator_test, allocator_void) {
    object_creator creator;

    using string = std::basic_string<char, std::char_traits<char>, object_allocator<char>>;
    string str { "Hello, custom allocator!", creator.allocator() };
    EXPECT_EQ(str, "Hello, custom allocator!");
}

} // namespace takatori::util
