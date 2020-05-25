#include <takatori/relation/step/offer.h>

#include <type_traits>

#include <gtest/gtest.h>

#include <takatori/relation/test_utils.h>

#include <takatori/util/clonable.h>

namespace takatori::relation::step {

class offer_test : public ::testing::Test {};

static_assert(offer::tag == expression_kind::offer);
static_assert(std::is_same_v<type_of_t<offer::tag>, offer>);
static_assert(!is_available_in_intermediate_plan(offer::tag));
static_assert(is_available_in_step_plan(offer::tag));

TEST_F(offer_test, simple) {
    offer expr {
            exchangedesc("e"),
            {
                    { vardesc(1), vardesc(2) },
            }
    };

    EXPECT_EQ(expr.destination(), exchangedesc("e"));
    ASSERT_EQ(expr.columns().size(), 1);
    {
        auto&& c = expr.columns()[0];
        EXPECT_EQ(c.source(), vardesc(1));
        EXPECT_EQ(c.destination(), vardesc(2));
    }

    EXPECT_EQ(&expr.input().owner(), &expr);

    {
        auto p = expr.input_ports();
        ASSERT_EQ(p.size(), 1);
        EXPECT_EQ(&p[0], &expr.input());
        EXPECT_TRUE(is_valid_port_list(p));
    }
    {
        auto p = expr.output_ports();
        ASSERT_EQ(p.size(), 0);
        EXPECT_TRUE(is_valid_port_list(p));
    }
}

TEST_F(offer_test, multiple) {
    offer expr {
            exchangedesc("e"),
            {
                    { vardesc(1), vardesc(2) },
                    { vardesc(3), vardesc(4) },
                    { vardesc(5), vardesc(6) },
            }
    };

    EXPECT_EQ(expr.destination(), exchangedesc("e"));
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

TEST_F(offer_test, clone) {
    offer expr {
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

TEST_F(offer_test, clone_move) {
    offer expr {
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

TEST_F(offer_test, output) {
    offer expr {
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
