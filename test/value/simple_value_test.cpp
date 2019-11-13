#include "takatori/value/simple_value.h"
#include "takatori/value/int.h"
#include "takatori/value/float.h"
#include "takatori/value/unknown.h"

#include <gtest/gtest.h>

#include "takatori/util/clonable.h"

namespace takatori::value {

class simple_value_test : public ::testing::Test {};

static_assert(int4::tag == value_kind::int4);
static_assert(std::is_same_v<type_of_t<int4::tag>, int4>);

static_assert(int8::tag == value_kind::int8);
static_assert(std::is_same_v<type_of_t<int8::tag>, int8>);

static_assert(float4::tag == value_kind::float4);
static_assert(std::is_same_v<type_of_t<float4::tag>, float4>);

static_assert(float8::tag == value_kind::float8);
static_assert(std::is_same_v<type_of_t<float8::tag>, float8>);

static_assert(unknown::tag == value_kind::unknown);
static_assert(std::is_same_v<type_of_t<unknown::tag>, unknown>);

TEST_F(simple_value_test, get) {
    int4 v { 100 };
    EXPECT_EQ(v.get(), 100);
}

TEST_F(simple_value_test, unknown) {
    unknown v {};
    EXPECT_EQ(v.get(), unknown::entity_type::null);
}

TEST_F(simple_value_test, hash) {
    EXPECT_EQ(std::hash<int4>{}(int4()), std::hash<int4>{}(int4()));
    EXPECT_EQ(std::hash<float4>{}(float4()), std::hash<float4>{}(float4()));
    EXPECT_NE(std::hash<int4>{}(int4()), std::hash<float4>{}(float4()));
}

TEST_F(simple_value_test, clone) {
    int4 v { 100 };
    auto copy = util::clone_unique(v);
    EXPECT_EQ(v, *copy);
    EXPECT_NE(std::addressof(v), copy.get());
}

TEST_F(simple_value_test, clone_move) {
    float4 v { 1.25 };
    auto copy = util::clone_unique(v);
    EXPECT_EQ(v, *copy);
    EXPECT_NE(std::addressof(v), copy.get());

    auto move = util::clone_unique(std::move(v));
    EXPECT_NE(std::addressof(v), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(simple_value_test, output) {
    unknown v;
    std::cout << v << std::endl;
}

} // namespace takatori::value
