#include <takatori/util/optional_ptr.h>

#include <functional>
#include <string>

#include <gtest/gtest.h>

#include <takatori/util/pointer_traits.h>

namespace takatori::util {

class optional_reference_test : public ::testing::Test {
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

    template<class T>
    static T const& make_const(T& object) { return object; }
};

static_assert(is_pointer_v<optional_ptr<int>>);

static_assert(!std::is_constructible_v<optional_ptr<int>, int&&>);
static_assert(std::is_trivially_destructible_v<optional_ptr<int>>);
static_assert(sizeof(optional_ptr<int>) == sizeof(int*));

static_assert(!std::is_copy_constructible_v<optional_ptr<int>>);
static_assert(!std::is_copy_assignable_v<optional_ptr<int>>);
static_assert(std::is_move_constructible_v<optional_ptr<int>>);
static_assert(std::is_move_assignable_v<optional_ptr<int>>);

static_assert(std::is_constructible_v<optional_ptr<int>, optional_ptr<int>>);
static_assert(std::is_constructible_v<optional_ptr<int const>, optional_ptr<int const>>);
static_assert(std::is_constructible_v<optional_ptr<int const>, optional_ptr<int>>);
static_assert(!std::is_constructible_v<optional_ptr<int>, optional_ptr<int const>>);

static_assert(std::is_assignable_v<optional_ptr<int>&, optional_ptr<int>>);
static_assert(std::is_assignable_v<optional_ptr<int const>&, optional_ptr<int const>>);
static_assert(std::is_assignable_v<optional_ptr<int const>&, optional_ptr<int>>);
static_assert(!std::is_assignable_v<optional_ptr<int>&, optional_ptr<int const>>);

TEST_F(optional_reference_test, simple) {
    int x = 100;
    optional_ptr ref {x };
    ASSERT_TRUE(ref);
    EXPECT_EQ(*ref, 100);
    *ref = 200;
    EXPECT_EQ(x, 200);
}

TEST_F(optional_reference_test, copy_ctor) {
    int x = 100;
    optional_ptr ref {x };
    optional_ptr<int> copy {ref };
    EXPECT_EQ(*copy, x);
}

TEST_F(optional_reference_test, copy_ctor_const) {
    int x = 100;
    optional_ptr ref {x };
    optional_ptr<int const> copy {make_const(ref) };
    EXPECT_EQ(*copy, x);
}

TEST_F(optional_reference_test, copy_assign) {
    int x = 100;
    optional_ptr ref {x };
    optional_ptr<int> copy;
    copy = ref;
    EXPECT_EQ(*copy, x);
}

TEST_F(optional_reference_test, copy_assign_const) {
    int x = 100;
    optional_ptr ref {x };
    optional_ptr<int const> copy;
    copy = ref;
    EXPECT_EQ(*copy, x);
}

TEST_F(optional_reference_test, empty) {
    optional_ptr<std::string> ref;
    ASSERT_FALSE(ref);
    EXPECT_THROW(*ref, std::bad_optional_access);
    EXPECT_THROW(ref->data(), std::bad_optional_access);
    EXPECT_EQ(ref.begin(), ref.end());
}

TEST_F(optional_reference_test, nullptr) {
    std::string* ptr {};
    optional_ptr<std::string> ref {ptr };
    ASSERT_FALSE(ref);
    EXPECT_THROW(*ref, std::bad_optional_access);
    EXPECT_THROW(ref->data(), std::bad_optional_access);
    EXPECT_EQ(ref.begin(), ref.end());
}

TEST_F(optional_reference_test, pointer) {
    int x = 100;
    optional_ptr ref {&x };
    ASSERT_TRUE(ref);
    EXPECT_EQ(*ref, 100);

    *ref = 200;
    EXPECT_EQ(x, 200);
}

TEST_F(optional_reference_test, pointer_const) {
    int x = 100;
    optional_ptr ref {&x };
    auto&& cref = make_const(ref);

    ASSERT_TRUE(cref);
    EXPECT_EQ(*cref, 100);

    static_assert(!std::is_assignable_v<decltype(*cref), int>);
}

TEST_F(optional_reference_test, const_pointer) {
    int const x = 100;
    optional_ptr ref {&x };
    static_assert(std::is_const_v<std::remove_reference_t<decltype(*ref)>>);
    ASSERT_TRUE(ref);
    EXPECT_EQ(*ref, 100);
}

TEST_F(optional_reference_test, ref) {
    int x = 100;
    optional_ptr ref {x };
    ASSERT_TRUE(ref);
    EXPECT_EQ(*ref, 100);

    *ref = 200;
    EXPECT_EQ(x, 200);
}

TEST_F(optional_reference_test, ref_const) {
    int x = 100;
    optional_ptr ref {x };
    auto&& cref = make_const(ref);
    ASSERT_TRUE(cref);
    EXPECT_EQ(*cref, 100);

    static_assert(!std::is_assignable_v<decltype(*cref), int>);
}

TEST_F(optional_reference_test, const_ref) {
    int const x = 100;
    optional_ptr ref {x };
    static_assert(std::is_const_v<std::remove_reference_t<decltype(*ref)>>);
    ASSERT_TRUE(ref);
    EXPECT_EQ(*ref, 100);
}

TEST_F(optional_reference_test, reset) {
    int x = 100;
    optional_ptr ref {x };
    ASSERT_TRUE(ref);

    ref.reset();
    ASSERT_FALSE(ref);

    ref.reset();
    ASSERT_FALSE(ref);
}

TEST_F(optional_reference_test, swap) {
    int x = 100;
    optional_ptr r1 {x };
    optional_ptr<int> r2;
    EXPECT_TRUE(r1);
    EXPECT_FALSE(r2);

    swap(r1, r2);
    EXPECT_FALSE(r1);
    EXPECT_TRUE(r2);
    EXPECT_EQ(r2.get(), &x);
}

TEST_F(optional_reference_test, assign) {
    int x = 100;
    optional_ptr ref {x };

    int y = 200;
    ref = y;
    EXPECT_TRUE(ref);
    EXPECT_EQ(ref.get(), &y);
    EXPECT_EQ(x, 100);
    EXPECT_EQ(y, 200);
}

TEST_F(optional_reference_test, assign_nullptr) {
    int x = 100;
    optional_ptr ref {x };
    ref = nullptr;
    EXPECT_FALSE(ref);
}

TEST_F(optional_reference_test, iter) {
    int x = 100;
    optional_ptr ref {x };
    auto&& iter = ref.begin();
    ASSERT_NE(iter, ref.end());
    EXPECT_EQ(&*iter, &x);
    *iter = 200;
    ++iter;
    ASSERT_EQ(iter, ref.end());
}

TEST_F(optional_reference_test, iter_const) {
    int x = 100;
    optional_ptr ref {x };
    auto&& cref = make_const(ref);

    auto&& iter = cref.begin();
    ASSERT_NE(iter, cref.end());
    EXPECT_EQ(&*iter, &x);
    static_assert(!std::is_assignable_v<decltype(*iter), int>);
    ++iter;
    ASSERT_EQ(iter, ref.end());
}

TEST_F(optional_reference_test, invoke) {
    std::function<int(int)> f = [](auto x) { return x + 1; };
    optional_ptr ref {f };
    EXPECT_EQ(f(100), 101);
}

TEST_F(optional_reference_test, less) {
    int a = 100;
    int b = 200;
    optional_ptr ra {a };
    optional_ptr rb {b };
    optional_ptr<int> re {};

    EXPECT_TRUE(ra < rb);
    EXPECT_FALSE(rb < ra);

    EXPECT_TRUE(re < ra);
    EXPECT_FALSE(ra < re);

    EXPECT_TRUE(ra < 101);
    EXPECT_FALSE(ra < 100);
    EXPECT_TRUE(99 < ra);
    EXPECT_FALSE(100 < ra);
}

TEST_F(optional_reference_test, static_cast) {
    Sub x;
    optional_ptr<Sub> subref {x };
    auto baseref = static_cast<optional_ptr<Sub>>(subref);
    EXPECT_EQ(dynamic_cast<Sub*>(baseref.get()), &x);
}

TEST_F(optional_reference_test, static_cast_to_const) {
    int x = 100;
    optional_ptr<int> ref {x };
    optional_ptr<int const> cref {ref };
    EXPECT_EQ(ref.get(), cref.get());
}

TEST_F(optional_reference_test, dynamic_pointer_cast) {
    Sub x;
    optional_ptr<Base> baseref {x };
    auto subref = dynamic_pointer_cast<Sub>(baseref);
    EXPECT_EQ(subref.get(), &x);
    EXPECT_FALSE(dynamic_pointer_cast<Other>(baseref));
}

TEST_F(optional_reference_test, dynamic_pointer_cast_const) {
    Sub x;
    optional_ptr<Base> baseref {x };
    auto&& cbaseref = make_const(baseref);
    auto subref = dynamic_pointer_cast<Sub>(cbaseref);
    static_assert(std::is_const_v<decltype(subref)::element_type>);

    EXPECT_EQ(subref.get(), &x);
    EXPECT_FALSE(dynamic_pointer_cast<Other>(cbaseref));
}

TEST_F(optional_reference_test, dynamic_pointer_cast_rvalue) {
    Sub x;
    optional_ptr<Base> baseref {x };
    auto subref = dynamic_pointer_cast<Sub>(optional_ptr {*baseref });
    EXPECT_EQ(subref.get(), &x);
    EXPECT_FALSE(dynamic_pointer_cast<Other>(optional_ptr {*baseref }));
}

TEST_F(optional_reference_test, dynamic_pointer_bare) {
    Sub x;
    Base& baseref = x;
    auto subref = dynamic_pointer_cast<Sub>(baseref);
    EXPECT_EQ(subref.get(), &x);
    EXPECT_FALSE(dynamic_pointer_cast<Other>(baseref));
}

TEST_F(optional_reference_test, dynamic_reference_cast) {
    Sub x;
    optional_ptr<Base> baseref {x };
    auto subref = dynamic_reference_cast<Sub>(baseref);
    EXPECT_EQ(subref.get(), &x);
    EXPECT_THROW(dynamic_reference_cast<Other>(baseref), std::bad_cast);
}

TEST_F(optional_reference_test, dynamic_reference_cast_const) {
    Sub x;
    optional_ptr<Base> baseref {x };
    auto&& cbaseref = make_const(baseref);
    auto subref = dynamic_reference_cast<Sub>(cbaseref);
    static_assert(std::is_const_v<decltype(subref)::element_type>);

    EXPECT_EQ(subref.get(), &x);
    EXPECT_THROW(dynamic_reference_cast<Other>(cbaseref), std::bad_cast);
}

TEST_F(optional_reference_test, dynamic_reference_cast_rvalue) {
    Sub x;
    optional_ptr<Base> baseref {x };
    auto subref = dynamic_reference_cast<Sub>(optional_ptr {*baseref });
    EXPECT_EQ(subref.get(), &x);
    EXPECT_THROW(dynamic_reference_cast<Other>(optional_ptr {*baseref }), std::bad_cast);
}

} // namespace takatori::util
