#include "takatori/util/clonable.h"

#include <string>

#include <gtest/gtest.h>

namespace takatori::util {

class clonable_test : public ::testing::Test {};

struct NotClonable {};

struct Base {
    Base() = default;
    virtual ~Base() = default;
    virtual Base* clone(object_creator) const = 0;
};

struct Sub : Base {
    Sub() = default;
    ~Sub() override = default;
    Sub* clone(object_creator allocator) const override { return allocator.create_object<Sub>(); };
};

static_assert(!is_clonable_v<NotClonable>);
static_assert(is_clonable_v<Base>);
static_assert(is_clonable_v<Sub>);

static_assert(is_clonable_v<Base&>);
static_assert(is_clonable_v<Base&&>);
static_assert(is_clonable_v<Base const&>);

TEST_F(clonable_test, reference) {
    std::unique_ptr<Base> ptr = std::make_unique<Sub>();
    auto copy = clone_unique(*ptr);
    EXPECT_NE(ptr.get(), copy.get());
}

TEST_F(clonable_test, rvalue_reference) {
    std::unique_ptr<Base> ptr = std::make_unique<Sub>();
    auto copy = clone_unique(std::move(*ptr));
    EXPECT_NE(ptr.get(), copy.get());
}

TEST_F(clonable_test, pointer) {
    std::unique_ptr<Base> ptr = std::make_unique<Sub>();
    auto copy = clone_unique(ptr.get());
    EXPECT_NE(ptr.get(), copy.get());
}

TEST_F(clonable_test, pointer_const) {
    std::unique_ptr<Base> ptr = std::make_unique<Sub>();
    auto const* raw = ptr.get();
    auto copy = clone_unique(raw);
    EXPECT_NE(ptr.get(), copy.get());
}

} // namespace takatori::util
