#include "takatori/util/clonable.h"

#include <functional>
#include <string>

#include <gtest/gtest.h>

#include "takatori/util/rvalue_ptr.h"
#include "takatori/util/rvalue_reference_wrapper.h"

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

TEST_F(clonable_test, reference_wrapper) {
    std::unique_ptr<Base> ptr = std::make_unique<Sub>();
    auto copy = clone_unique(std::ref(*ptr));
    EXPECT_NE(ptr.get(), copy.get());
}

TEST_F(clonable_test, reference_wrapper_const) {
    std::unique_ptr<Base> ptr = std::make_unique<Sub>();
    auto copy = clone_unique(std::cref(*ptr));
    EXPECT_NE(ptr.get(), copy.get());
}

TEST_F(clonable_test, rvalue_reference_wrapper) {
    std::unique_ptr<Base> ptr = std::make_unique<Sub>();
    auto copy = clone_unique(rvalue_reference_wrapper { std::move(*ptr) });
    EXPECT_NE(ptr.get(), copy.get());
}

TEST_F(clonable_test, unique_ptr) {
    std::unique_ptr<Base const> const ptr = std::make_unique<Sub>();
    auto copy = clone_unique(ptr);
    EXPECT_NE(ptr.get(), copy.get());
}

TEST_F(clonable_test, unique_ptr_rvalue_compatible_deleter) {
    object_creator c;
    std::unique_ptr<Base, object_deleter> ptr = c.create_unique<Sub>();
    auto raw = ptr.get();
    auto copy = clone_unique(std::move(ptr), c);
    EXPECT_EQ(raw, copy.get());
}

TEST_F(clonable_test, unique_ptr_rvalue_incompatible_deleter) {
    std::unique_ptr<Base> ptr = std::make_unique<Sub>();
    auto raw = ptr.get();

    pmr::monotonic_buffer_resource r;
    object_creator c { &r };
    auto copy = clone_unique(std::move(ptr), c);
    EXPECT_NE(raw, copy.get());
}

TEST_F(clonable_test, clone_shared) {
    std::unique_ptr<Base> ptr = std::make_unique<Sub>();

    pmr::monotonic_buffer_resource r;
    object_creator c { &r };
    auto copy = clone_shared(*ptr, c);
    EXPECT_NE(ptr.get(), copy.get());

    auto* deleter = std::get_deleter<object_deleter>(copy);
    ASSERT_TRUE(deleter);
    EXPECT_EQ(deleter->creator(), c);
}

} // namespace takatori::util
