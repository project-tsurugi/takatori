#include <takatori/relation/scan.h>

#include <stdexcept>
#include <type_traits>

#include <gtest/gtest.h>

#include "test_utils.h"

#include <takatori/util/clonable.h>

namespace takatori::relation {

class scan_test : public ::testing::Test {};

static_assert(scan::tag == expression_kind::scan);
static_assert(std::is_same_v<type_of_t<scan::tag>, scan>);

using details::row_slice;

TEST_F(scan_test, simple) {
    scan expr {
            tabledesc("T"),
            {
                    {
                            columndesc("C1"),
                            vardesc(1),
                    },
            },
            {
                    scan::key {
                            columndesc("C2"),
                            constant(0),
                    },
                    endpoint_kind::inclusive,
            },
            {
                    scan::key {
                            columndesc("C2"),
                            constant(100),
                    },
                    endpoint_kind::exclusive,
            },
    };

    EXPECT_EQ(&expr.output().owner(), &expr);
    EXPECT_EQ(expr.source(), tabledesc("T"));
    ASSERT_EQ(expr.columns().size(), 1);
    {
        auto&& c = expr.columns()[0];
        EXPECT_EQ(c.source(), columndesc("C1"));
        EXPECT_EQ(c.destination(), vardesc(1));
    }
    {
        auto&& b = expr.lower();
        ASSERT_EQ(b.keys().size(), 1);
        {
            auto&& k = b.keys()[0];
            EXPECT_EQ(k.variable(), columndesc("C2"));
            EXPECT_EQ(k.value(), constant(0));
            EXPECT_EQ(k.value().parent_element(), &expr);
        }
        EXPECT_EQ(b.kind(), endpoint_kind::inclusive);
    }
    {
        auto&& b = expr.upper();
        ASSERT_EQ(b.keys().size(), 1);
        {
            auto&& k = b.keys()[0];
            EXPECT_EQ(k.variable(), columndesc("C2"));
            EXPECT_EQ(k.value(), constant(100));
            EXPECT_EQ(k.value().parent_element(), &expr);
        }
        EXPECT_EQ(b.kind(), endpoint_kind::exclusive);
    }
    EXPECT_EQ(expr.row_slice(), row_slice {});

    {
        auto p = expr.input_ports();
        ASSERT_EQ(p.size(), 0);
        EXPECT_TRUE(is_valid_port_list(p));
    }
    {
        auto p = expr.output_ports();
        ASSERT_EQ(p.size(), 1);
        EXPECT_EQ(&p[0], &expr.output());
        EXPECT_TRUE(is_valid_port_list(p));
    }
}

TEST_F(scan_test, full) {
    scan expr {
            tabledesc("T"),
            {
                    {
                            columndesc("C1"),
                            vardesc(1),
                    },
            },
    };

    EXPECT_EQ(&expr.output().owner(), &expr);
    EXPECT_EQ(expr.source(), tabledesc("T"));
    ASSERT_EQ(expr.columns().size(), 1);
    {
        auto&& c = expr.columns()[0];
        EXPECT_EQ(c.source(), columndesc("C1"));
        EXPECT_EQ(c.destination(), vardesc(1));
    }
    {
        auto&& b = expr.lower();
        EXPECT_FALSE(b);
        ASSERT_EQ(b.keys().size(), 0);
        EXPECT_EQ(b.kind(), endpoint_kind::unbound);
    }
    {
        auto&& b = expr.upper();
        EXPECT_FALSE(b);
        ASSERT_EQ(b.keys().size(), 0);
        EXPECT_EQ(b.kind(), endpoint_kind::unbound);
    }
}

TEST_F(scan_test, row_slice) {
    scan expr {
            tabledesc("T"),
            {
                    {
                            columndesc("C1"),
                            vardesc(1),
                    },
            },
            {},
            {},
            {},
            {
                    10,
                    5,
            },
    };

    EXPECT_EQ(expr.row_slice(), (row_slice {
            10,
            5,
    }));
}

TEST_F(scan_test, direction) {
    scan expr {
            tabledesc("T"),
            {
                    {
                            columndesc("C1"),
                            vardesc(1),
                    },
            },
            {},
            {},
            scan_direction::backward,
            {
                    {},
                    5,
            },
    };

    EXPECT_EQ(expr.direction(), scan_direction::backward);
    EXPECT_EQ(expr.row_slice(), (row_slice { {}, 5 }));
}

TEST_F(scan_test, direction_default) {
    scan expr {
            tabledesc("T"),
            {
                    {
                            columndesc("C1"),
                            vardesc(1),
                    },
            },
    };

    EXPECT_EQ(expr.direction(), std::nullopt);

    expr.direction() = scan_direction::forward;
    EXPECT_EQ(expr.direction(), scan_direction::forward);

    expr.direction() = std::nullopt;
    EXPECT_EQ(expr.direction(), std::nullopt);
}

TEST_F(scan_test, direction_compare) {
    scan a {
            tabledesc("T"),
            {
                    {
                            columndesc("C1"),
                            vardesc(1),
                    },
            },
            {},
            {},
            scan_direction::forward,
    };
    scan b {
            tabledesc("T"),
            {
                    {
                            columndesc("C1"),
                            vardesc(1),
                    },
            },
            {},
            {},
            scan_direction::forward,
    };
    scan c {
            tabledesc("T"),
            {
                    {
                            columndesc("C1"),
                            vardesc(1),
                    },
            },
            {},
            {},
            scan_direction::backward,
    };
    scan d {
            tabledesc("T"),
            {
                    {
                            columndesc("C1"),
                            vardesc(1),
                    },
            },
    };

    EXPECT_EQ(a, b);
    EXPECT_NE(a, c);
    EXPECT_NE(a, d);
}

TEST_F(scan_test, direction_clone) {
    scan expr {
            tabledesc("T"),
            {
                    {
                            columndesc("C1"),
                            vardesc(1),
                    },
            },
            {},
            {},
            scan_direction::backward,
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(copy->direction(), scan_direction::backward);
    EXPECT_EQ(expr, *copy);

    auto move = util::clone_unique(std::move(expr));
    EXPECT_EQ(move->direction(), scan_direction::backward);
    EXPECT_EQ(*copy, *move);
}

TEST_F(scan_test, direction_output) {
    scan expr {
            tabledesc("T"),
            {
                    {
                            columndesc("C1"),
                            vardesc(1),
                    },
            },
            {},
            {},
            scan_direction::forward,
    };

    std::cout << expr << std::endl;
}

TEST_F(scan_test, multiple) {
    scan expr {
            tabledesc("T"),
            {
                    {
                            columndesc("C1"),
                            vardesc(1),
                    },
                    {
                            columndesc("C2"),
                            vardesc(2),
                    },
                    {
                            columndesc("C3"),
                            vardesc(3),
                    },
            },
            {
                    {
                            scan::key {
                                    columndesc("C4"),
                                    constant(4),
                            },
                            scan::key {
                                    columndesc("C5"),
                                    constant(5),
                            },
                            scan::key {
                                    columndesc("C6"),
                                    constant(6),
                            },
                    },
                    endpoint_kind::prefixed_exclusive,
            },
            {
                    {
                            scan::key {
                                    columndesc("C4"),
                                    constant(4),
                            },
                            scan::key {
                                    columndesc("C5"),
                                    constant(5),
                            },
                    },
                    endpoint_kind::prefixed_inclusive,
            },
    };

    EXPECT_EQ(&expr.output().owner(), &expr);
    EXPECT_EQ(expr.source(), tabledesc("T"));
    ASSERT_EQ(expr.columns().size(), 3);
    {
        auto&& c = expr.columns()[0];
        EXPECT_EQ(c.source(), columndesc("C1"));
        EXPECT_EQ(c.destination(), vardesc(1));
    }
    {
        auto&& c = expr.columns()[1];
        EXPECT_EQ(c.source(), columndesc("C2"));
        EXPECT_EQ(c.destination(), vardesc(2));
    }
    {
        auto&& c = expr.columns()[2];
        EXPECT_EQ(c.source(), columndesc("C3"));
        EXPECT_EQ(c.destination(), vardesc(3));
    }
    {
        auto&& b = expr.lower();
        EXPECT_TRUE(b);
        ASSERT_EQ(b.keys().size(), 3);
        {
            auto&& k = b.keys()[0];
            EXPECT_EQ(k.variable(), columndesc("C4"));
            EXPECT_EQ(k.value(), constant(4));
            EXPECT_EQ(k.value().parent_element(), &expr);
        }
        {
            auto&& k = b.keys()[1];
            EXPECT_EQ(k.variable(), columndesc("C5"));
            EXPECT_EQ(k.value(), constant(5));
            EXPECT_EQ(k.value().parent_element(), &expr);
        }
        {
            auto&& k = b.keys()[2];
            EXPECT_EQ(k.variable(), columndesc("C6"));
            EXPECT_EQ(k.value(), constant(6));
            EXPECT_EQ(k.value().parent_element(), &expr);
        }
        EXPECT_EQ(b.kind(), endpoint_kind::prefixed_exclusive);
    }
    {
        auto&& b = expr.upper();
        EXPECT_TRUE(b);
        ASSERT_EQ(b.keys().size(), 2);
        {
            auto&& k = b.keys()[0];
            EXPECT_EQ(k.variable(), columndesc("C4"));
            EXPECT_EQ(k.value(), constant(4));
            EXPECT_EQ(k.value().parent_element(), &expr);
        }
        {
            auto&& k = b.keys()[1];
            EXPECT_EQ(k.variable(), columndesc("C5"));
            EXPECT_EQ(k.value(), constant(5));
            EXPECT_EQ(k.value().parent_element(), &expr);
        }
        EXPECT_EQ(b.kind(), endpoint_kind::prefixed_inclusive);
    }
}

TEST_F(scan_test, clone) {
    scan expr {
            tabledesc("T"),
            {
                    {
                            columndesc("C1"),
                            vardesc(1),
                    },
            },
            {
                    scan::key {
                            columndesc("C2"),
                            constant(0),
                    },
                    endpoint_kind::inclusive,
            },
            {
                    scan::key {
                            columndesc("C2"),
                            constant(100),
                    },
                    endpoint_kind::exclusive,
            },
            {},
            {
                    10,
                    5,
            },
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(scan_test, clone_move) {
    scan expr {
            tabledesc("T"),
            {
                    scan::column {
                            columndesc("C1"),
                            vardesc(1),
                    },
            },
            {
                    scan::key {
                            columndesc("C2"),
                            constant(0),
                    },
                    endpoint_kind::inclusive,
            },
            {
                    scan::key {
                            columndesc("C2"),
                            constant(100),
                    },
                    endpoint_kind::exclusive,
            },
            {},
            {
                    10,
                    5,
            },
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(scan_test, output) {
    scan expr {
            tabledesc("T"),
            {
                    scan::column {
                            columndesc("C1"),
                            vardesc(1),
                    },
            },
            {
                    scan::key {
                            columndesc("C2"),
                            constant(0),
                    },
                    endpoint_kind::inclusive,
            },
            {
                    scan::key {
                            columndesc("C2"),
                            constant(100),
                    },
                    endpoint_kind::exclusive,
            },
            {},
            {
                    10,
                    5,
            },
    };

    std::cout << expr << std::endl;
}

} // namespace takatori::relation
