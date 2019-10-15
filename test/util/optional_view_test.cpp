#include "util/optional_view.h"

#include <functional>
#include <string>

#include <gtest/gtest.h>

namespace util {

class optional_view_test : public ::testing::Test {
public:
    struct Base {
        virtual void f() = 0;
    };
    struct Sub : Base {
        void f() override {}
    };
    struct Other : Base {
        void f() override {}
    };
};

TEST_F(optional_view_test, simple) {
    int x = 100;
    optional_view ref {x };
    ASSERT_TRUE(ref);
    EXPECT_EQ(*ref, 100);
    *ref = 200;
    EXPECT_EQ(x, 200);
}

TEST_F(optional_view_test, empty) {
    optional_view<std::string> ref;
    ASSERT_FALSE(ref);
    EXPECT_THROW(*ref, std::bad_optional_access);
    EXPECT_THROW(ref->data(), std::bad_optional_access);
    EXPECT_EQ(ref.begin(), ref.end());
}

TEST_F(optional_view_test, nullptr) {
    std::string* ptr {};
    optional_view<std::string> ref {ptr };
    ASSERT_FALSE(ref);
    EXPECT_THROW(*ref, std::bad_optional_access);
    EXPECT_THROW(ref->data(), std::bad_optional_access);
    EXPECT_EQ(ref.begin(), ref.end());
}

TEST_F(optional_view_test, pointer) {
    int x = 100;
    optional_view ref {&x };
    ASSERT_TRUE(ref);
    EXPECT_EQ(*ref, 100);

    *ref = 200;
    EXPECT_EQ(x, 200);
}

TEST_F(optional_view_test, const_pointer) {
    int const x = 100;
    optional_view ref {&x };
    static_assert(std::is_const_v<std::remove_reference_t<decltype(*ref)>>);
    ASSERT_TRUE(ref);
    EXPECT_EQ(*ref, 100);
}

TEST_F(optional_view_test, const_ref) {
    int const x = 100;
    optional_view ref {x };
    static_assert(std::is_const_v<std::remove_reference_t<decltype(*ref)>>);
    ASSERT_TRUE(ref);
    EXPECT_EQ(*ref, 100);
}

TEST_F(optional_view_test, rvalue) {
    auto uptr = std::make_unique<int>(1);
    optional_view ref {uptr };

    auto another = *optional_view{uptr };
    EXPECT_EQ(*another, 1);
    EXPECT_FALSE(uptr);
}

TEST_F(optional_view_test, reset) {
    int x = 100;
    optional_view ref {x };
    ASSERT_TRUE(ref);

    ref.reset();
    ASSERT_FALSE(ref);

    ref.reset();
    ASSERT_FALSE(ref);
}

TEST_F(optional_view_test, swap) {
    int x = 100;
    optional_view r1 {x };
    optional_view<int> r2;
    EXPECT_TRUE(r1);
    EXPECT_FALSE(r2);

    swap(r1, r2);
    EXPECT_FALSE(r1);
    EXPECT_TRUE(r2);
    EXPECT_EQ(r2.get(), &x);
}

TEST_F(optional_view_test, assign) {
    int x = 100;
    optional_view ref {x };

    int y = 200;
    ref = y;
    EXPECT_TRUE(ref);
    EXPECT_EQ(ref.get(), &y);
    EXPECT_EQ(x, 100);
    EXPECT_EQ(y, 200);
}

TEST_F(optional_view_test, assign_nullptr) {
    int x = 100;
    optional_view ref {x };
    ref = nullptr;
    EXPECT_FALSE(ref);
}

TEST_F(optional_view_test, iter) {
    int x = 100;
    optional_view ref {x };
    auto&& iter = ref.begin();
    ASSERT_NE(iter, ref.end());
    EXPECT_EQ(&*iter, &x);
    *iter = 200;
    ++iter;
    ASSERT_EQ(iter, ref.end());
}

TEST_F(optional_view_test, invoke) {
    std::function<int(int)> f = [](auto x) { return x + 1; };
    optional_view ref {f };
    EXPECT_EQ(f(100), 101);
}

TEST_F(optional_view_test, less) {
    int a = 100;
    int b = 200;
    optional_view ra {a };
    optional_view rb {b };
    optional_view<int> re {};

    EXPECT_TRUE(ra < rb);
    EXPECT_FALSE(rb < ra);

    EXPECT_TRUE(re < ra);
    EXPECT_FALSE(ra < re);

    EXPECT_TRUE(ra < 101);
    EXPECT_FALSE(ra < 100);
    EXPECT_TRUE(99 < ra);
    EXPECT_FALSE(100 < ra);
}

TEST_F(optional_view_test, static_cast) {
    Sub x;
    optional_view<Sub> subref {x };
    auto baseref = static_cast<optional_view<Sub>>(subref);
    EXPECT_EQ(dynamic_cast<Sub*>(baseref.get()), &x);
}

TEST_F(optional_view_test, static_cast_to_const) {
    int x = 100;
    optional_view<int> ref {x };
    optional_view<int const> cref {ref };
    EXPECT_EQ(ref.get(), cref.get());
}

TEST_F(optional_view_test, dynamic_pointer_cast) {
    Sub x;
    optional_view<Base> baseref {x };
    auto subref = dynamic_pointer_cast<Sub>(baseref);
    EXPECT_EQ(subref.get(), &x);
    EXPECT_FALSE(dynamic_pointer_cast<Other>(baseref));
}

TEST_F(optional_view_test, dynamic_reference_cast) {
    Sub x;
    optional_view<Base> baseref {x };
    auto subref = dynamic_reference_cast<Sub>(baseref);
    EXPECT_EQ(subref.get(), &x);
    EXPECT_THROW(dynamic_reference_cast<Other>(baseref), std::bad_cast);
}

} // namespace util
