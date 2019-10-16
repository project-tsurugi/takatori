#include "util/clonable.h"

#include <string>

#include <gtest/gtest.h>

namespace util {

class clonable_test : public ::testing::Test {};

struct NotClonable {};

struct Base {
    Base() = default;
    virtual ~Base() = default;
    virtual Base* clone() const = 0;
};

struct Sub : Base {
    Sub() = default;
    ~Sub() override = default;
    Sub* clone() const override { return new Sub(); };
};

static_assert(!is_clonable_v<NotClonable>);
static_assert(is_clonable_v<Base>);
static_assert(is_clonable_v<Sub>);

static_assert(is_clonable_v<Base&>);
static_assert(is_clonable_v<Base&&>);
static_assert(is_clonable_v<Base const&>);

TEST_F(clonable_test, reference) {
    std::unique_ptr<Base> ptr = std::make_unique<Sub>();
    auto copy = clone(*ptr);
    EXPECT_NE(ptr.get(), copy.get());
}

TEST_F(clonable_test, pointer) {
    std::unique_ptr<Base> ptr = std::make_unique<Sub>();
    auto copy = clone(ptr.get());
    EXPECT_NE(ptr.get(), copy.get());
}

TEST_F(clonable_test, pointer_const) {
    std::unique_ptr<Base> ptr = std::make_unique<Sub>();
    auto const* raw = ptr.get();
    auto copy = clone(raw);
    EXPECT_NE(ptr.get(), copy.get());
}

TEST_F(clonable_test, smart_pointer) {
    std::unique_ptr<Base> ptr = std::make_unique<Sub>();
    auto copy = clone(ptr);
    EXPECT_NE(ptr.get(), copy.get());
}

TEST_F(clonable_test, smart_pointer_const) {
    std::unique_ptr<Base> const ptr = std::make_unique<Sub>();
    auto copy = clone(ptr);
    EXPECT_NE(ptr.get(), copy.get());
}

TEST_F(clonable_test, smart_pointer_move) {
    std::unique_ptr<Base> ptr = std::make_unique<Sub>();
    auto copy = clone(std::move(ptr));
    EXPECT_NE(ptr.get(), copy.get());
}

} // namespace util
