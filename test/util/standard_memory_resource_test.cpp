#include "takatori/util/standard_memory_resource.h"

#include <gtest/gtest.h>

namespace takatori::util {

class standard_memory_resource_test : public ::testing::Test {};

TEST_F(standard_memory_resource_test, simple) {
    pmr::polymorphic_allocator<int> a { get_standard_memory_resource() };
    using traits = std::allocator_traits<decltype(a)>;

    auto* ptr = traits::allocate(a, 1);
    traits::construct(a, ptr, 100);
    EXPECT_EQ(*ptr, 100);

    traits::destroy(a, ptr);
    traits::deallocate(a, ptr, 1);
}

TEST_F(standard_memory_resource_test, bare_new) {
    pmr::polymorphic_allocator<int> a { get_standard_memory_resource() };
    using traits = std::allocator_traits<decltype(a)>;

    auto* ptr = new int(100);

    traits::destroy(a, ptr);
    traits::deallocate(a, ptr, 1);
}

TEST_F(standard_memory_resource_test, bare_delete) {
    pmr::polymorphic_allocator<int> a { get_standard_memory_resource() };
    using traits = std::allocator_traits<decltype(a)>;

    auto* ptr = traits::allocate(a, 1);
    traits::construct(a, ptr, 100);
    EXPECT_EQ(*ptr, 100);

    delete ptr;
}

TEST_F(standard_memory_resource_test, array) {
    pmr::polymorphic_allocator<int> a { get_standard_memory_resource() };
    using traits = std::allocator_traits<decltype(a)>;

    auto* ptr = traits::allocate(a, 3);
    traits::construct(a, &ptr[0], 100);
    traits::construct(a, &ptr[1], 200);
    traits::construct(a, &ptr[2], 300);
    EXPECT_EQ(ptr[0], 100);
    EXPECT_EQ(ptr[1], 200);
    EXPECT_EQ(ptr[2], 300);

    traits::destroy(a, &ptr[0]);
    traits::destroy(a, &ptr[1]);
    traits::destroy(a, &ptr[2]);
    traits::deallocate(a, ptr, 3);
}

} // namespace takatori::util
