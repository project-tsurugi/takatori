#include "takatori/util/reference_iterator.h"

#include <vector>

#include <gtest/gtest.h>

namespace takatori::util {

class reference_iterator_test : public ::testing::Test {};

using traits = std::iterator_traits<reference_iterator<pointer_extractor<int>>>;
static_assert(std::is_same_v<typename traits::value_type, int>);
static_assert(std::is_same_v<typename traits::reference, int&>);
static_assert(std::is_same_v<typename traits::pointer, int*>);
static_assert(std::is_same_v<typename traits::difference_type, std::ptrdiff_t>);
static_assert(std::is_same_v<typename traits::iterator_category, std::random_access_iterator_tag>);

using const_traits = std::iterator_traits<reference_iterator<pointer_extractor<int const>>>;
static_assert(std::is_same_v<typename const_traits::value_type, int const>);
static_assert(std::is_same_v<typename const_traits::reference, int const&>);
static_assert(std::is_same_v<typename const_traits::pointer, int const*>);
static_assert(std::is_same_v<typename const_traits::difference_type, std::ptrdiff_t>);
static_assert(std::is_same_v<typename const_traits::iterator_category, std::random_access_iterator_tag>);

static_assert(std::is_constructible_v<reference_iterator<pointer_extractor<int const>>, reference_iterator<pointer_extractor<int>>>);
static_assert(!std::is_constructible_v<reference_iterator<pointer_extractor<int>>, reference_iterator<pointer_extractor<int const>>>);

TEST_F(reference_iterator_test, simple) {
    int a = 1, b = 2, c = 3;
    std::vector<int*> v { &a, &b, &c };

    reference_iterator iter { v.data() };
    reference_iterator end { v.data() + v.size() };

    ASSERT_NE(iter, end);
    EXPECT_EQ(*iter, 1);
    ++iter;

    ASSERT_NE(iter, end);
    EXPECT_EQ(*iter, 2);
    ++iter;

    ASSERT_NE(iter, end);
    EXPECT_EQ(*iter, 3);
    ++iter;

    ASSERT_EQ(iter, end);
}

TEST_F(reference_iterator_test, const) {
    int a = 1, b = 2, c = 3;
    std::vector<int*> v { &a, &b, &c };

    reference_iterator iter { v.data() };
    reference_iterator<double_pointer_extractor<int const>> citer { iter };

    EXPECT_EQ(iter, citer);
}

} // namespace takatori::util
