#include <takatori/descriptor/element.h>

#include <type_traits>

#include <gtest/gtest.h>

#include <takatori/descriptor/variable.h>
#include <takatori/testing/descriptors.h>

namespace takatori::descriptor {

class descriptor_element_test : public ::testing::Test {};

using takatori::testing::mock_binding_info;
using info = mock_binding_info<variable::tag, int>;
using takatori::testing::value_of;

TEST_F(descriptor_element_test, simple) {
    variable d { std::make_shared<info>(1) };
    EXPECT_EQ(decltype(d)::tag, variable::tag);
    EXPECT_EQ(value_of<int>(d), 1);
}

TEST_F(descriptor_element_test, optional_entity) {
    variable d { std::make_shared<info>(1) };
    EXPECT_EQ(d.optional_entity().get(), std::addressof(d.entity()));
}

TEST_F(descriptor_element_test, optional_entity_empty) {
    variable e { std::shared_ptr<info> {} };
    EXPECT_FALSE(e.optional_entity());
}

TEST_F(descriptor_element_test, shared_entity) {
    variable d { std::make_shared<info>(1) };
    EXPECT_EQ(d.shared_entity().get(), std::addressof(d.entity()));
}

TEST_F(descriptor_element_test, eq) {
    variable a { std::make_shared<info>(1) };
    variable b { std::make_shared<info>(1) };
    variable c { std::make_shared<info>(2) };
    variable e { std::shared_ptr<info> {} };
    variable f { std::shared_ptr<info> {} };
    EXPECT_EQ(a, b);
    EXPECT_NE(a, c);
    EXPECT_NE(a, e);
    EXPECT_EQ(e, f);
}

TEST_F(descriptor_element_test, hash) {
    std::hash<variable> h {};
    variable a { std::make_shared<info>(1) };
    variable b { std::make_shared<info>(1) };
    variable c { std::make_shared<info>(2) };
    variable e { std::shared_ptr<info> {} };
    variable f { std::shared_ptr<info> {} };
    EXPECT_EQ(h(a), h(b));
    EXPECT_NE(h(a), h(c));
    EXPECT_NE(h(a), h(e));
    EXPECT_EQ(h(e), h(f));
}

TEST_F(descriptor_element_test, output) {
    variable d { std::make_shared<info>(1) };
    std::cout << d << std::endl;
}

} // namespace takatori::descriptor
