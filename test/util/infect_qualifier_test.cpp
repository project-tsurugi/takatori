#include "util/infect_qualifier.h"

#include <gtest/gtest.h>

namespace util {

class infect_qualifier_test : public ::testing::Test {};

static_assert(std::is_same_v<infect_reference_t<int, double>, double>);
static_assert(std::is_same_v<infect_reference_t<int&, double>, double&>);
static_assert(std::is_same_v<infect_reference_t<int&&, double>, double&&>);

static_assert(std::is_same_v<infect_reference_t<int, double const>, double const>);
static_assert(std::is_same_v<infect_reference_t<int&, double const>, double const&>);
static_assert(std::is_same_v<infect_reference_t<int&&, double const>, double const&&>);

static_assert(std::is_same_v<infect_reference_t<int, double&>, double&>);
static_assert(std::is_same_v<infect_reference_t<int&, double&>, double&>);
static_assert(std::is_same_v<infect_reference_t<int&&, double&>, double&>);

static_assert(std::is_same_v<infect_reference_t<int, double&&>, double&&>);
static_assert(std::is_same_v<infect_reference_t<int&, double&&>, double&>);
static_assert(std::is_same_v<infect_reference_t<int&&, double&&>, double&&>);

static_assert(std::is_same_v<infect_const_t<int, double>, double>);
static_assert(std::is_same_v<infect_const_t<int const, double>, double const>);
static_assert(std::is_same_v<infect_const_t<int, double const>, double const>);
static_assert(std::is_same_v<infect_const_t<int const, double const>, double const>);

static_assert(std::is_same_v<infect_qualifier_t<int, double>, double>);
static_assert(std::is_same_v<infect_qualifier_t<int&, double>, double&>);
static_assert(std::is_same_v<infect_qualifier_t<int&&, double>, double&&>);

static_assert(std::is_same_v<infect_qualifier_t<int const, double>, double const>);
static_assert(std::is_same_v<infect_qualifier_t<int const&, double>, double const&>);
static_assert(std::is_same_v<infect_qualifier_t<int const&&, double>, double const&&>);

static_assert(std::is_same_v<infect_qualifier_t<int, double const>, double const>);
static_assert(std::is_same_v<infect_qualifier_t<int&, double const>, double const&>);
static_assert(std::is_same_v<infect_qualifier_t<int&&, double const>, double const&&>);

static_assert(std::is_same_v<infect_qualifier_t<int const, double const>, double const>);
static_assert(std::is_same_v<infect_qualifier_t<int const&, double const>, double const&>);
static_assert(std::is_same_v<infect_qualifier_t<int const&&, double const>, double const&&>);

TEST_F(infect_qualifier_test, simple) {
    // only type traits
}

} // namespace util
