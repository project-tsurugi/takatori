#include <takatori/plan/forward.h>

#include <gtest/gtest.h>

#include <stdexcept>
#include <type_traits>

#include "test_utils.h"

#include <takatori/util/clonable.h>

namespace takatori::plan {

class forward_test : public ::testing::Test {};

static_assert(forward::tag == step_kind::forward);

using relation::details::row_slice;

TEST_F(forward_test, simple) {
    forward s;
    EXPECT_EQ(s.columns().size(), 0);
    EXPECT_EQ(s.input_columns().size(), 0);
    EXPECT_EQ(s.output_columns().size(), 0);
    EXPECT_EQ(s.row_slice(), row_slice {});
}

TEST_F(forward_test, row_slice) {
    forward s {
            {
                    10,
                    5,
            },
    };
    EXPECT_EQ(s.columns().size(), 0);
    EXPECT_EQ(s.input_columns().size(), 0);
    EXPECT_EQ(s.output_columns().size(), 0);
    EXPECT_EQ(s.row_slice(), (row_slice {
            10,
            5,
    }));
}

// compatibility API: the row count must be an integral type
static_assert(std::is_same_v<forward::size_type, std::size_t>);
static_assert(std::is_constructible_v<forward, int>);
static_assert(std::is_constructible_v<forward, std::vector<descriptor::variable>, int>);
static_assert(!std::is_constructible_v<forward, std::nullopt_t>);
static_assert(!std::is_constructible_v<forward, std::optional<std::size_t>>);
static_assert(!std::is_constructible_v<forward, std::vector<descriptor::variable>, std::nullopt_t>);
static_assert(!std::is_constructible_v<forward, bool>);

TEST_F(forward_test, compat_limit) {
    forward s { 5 };
    EXPECT_EQ(s.columns().size(), 0);
    EXPECT_EQ(s.row_slice(), (row_slice { {}, 5 }));
    EXPECT_EQ(s.limit(), 5);
}

TEST_F(forward_test, compat_limit_columns) {
    forward s {
            {
                    vardesc(1),
                    vardesc(2),
            },
            5,
    };
    EXPECT_EQ(s.columns().size(), 2);
    EXPECT_EQ(s.row_slice(), (row_slice { {}, 5 }));
}

TEST_F(forward_test, compat_limit_vector) {
    std::vector<descriptor::variable> columns {
            vardesc(1),
            vardesc(2),
    };
    forward s { std::move(columns), 5 };
    EXPECT_EQ(s.columns().size(), 2);
    EXPECT_EQ(s.row_slice(), (row_slice { {}, 5 }));
}

TEST_F(forward_test, compat_limit_brace) {
    std::vector<descriptor::variable> columns {
            vardesc(1),
            vardesc(2),
    };
    // `{}` must be resolved as an empty row slice (unlimited), as same as `std::optional` in the previous API
    forward s { std::move(columns), {} };
    EXPECT_EQ(s.columns().size(), 2);
    EXPECT_EQ(s.row_slice(), row_slice {});
    EXPECT_EQ(s.limit(), std::nullopt);
}

TEST_F(forward_test, compat_limit_getter) {
    forward s {};
    EXPECT_EQ(s.limit(), std::nullopt);

    s.row_slice() = { 0, 5 };
    EXPECT_EQ(s.limit(), 5);

    s.row_slice() = { 1, 5 };
    EXPECT_THROW((void) s.limit(), std::domain_error);

    s.row_slice() = { vardesc(2), 5 };
    EXPECT_THROW((void) s.limit(), std::domain_error);

    s.row_slice() = { {}, vardesc(2) };
    EXPECT_THROW((void) s.limit(), std::domain_error);
}

TEST_F(forward_test, compat_limit_setter) {
    forward s { row_slice { 10, {} } };

    s.limit(5);
    EXPECT_EQ(s.row_slice(), (row_slice { 10, 5 }));

    s.limit(std::nullopt);
    EXPECT_EQ(s.row_slice(), (row_slice { 10, {} }));
}

TEST_F(forward_test, columns) {
    forward s {
            vardesc(1),
            vardesc(2),
            vardesc(3),
    };

    ASSERT_EQ(s.columns().size(), 3);
    {
        auto&& cs = s.columns();
        EXPECT_EQ(cs[0], vardesc(1));
        EXPECT_EQ(cs[1], vardesc(2));
        EXPECT_EQ(cs[2], vardesc(3));
    }

    ASSERT_EQ(s.input_columns().size(), 3);
    {
        auto&& cs = s.input_columns();
        EXPECT_EQ(cs[0], vardesc(1));
        EXPECT_EQ(cs[1], vardesc(2));
        EXPECT_EQ(cs[2], vardesc(3));
    }

    ASSERT_EQ(s.output_columns().size(), 3);
    {
        auto&& cs = s.output_columns();
        EXPECT_EQ(cs[0], vardesc(1));
        EXPECT_EQ(cs[1], vardesc(2));
        EXPECT_EQ(cs[2], vardesc(3));
    }

    EXPECT_EQ(s.row_slice(), row_slice {});
}

TEST_F(forward_test, clone) {
    forward s {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            {
                    10, 5,
            },
    };

    auto copy = util::clone_unique(s);
    EXPECT_EQ(s, *copy);
    EXPECT_NE(std::addressof(s), copy.get());
}

TEST_F(forward_test, clone_move) {
    forward s {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            {
                    10, 5,
            },
    };

    auto copy = util::clone_unique(s);
    EXPECT_EQ(s, *copy);
    EXPECT_NE(std::addressof(s), copy.get());

    auto move = util::clone_unique(std::move(s));
    EXPECT_NE(std::addressof(s), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(forward_test, output) {
    forward s {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            {
                    10, 5,
            },
    };

    std::cout << s << std::endl;
}

} // namespace takatori::plan
