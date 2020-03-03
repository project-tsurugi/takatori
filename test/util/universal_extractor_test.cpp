#include <takatori/util/universal_extractor.h>

#include <memory>
#include <vector>

#include <gtest/gtest.h>

namespace takatori::util {

class universal_extractor_test : public ::testing::Test {};

TEST_F(universal_extractor_test, unique) {
    std::vector<std::unique_ptr<int>> v;
    v.emplace_back(std::make_unique<int>(1));
    v.emplace_back(std::make_unique<int>(2));
    v.emplace_back(std::make_unique<int>(3));

    universal_extractor<int> ext {
            [](void* cursor) -> int& {
                return **static_cast<std::unique_ptr<int>*>(cursor);
            },
            [](void* cursor, std::ptrdiff_t offset) {
                return static_cast<void*>(static_cast<std::unique_ptr<int>*>(cursor) + offset);
            },
    };

    decltype(ext)::cursor_type it = v.data();
    decltype(ext)::cursor_type end = ext.advance(it, 3);

    ASSERT_NE(it, end);
    EXPECT_EQ(ext.get(it), 1);

    it = ext.advance(it, 1);
    ASSERT_NE(it, end);
    EXPECT_EQ(ext.get(it), 2);

    it = ext.advance(it, 1);
    ASSERT_NE(it, end);
    EXPECT_EQ(ext.get(it), 3);

    it = ext.advance(it, 1);
    EXPECT_EQ(it, end);
}

TEST_F(universal_extractor_test, shared) {
    std::vector<std::shared_ptr<int>> v;
    v.emplace_back(std::make_shared<int>(1));
    v.emplace_back(std::make_shared<int>(2));
    v.emplace_back(std::make_shared<int>(3));

    universal_extractor<int> ext {
            [](void* cursor) -> int& {
                return **static_cast<std::shared_ptr<int>*>(cursor);
            },
            [](void* cursor, std::ptrdiff_t offset) {
                return static_cast<void*>(static_cast<std::shared_ptr<int>*>(cursor) + offset);
            },
    };

    decltype(ext)::cursor_type it = v.data();
    decltype(ext)::cursor_type end = ext.advance(it, 3);

    ASSERT_NE(it, end);
    EXPECT_EQ(ext.get(it), 1);

    it = ext.advance(it, 1);
    ASSERT_NE(it, end);
    EXPECT_EQ(ext.get(it), 2);

    it = ext.advance(it, 1);
    ASSERT_NE(it, end);
    EXPECT_EQ(ext.get(it), 3);

    it = ext.advance(it, 1);
    EXPECT_EQ(it, end);
}

} // namespace takatori::util
