#include "takatori/relation/step/take_cogroup.h"

#include <type_traits>

#include <gtest/gtest.h>

#include "../test_utils.h"

#include "takatori/util/clonable.h"

namespace takatori::relation::step {

class take_cogroup_test : public ::testing::Test {};

static_assert(take_cogroup::tag == expression_kind::take_cogroup);
static_assert(std::is_same_v<type_of_t<take_cogroup::tag>, take_cogroup>);
static_assert(!is_available_in_intermediate_plan(take_cogroup::tag));
static_assert(is_available_in_step_plan(take_cogroup::tag));

TEST_F(take_cogroup_test, simple) {
    take_cogroup expr {
            {
                    exchangedesc("e"),
                    {
                            { vardesc(1), vardesc(2) },
                    }
            }
    };

    ASSERT_EQ(expr.groups().size(), 1);
    {
        auto&& g = expr.groups()[0];
        EXPECT_EQ(g.source(), exchangedesc("e"));
        ASSERT_EQ(g.columns().size(), 1);
        {
            auto&& c = g.columns()[0];
            EXPECT_EQ(c.source(), vardesc(1));
            EXPECT_EQ(c.destination(), vardesc(2));
        }
        EXPECT_EQ(g.mandatory(), false);
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

TEST_F(take_cogroup_test, mandatory) {
    take_cogroup expr {
            {
                    exchangedesc("e"),
                    {
                            { vardesc(1), vardesc(2) },
                    },
                    true,
            }
    };

    ASSERT_EQ(expr.groups().size(), 1);
    {
        auto&& g = expr.groups()[0];
        EXPECT_EQ(g.source(), exchangedesc("e"));
        ASSERT_EQ(g.columns().size(), 1);
        {
            auto&& c = g.columns()[0];
            EXPECT_EQ(c.source(), vardesc(1));
            EXPECT_EQ(c.destination(), vardesc(2));
        }
        EXPECT_EQ(g.mandatory(), true);
    }
}

TEST_F(take_cogroup_test, multiple_mapping) {
    take_cogroup expr {
            {
                    exchangedesc("e"),
                    {
                            { vardesc(1), vardesc(2) },
                            { vardesc(3), vardesc(4) },
                            { vardesc(5), vardesc(6) },
                    }
            }
    };

    ASSERT_EQ(expr.groups().size(), 1);
    {
        auto&& g = expr.groups()[0];
        EXPECT_EQ(g.source(), exchangedesc("e"));
        ASSERT_EQ(g.columns().size(), 3);
        {
            auto&& c = g.columns()[0];
            EXPECT_EQ(c.source(), vardesc(1));
            EXPECT_EQ(c.destination(), vardesc(2));
        }
        {
            auto&& c = g.columns()[1];
            EXPECT_EQ(c.source(), vardesc(3));
            EXPECT_EQ(c.destination(), vardesc(4));
        }
        {
            auto&& c = g.columns()[2];
            EXPECT_EQ(c.source(), vardesc(5));
            EXPECT_EQ(c.destination(), vardesc(6));
        }
        EXPECT_EQ(g.mandatory(), false);
    }
}

TEST_F(take_cogroup_test, multiple_groups) {
    take_cogroup expr {
            {
                    exchangedesc("e1"),
                    {
                            { vardesc(1), vardesc(2) },
                    }
            },
            {
                    exchangedesc("e2"),
                    {
                            { vardesc(3), vardesc(4) },
                    }
            },
            {
                    exchangedesc("e3"),
                    {
                            { vardesc(5), vardesc(6) },
                    }
            },
    };

    ASSERT_EQ(expr.groups().size(), 3);
    {
        auto&& g = expr.groups()[0];
        EXPECT_EQ(g.source(), exchangedesc("e1"));
        ASSERT_EQ(g.columns().size(), 1);
        {
            auto&& c = g.columns()[0];
            EXPECT_EQ(c.source(), vardesc(1));
            EXPECT_EQ(c.destination(), vardesc(2));
        }
        EXPECT_EQ(g.mandatory(), false);
    }
    {
        auto&& g = expr.groups()[1];
        EXPECT_EQ(g.source(), exchangedesc("e2"));
        ASSERT_EQ(g.columns().size(), 1);
        {
            auto&& c = g.columns()[0];
            EXPECT_EQ(c.source(), vardesc(3));
            EXPECT_EQ(c.destination(), vardesc(4));
        }
        EXPECT_EQ(g.mandatory(), false);
    }
    {
        auto&& g = expr.groups()[2];
        EXPECT_EQ(g.source(), exchangedesc("e3"));
        ASSERT_EQ(g.columns().size(), 1);
        {
            auto&& c = g.columns()[0];
            EXPECT_EQ(c.source(), vardesc(5));
            EXPECT_EQ(c.destination(), vardesc(6));
        }
        EXPECT_EQ(g.mandatory(), false);
    }
}

TEST_F(take_cogroup_test, complex) {
    take_cogroup expr {
            {
                    exchangedesc("e1"),
                    {
                            { vardesc(1), vardesc(2) },
                    },
            },
            {
                    exchangedesc("e2"),
                    {
                            { vardesc(3), vardesc(4) },
                            { vardesc(13), vardesc(14) },
                    },
                    false,
            },
            {
                    exchangedesc("e3"),
                    {
                            { vardesc(5), vardesc(6) },
                            { vardesc(15), vardesc(16) },
                            { vardesc(25), vardesc(26) },
                    },
                    true,
            },
    };

    ASSERT_EQ(expr.groups().size(), 3);
    {
        auto&& g = expr.groups()[0];
        EXPECT_EQ(g.source(), exchangedesc("e1"));
        ASSERT_EQ(g.columns().size(), 1);
        {
            auto&& c = g.columns()[0];
            EXPECT_EQ(c.source(), vardesc(1));
            EXPECT_EQ(c.destination(), vardesc(2));
        }
        EXPECT_EQ(g.mandatory(), false);
    }
    {
        auto&& g = expr.groups()[1];
        EXPECT_EQ(g.source(), exchangedesc("e2"));
        ASSERT_EQ(g.columns().size(), 2);
        {
            auto&& c = g.columns()[0];
            EXPECT_EQ(c.source(), vardesc(3));
            EXPECT_EQ(c.destination(), vardesc(4));
        }
        {
            auto&& c = g.columns()[1];
            EXPECT_EQ(c.source(), vardesc(13));
            EXPECT_EQ(c.destination(), vardesc(14));
        }
        EXPECT_EQ(g.mandatory(), false);
    }
    {
        auto&& g = expr.groups()[2];
        EXPECT_EQ(g.source(), exchangedesc("e3"));
        ASSERT_EQ(g.columns().size(), 3);
        {
            auto&& c = g.columns()[0];
            EXPECT_EQ(c.source(), vardesc(5));
            EXPECT_EQ(c.destination(), vardesc(6));
        }
        {
            auto&& c = g.columns()[1];
            EXPECT_EQ(c.source(), vardesc(15));
            EXPECT_EQ(c.destination(), vardesc(16));
        }
        {
            auto&& c = g.columns()[2];
            EXPECT_EQ(c.source(), vardesc(25));
            EXPECT_EQ(c.destination(), vardesc(26));
        }
        EXPECT_EQ(g.mandatory(), true);
    }
}

TEST_F(take_cogroup_test, clone) {
    take_cogroup expr {
            {
                    exchangedesc("e1"),
                    {
                            { vardesc(1), vardesc(2) },
                    },
            },
            {
                    exchangedesc("e2"),
                    {
                            { vardesc(3), vardesc(4) },
                            { vardesc(13), vardesc(14) },
                    },
                    false,
            },
            {
                    exchangedesc("e3"),
                    {
                            { vardesc(5), vardesc(6) },
                            { vardesc(15), vardesc(16) },
                            { vardesc(25), vardesc(26) },
                    },
                    true,
            },
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(take_cogroup_test, clone_move) {
    take_cogroup expr {
            {
                    exchangedesc("e1"),
                    {
                            { vardesc(1), vardesc(2) },
                    },
            },
            {
                    exchangedesc("e2"),
                    {
                            { vardesc(3), vardesc(4) },
                            { vardesc(13), vardesc(14) },
                    },
                    false,
            },
            {
                    exchangedesc("e3"),
                    {
                            { vardesc(5), vardesc(6) },
                            { vardesc(15), vardesc(16) },
                            { vardesc(25), vardesc(26) },
                    },
                    true,
            },
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(take_cogroup_test, output) {
    take_cogroup expr {
            {
                    exchangedesc("e1"),
                    {
                            { vardesc(1), vardesc(2) },
                    },
            },
            {
                    exchangedesc("e2"),
                    {
                            { vardesc(3), vardesc(4) },
                            { vardesc(13), vardesc(14) },
                    },
                    false,
            },
            {
                    exchangedesc("e3"),
                    {
                            { vardesc(5), vardesc(6) },
                            { vardesc(15), vardesc(16) },
                            { vardesc(25), vardesc(26) },
                    },
                    true,
            },
    };

    std::cout << expr << std::endl;
}

} // namespace takatori::relation::step
