#include <takatori/util/maybe_shared_ptr.h>

#include <gtest/gtest.h>

#include <takatori/util/hash.h>

namespace takatori::util {

class maybe_shared_ptr_test : public ::testing::Test {};

class Base {};
class Sub : public Base {};

static_assert(std::is_constructible_v<maybe_shared_ptr<int>, int*>);
static_assert(std::is_constructible_v<maybe_shared_ptr<int>, std::shared_ptr<int>>);
static_assert(std::is_constructible_v<maybe_shared_ptr<int>, maybe_shared_ptr<int>>);
static_assert(std::is_constructible_v<maybe_shared_ptr<Base>, maybe_shared_ptr<Sub>&>);

TEST_F(maybe_shared_ptr_test, naked) {
    int v = 100;
    maybe_shared_ptr ptr { &v };

    EXPECT_TRUE(ptr);
    EXPECT_EQ(ptr.get(), &v);
    EXPECT_EQ(ptr.ownership(), nullptr);
    EXPECT_EQ(*ptr, 100);

    v = 200;
    EXPECT_EQ(*ptr, 200);

    auto copy = ptr;
    EXPECT_EQ(copy, ptr);

    int x = v;
    EXPECT_EQ(ptr, maybe_shared_ptr(&v));
    EXPECT_NE(ptr, maybe_shared_ptr(&x));
    EXPECT_EQ(hash(ptr), hash(&v));
}

TEST_F(maybe_shared_ptr_test, shared) {
    auto v = std::make_shared<int>(100);
    maybe_shared_ptr ptr { v };

    EXPECT_TRUE(ptr);
    EXPECT_EQ(ptr.get(), v.get());
    EXPECT_EQ(ptr.ownership(), v);
    EXPECT_EQ(*ptr, 100);

    *v = 200;
    EXPECT_EQ(*ptr, 200);

    auto copy = ptr;
    EXPECT_EQ(copy, ptr);

    int x = *v;
    EXPECT_EQ(ptr, maybe_shared_ptr(v));
    EXPECT_NE(ptr, maybe_shared_ptr(&x));
    EXPECT_EQ(hash(ptr), hash(v));
}

TEST_F(maybe_shared_ptr_test, nullptr) {
    maybe_shared_ptr<int> ptr {};

    EXPECT_FALSE(ptr);
    EXPECT_EQ(ptr.get(), nullptr);
    EXPECT_EQ(ptr.ownership(), nullptr);

    int x {};
    EXPECT_EQ(ptr, maybe_shared_ptr<int>());
    EXPECT_NE(ptr, maybe_shared_ptr(&x));
    EXPECT_EQ(hash(ptr), hash<void*>(nullptr));
}

TEST_F(maybe_shared_ptr_test, ctor_conversion_naked) {
    int v = 100;

    maybe_shared_ptr<int> ptr { &v };
    maybe_shared_ptr<int const> conv { ptr };

    EXPECT_EQ(conv, ptr);
}

TEST_F(maybe_shared_ptr_test, ctor_conversion_naked_move) {
    int v = 100;

    maybe_shared_ptr<int> ptr { &v };
    maybe_shared_ptr<int const> conv { std::move(ptr) };

    EXPECT_EQ(conv.get(), &v);
}

TEST_F(maybe_shared_ptr_test, ctor_conversion_shared) {
    auto v = std::make_shared<int>(100);

    maybe_shared_ptr<int> ptr { v };
    maybe_shared_ptr<int const> conv { ptr };

    EXPECT_EQ(conv.ownership(), v);
}

TEST_F(maybe_shared_ptr_test, ctor_conversion_shared_move) {
    auto v = std::make_shared<int>(100);

    maybe_shared_ptr<int> ptr { v };
    maybe_shared_ptr<int const> conv { std::move(ptr) };

    EXPECT_EQ(conv.ownership(), v);
}

TEST_F(maybe_shared_ptr_test, assign_nullptr) {
    maybe_shared_ptr<int> ptr { std::make_shared<int>(100) };
    ptr = nullptr;

    EXPECT_EQ(ptr.get(), nullptr);
}

TEST_F(maybe_shared_ptr_test, assign_naked) {
    maybe_shared_ptr<int> ptr { std::make_shared<int>(100) };

    int v {};
    ptr = &v;

    EXPECT_EQ(ptr.get(), &v);
}

TEST_F(maybe_shared_ptr_test, assign_shared) {
    maybe_shared_ptr<int> ptr { std::make_shared<int>(100) };

    auto v = std::make_shared<int>(100);
    ptr = v;

    EXPECT_EQ(ptr.ownership(), v);
}

TEST_F(maybe_shared_ptr_test, assign_conversion) {
    auto v = std::make_shared<int>(100);

    maybe_shared_ptr<int> ptr { v };
    maybe_shared_ptr<int const> conv;
    conv = ptr;

    EXPECT_EQ(conv.ownership(), v);
}

TEST_F(maybe_shared_ptr_test, assign_conversion_move) {
    auto v = std::make_shared<int>(100);

    maybe_shared_ptr<int> ptr { v };
    maybe_shared_ptr<int const> conv;
    conv = std::move(ptr);

    EXPECT_EQ(conv.ownership(), v);
}

TEST_F(maybe_shared_ptr_test, reset) {
    maybe_shared_ptr ptr { std::make_shared<int>(100) };
    auto weak = std::weak_ptr { ptr.ownership() };
    EXPECT_TRUE(weak.lock());

    ptr.reset();
    EXPECT_FALSE(ptr);
    EXPECT_FALSE(weak.lock());
}

TEST_F(maybe_shared_ptr_test, output_naked) {
    int v = 100;
    maybe_shared_ptr ptr { &v };
    std::cout << ptr << std::endl;
}

TEST_F(maybe_shared_ptr_test, output_shared) {
    maybe_shared_ptr ptr { std::make_shared<int>(100) };
    std::cout << ptr << std::endl;
}

} // namespace takatori::util
