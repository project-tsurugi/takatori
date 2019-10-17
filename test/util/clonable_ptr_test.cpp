#include "util/clonable_ptr.h"

#include <memory>
#include <string>
#include <utility>

#include <gtest/gtest.h>

#include "util/pointer_traits.h"

namespace util {

class clonable_ptr_test : public ::testing::Test {};

class Obj {
public:
    Obj() = default;
    explicit Obj(int count) noexcept : count_(count) {}
    int count() const noexcept { return count_; }
    Obj* clone(object_creator allocator) const { return allocator.create_object<Obj>(count() + 1); }

private:
    int count_ { 0 };
};

static_assert(is_smart_pointer_v<clonable_ptr<Obj>>);

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

TEST_F(clonable_ptr_test, simple) {
    auto obj = make_clonable<Obj>({});
    auto copy = obj;
    EXPECT_EQ(obj->count(), 0);
    EXPECT_EQ(copy->count(), 1);
    EXPECT_NE(obj, copy);
}

TEST_F(clonable_ptr_test, empty) {
    clonable_ptr<Obj> ptr;
    ASSERT_FALSE(ptr);
}

TEST_F(clonable_ptr_test, nullptr) {
    clonable_ptr<Obj> ptr { nullptr };
    ASSERT_FALSE(ptr);
}

TEST_F(clonable_ptr_test, raw_ptr) {
    object_creator creator;
    auto raw = creator.create_object<Obj>(100);
    clonable_ptr<Obj> ptr { raw, creator };
    ASSERT_TRUE(ptr);
    EXPECT_EQ(ptr->count(), 100);
    EXPECT_EQ(ptr.get(), raw);
}

TEST_F(clonable_ptr_test, unique_ptr) {
    auto smart = std::make_unique<Obj>(100);
    auto raw = smart.get();
    clonable_ptr ptr { std::move(smart), standard_object_creator() };
    ASSERT_TRUE(ptr);
    EXPECT_EQ(ptr->count(), 100);
    EXPECT_EQ(ptr.get(), raw);
}

TEST_F(clonable_ptr_test, unique_ptr_clone) {
    pmr::monotonic_buffer_resource resource;
    object_creator creator { &resource };
    auto smart = creator.create_unique<Obj>(100);
    auto raw = smart.get();
    clonable_ptr ptr { std::move(smart), standard_object_creator() };
    ASSERT_TRUE(ptr);
    EXPECT_EQ(ptr->count(), 101);
    EXPECT_NE(ptr.get(), raw);
}

TEST_F(clonable_ptr_test, copy_ctor) {
    auto obj = make_clonable<Obj>({});
    clonable_ptr copy { obj };
    EXPECT_EQ(obj->count(), 0);
    EXPECT_EQ(copy->count(), 1);
    EXPECT_NE(obj, copy);
}

TEST_F(clonable_ptr_test, copy_assign) {
    auto obj = make_clonable<Obj>({});
    clonable_ptr<Obj> copy;
    ASSERT_FALSE(copy);

    copy = obj;
    ASSERT_TRUE(copy);
    EXPECT_EQ(obj->count(), 0);
    EXPECT_EQ(copy->count(), 1);
    EXPECT_NE(obj, copy);
}

TEST_F(clonable_ptr_test, move_ctor) {
    auto obj = make_clonable<Obj>({});
    EXPECT_EQ(obj->count(), 0);
    auto* ptr = obj.get();

    clonable_ptr copy { std::move(obj) };
    EXPECT_EQ(copy->count(), 0);
    EXPECT_EQ(copy.get(), ptr);
}

TEST_F(clonable_ptr_test, move_assign) {
    auto obj = make_clonable<Obj>({});
    EXPECT_EQ(obj->count(), 0);
    auto* ptr = obj.get();

    clonable_ptr<Obj> copy;
    ASSERT_FALSE(copy);

    copy = std::move(obj);
    ASSERT_TRUE(copy);
    EXPECT_EQ(copy->count(), 0);
    EXPECT_EQ(copy.get(), ptr);
}

TEST_F(clonable_ptr_test, nullptr_assign) {
    auto obj = make_clonable<Obj>({});
    EXPECT_EQ(obj->count(), 0);

    obj = nullptr;
    ASSERT_FALSE(obj);
}

TEST_F(clonable_ptr_test, release) {
    auto obj = make_clonable<Obj>({});
    auto raw = obj.get();
    EXPECT_EQ(obj->count(), 0);

    std::unique_ptr<Obj, object_deleter> unique { obj.release(), obj.deleter() };
    ASSERT_FALSE(obj);
    EXPECT_EQ(unique.get(), raw);
}

TEST_F(clonable_ptr_test, reset) {
    auto obj = make_clonable<Obj>({});
    ASSERT_TRUE(obj);
    obj.reset();
    ASSERT_FALSE(obj);
}

TEST_F(clonable_ptr_test, reset_pointer) {
    auto obj = make_clonable<Obj>({});
    ASSERT_TRUE(obj);

    obj.reset(obj.creator().template create_object<Obj>(100));
    ASSERT_TRUE(obj);
    EXPECT_EQ(obj->count(), 100);
}

TEST_F(clonable_ptr_test, swap) {
    auto a = make_clonable<Obj>({}, 100);
    auto b = make_clonable<Obj>({}, 200);

    auto* a_ptr = a.get();
    auto* b_ptr = b.get();

    swap(a, b);

    EXPECT_EQ(a.get(), b_ptr);
    EXPECT_EQ(b.get(), a_ptr);
}

} // namespace util
