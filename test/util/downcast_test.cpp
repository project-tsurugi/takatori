#include "util/downcast.h"

#include <memory>

#include <gtest/gtest.h>

namespace util {

class downcast_test : public ::testing::Test {};

struct Base {
    Base() = default;
    virtual ~Base() = default;
};

struct Sub : Base {
    Sub() = default;
    ~Sub() override = default;
};

TEST_F(downcast_test, lvalue) {
    std::unique_ptr<Base> sub = std::make_unique<Sub>();
    auto&& s = downcast<Sub>(*sub);
    static_assert(std::is_same_v<decltype(s), Sub&>);
}

TEST_F(downcast_test, lvalue_const) {
    std::unique_ptr<Base const> sub = std::make_unique<Sub>();
    auto&& s = downcast<Sub>(*sub);
    static_assert(std::is_same_v<decltype(s), Sub const&>);
}

TEST_F(downcast_test, rvalue) {
    std::unique_ptr<Base> sub = std::make_unique<Sub>();
    auto&& s = dynamic_cast<Sub&&>(*sub);
    static_assert(std::is_same_v<decltype(s), Sub&&>);
}

} // namespace util
