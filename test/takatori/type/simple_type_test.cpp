#include <takatori/type/simple_type.h>
#include <takatori/type/primitive.h>

#include <gtest/gtest.h>

#include <takatori/util/clonable.h>

namespace takatori::type {

class simple_type_test : public ::testing::Test {};

static_assert(int1::tag == type_kind::int1);
static_assert(std::is_same_v<type_of_t<int1::tag>, int1>);

static_assert(int2::tag == type_kind::int2);
static_assert(std::is_same_v<type_of_t<int2::tag>, int2>);

static_assert(int4::tag == type_kind::int4);
static_assert(std::is_same_v<type_of_t<int4::tag>, int4>);

static_assert(int8::tag == type_kind::int8);
static_assert(std::is_same_v<type_of_t<int8::tag>, int8>);

static_assert(float4::tag == type_kind::float4);
static_assert(std::is_same_v<type_of_t<float4::tag>, float4>);

static_assert(float8::tag == type_kind::float8);
static_assert(std::is_same_v<type_of_t<float8::tag>, float8>);

static_assert(unknown::tag == type_kind::unknown);
static_assert(std::is_same_v<type_of_t<unknown::tag>, unknown>);

TEST_F(simple_type_test, hash) {
    EXPECT_EQ(std::hash<int4>{}(int4()), std::hash<int4>{}(int4()));
    EXPECT_EQ(std::hash<float4>{}(float4()), std::hash<float4>{}(float4()));
    EXPECT_NE(std::hash<int4>{}(int4()), std::hash<float4>{}(float4()));
}

TEST_F(simple_type_test, clone) {
    int4 t;
    auto copy = util::clone_unique(t);
    EXPECT_EQ(t, *copy);
    EXPECT_NE(std::addressof(t), copy.get());
}

TEST_F(simple_type_test, clone_move) {
    float4 t;
    auto copy = util::clone_unique(t);
    EXPECT_EQ(t, *copy);
    EXPECT_NE(std::addressof(t), copy.get());

    auto move = util::clone_unique(std::move(t));
    EXPECT_NE(std::addressof(t), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(simple_type_test, output) {
    unknown t;
    std::cout << t << std::endl;
}

} // namespace takatori::type
