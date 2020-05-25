#include <takatori/relation/step/take_group.h>

#include <type_traits>

#include <gtest/gtest.h>

#include <takatori/relation/test_utils.h>

#include <takatori/util/clonable.h>

namespace takatori::relation::step {

class take_group_test : public ::testing::Test {};

static_assert(take_group::tag == expression_kind::take_group);
static_assert(std::is_same_v<type_of_t<take_group::tag>, take_group>);
static_assert(!is_available_in_intermediate_plan(take_group::tag));
static_assert(is_available_in_step_plan(take_group::tag));

TEST_F(take_group_test, simple) {
    take_group expr {
            exchangedesc("e"),
            {
                    { vardesc(1), vardesc(2) },
            }
    };

    EXPECT_EQ(expr.source(), exchangedesc("e"));
    ASSERT_EQ(expr.columns().size(), 1);
    {
        auto&& c = expr.columns()[0];
        EXPECT_EQ(c.source(), vardesc(1));
        EXPECT_EQ(c.destination(), vardesc(2));
    }

    EXPECT_EQ(&expr.output().owner(), &expr);

    {
        auto p = expr.input_ports();
        ASSERT_EQ(p.size(), 0);
    }
    {
        auto p = expr.output_ports();
        ASSERT_EQ(p.size(), 1);
        EXPECT_EQ(&p[0], &expr.output());
        EXPECT_TRUE(is_valid_port_list(p));
    }
}

TEST_F(take_group_test, multiple) {
    take_group expr {
            exchangedesc("e"),
            {
                    { vardesc(1), vardesc(2) },
                    { vardesc(3), vardesc(4) },
                    { vardesc(5), vardesc(6) },
            }
    };

    EXPECT_EQ(expr.source(), exchangedesc("e"));
    ASSERT_EQ(expr.columns().size(), 3);
    {
        auto&& c = expr.columns()[0];
        EXPECT_EQ(c.source(), vardesc(1));
        EXPECT_EQ(c.destination(), vardesc(2));
    }
    {
        auto&& c = expr.columns()[1];
        EXPECT_EQ(c.source(), vardesc(3));
        EXPECT_EQ(c.destination(), vardesc(4));
    }
    {
        auto&& c = expr.columns()[2];
        EXPECT_EQ(c.source(), vardesc(5));
        EXPECT_EQ(c.destination(), vardesc(6));
    }
}

TEST_F(take_group_test, clone) {
    take_group expr {
            exchangedesc("e"),
            {
                    { vardesc(1), vardesc(2) },
                    { vardesc(3), vardesc(4) },
                    { vardesc(5), vardesc(6) },
            }
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(take_group_test, clone_move) {
    take_group expr {
            exchangedesc("e"),
            {
                    { vardesc(1), vardesc(2) },
                    { vardesc(3), vardesc(4) },
                    { vardesc(5), vardesc(6) },
            }
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(take_group_test, output) {
    take_group expr {
            exchangedesc("e"),
            {
                    { vardesc(1), vardesc(2) },
                    { vardesc(3), vardesc(4) },
                    { vardesc(5), vardesc(6) },
            }
    };

    std::cout << expr << std::endl;
}

} // namespace takatori::relation::step
