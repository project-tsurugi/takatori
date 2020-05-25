#include <takatori/util/rvalue_reference_wrapper.h>

#include <functional>
#include <memory>
#include <vector>

#include <gtest/gtest.h>

namespace takatori::util {

class rvalue_reference_wrapper_test : public ::testing::Test {
public:
    template<class T>
    std::vector<T> to_vec(std::initializer_list<rvalue_reference_wrapper<T>> list) {
        return std::vector<T> { list.begin(), list.end() };
    }
};

TEST_F(rvalue_reference_wrapper_test, simple) {
    int v = rvref(1 + 2);
    EXPECT_EQ(v, 3);
}

TEST_F(rvalue_reference_wrapper_test, expier) {
    auto v = to_vec<int>({ 1, 2 + 3, 4 + 5 });

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 5);
    EXPECT_EQ(v[2], 9);
}

TEST_F(rvalue_reference_wrapper_test, implicit) {
    auto v = to_vec<std::unique_ptr<int>>({
            std::make_unique<int>(1),
            std::make_unique<int>(2),
            std::make_unique<int>(3),
    });
    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(*v[0], 1);
    EXPECT_EQ(*v[1], 2);
    EXPECT_EQ(*v[2], 3);
}

} // namespace takatori::util
