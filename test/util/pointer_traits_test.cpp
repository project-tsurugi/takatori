#include "takatori/util/pointer_traits.h"

#include <memory>
#include <optional>
#include <vector>

#include <gtest/gtest.h>

namespace takatori::util {

class pointer_test : public ::testing::Test {};

static_assert(is_pointer_v<int*>);
static_assert(is_pointer_v<int const*>);
static_assert(is_pointer_v<std::unique_ptr<int>>);
static_assert(is_pointer_v<std::shared_ptr<int>>);
static_assert(is_pointer_v<std::unique_ptr<int const>>);
static_assert(is_pointer_v<std::shared_ptr<int const>>);
static_assert(!is_pointer_v<int>);
static_assert(!is_pointer_v<std::vector<int>>);

static_assert(is_raw_pointer_v<int*>);
static_assert(is_raw_pointer_v<int const*>);
static_assert(!is_raw_pointer_v<std::unique_ptr<int>>);

static_assert(is_smart_pointer_v<std::unique_ptr<int>>);
static_assert(is_smart_pointer_v<std::shared_ptr<int>>);
static_assert(is_smart_pointer_v<std::unique_ptr<int const>>);
static_assert(is_smart_pointer_v<std::shared_ptr<int const>>);
static_assert(!is_smart_pointer_v<int*>);

static_assert(std::is_same_v<remove_pointer_t<std::unique_ptr<int>>, int>);
static_assert(std::is_same_v<remove_pointer_t<std::unique_ptr<int const>>, int const>);
static_assert(std::is_same_v<remove_pointer_t<int*>, int>);
static_assert(std::is_same_v<remove_pointer_t<int const*>, int const>);
static_assert(std::is_same_v<remove_pointer_t<int>, int>);

TEST_F(pointer_test, simple) {
    // only type traits
}

} // namespace takatori::util
