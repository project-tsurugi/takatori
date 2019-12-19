#include <takatori/util/smart_pointer_extractor.h>

#include <memory>
#include <vector>

#include <gtest/gtest.h>

namespace takatori::util {

class smart_pointer_extractor_test : public ::testing::Test {};

using ut = smart_pointer_extractor<std::unique_ptr<int>>;
static_assert(std::is_same_v<ut::value_type, int>);
static_assert(std::is_same_v<ut::pointer, int*>);
static_assert(std::is_same_v<ut::reference, int&>);
static_assert(std::is_same_v<ut::cursor_type, std::unique_ptr<int> const*>);

TEST_F(smart_pointer_extractor_test, unique) {
    std::vector<std::unique_ptr<int>> v;
    v.emplace_back(std::make_unique<int>(1));
    v.emplace_back(std::make_unique<int>(2));
    v.emplace_back(std::make_unique<int>(3));

    using ext = smart_pointer_extractor<std::unique_ptr<int>>;

    ext::cursor_type it = v.data();
    ext::cursor_type end = ext::advance(it, 3);

    ASSERT_NE(it, end);
    EXPECT_EQ(ext::get(it), 1);

    it = ext::advance(it, 1);
    ASSERT_NE(it, end);
    EXPECT_EQ(ext::get(it), 2);

    it = ext::advance(it, 1);
    ASSERT_NE(it, end);
    EXPECT_EQ(ext::get(it), 3);

    it = ext::advance(it, 1);
    EXPECT_EQ(it, end);
}

TEST_F(smart_pointer_extractor_test, shared) {
    std::vector<std::shared_ptr<int>> v;
    v.emplace_back(std::make_shared<int>(1));
    v.emplace_back(std::make_shared<int>(2));
    v.emplace_back(std::make_shared<int>(3));

    using ext = smart_pointer_extractor<std::shared_ptr<int>>;

    ext::cursor_type it = v.data();
    ext::cursor_type end = ext::advance(it, 3);

    ASSERT_NE(it, end);
    EXPECT_EQ(ext::get(it), 1);

    it = ext::advance(it, 1);
    ASSERT_NE(it, end);
    EXPECT_EQ(ext::get(it), 2);

    it = ext::advance(it, 1);
    ASSERT_NE(it, end);
    EXPECT_EQ(ext::get(it), 3);

    it = ext::advance(it, 1);
    EXPECT_EQ(it, end);
}

} // namespace takatori::util
