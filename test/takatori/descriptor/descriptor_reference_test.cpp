#include <takatori/descriptor/reference.h>

#include <type_traits>

#include <gtest/gtest.h>

#include <takatori/descriptor/element.h>
#include <takatori/descriptor/variable.h>
#include <takatori/testing/mock_binding_info.h>

namespace takatori::descriptor {

class descriptor_reference_test : public ::testing::Test {};

using takatori::testing::mock_binding_info;
using info = mock_binding_info<variable::tag, int>;

static_assert(std::is_trivially_copyable_v<variable::reference_type>);
static_assert(std::is_same_v<variable::reference_type::descriptor_type, variable>);
static_assert(variable::reference_type::tag == variable::tag);

TEST_F(descriptor_reference_test, simple) {
    variable d { std::make_shared<info>(1) };
    auto r = d.reference();
    EXPECT_EQ(decltype(r)::tag, variable::tag);
    EXPECT_EQ(r, d);
}

TEST_F(descriptor_reference_test, eq) {
    variable a { std::make_shared<info>(1) };
    variable b { std::make_shared<info>(1) };
    variable c { std::make_shared<info>(2) };
    variable e { std::shared_ptr<info> {} };
    variable f { std::shared_ptr<info> {} };

    EXPECT_EQ(a.reference(), b.reference());
    EXPECT_NE(a.reference(), c.reference());
    EXPECT_NE(a.reference(), e.reference());
    EXPECT_EQ(e.reference(), f.reference());

    EXPECT_EQ(a, b.reference());
    EXPECT_NE(a, c.reference());
    EXPECT_NE(a, e.reference());
    EXPECT_EQ(e, f.reference());

    EXPECT_EQ(a.reference(), b);
    EXPECT_NE(a.reference(), c);
    EXPECT_NE(a.reference(), e);
    EXPECT_EQ(e.reference(), f);
}

TEST_F(descriptor_reference_test, hash) {
    std::hash<variable> hd {};
    std::hash<reference<variable>> hr {};
    variable a { std::make_shared<info>(1) };
    variable b { std::make_shared<info>(1) };
    variable c { std::make_shared<info>(2) };
    variable e { std::shared_ptr<info> {} };
    variable f { std::shared_ptr<info> {} };

    EXPECT_EQ(hr(a.reference()), hr(b.reference()));
    EXPECT_NE(hr(a.reference()), hr(c.reference()));
    EXPECT_NE(hr(a.reference()), hr(e.reference()));
    EXPECT_EQ(hr(e.reference()), hr(f.reference()));

    EXPECT_EQ(hd(a), hr(b.reference()));
    EXPECT_NE(hd(a), hr(c.reference()));
    EXPECT_NE(hd(a), hr(e.reference()));
    EXPECT_EQ(hd(e), hr(f.reference()));
}

TEST_F(descriptor_reference_test, output) {
    variable d { std::make_shared<info>(1) };
    std::cout << d.reference() << std::endl;
}

} // namespace takatori::descriptor
