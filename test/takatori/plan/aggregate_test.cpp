#include <takatori/plan/aggregate.h>

#include <algorithm>

#include <gtest/gtest.h>

#include "test_utils.h"

#include <takatori/util/clonable.h>

namespace takatori::plan {

class aggregate_test : public ::testing::Test {};

static_assert(aggregate::tag == step_kind::aggregate);

TEST_F(aggregate_test, simple) {
    aggregate s;
    EXPECT_EQ(s.source_columns().size(), 0);
    EXPECT_EQ(s.destination_columns().size(), 0);
    EXPECT_TRUE(std::equal(
            s.input_columns().begin(), s.input_columns().end(),
            s.source_columns().begin(), s.source_columns().end()));
    EXPECT_TRUE(std::equal(
            s.output_columns().begin(), s.output_columns().end(),
            s.destination_columns().begin(), s.destination_columns().end()));
    EXPECT_EQ(s.group_keys().size(), 0);
    EXPECT_EQ(s.aggregations().size(), 0);
    EXPECT_EQ(s.mode(), group_mode::equivalence);
}

TEST_F(aggregate_test, group_keys) {
    aggregate s {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            {},
    };

    ASSERT_EQ(s.source_columns().size(), 3);
    EXPECT_EQ(s.source_columns()[0], vardesc(1));
    EXPECT_EQ(s.source_columns()[1], vardesc(2));
    EXPECT_EQ(s.source_columns()[2], vardesc(3));

    ASSERT_EQ(s.destination_columns().size(), 3);
    EXPECT_EQ(s.destination_columns()[0], vardesc(1));
    EXPECT_EQ(s.destination_columns()[1], vardesc(2));
    EXPECT_EQ(s.destination_columns()[2], vardesc(3));

    EXPECT_TRUE(std::equal(
            s.input_columns().begin(), s.input_columns().end(),
            s.source_columns().begin(), s.source_columns().end()));
    EXPECT_TRUE(std::equal(
            s.output_columns().begin(), s.output_columns().end(),
            s.destination_columns().begin(), s.destination_columns().end()));

    ASSERT_EQ(s.group_keys().size(), 3);
    EXPECT_EQ(s.group_keys()[0], vardesc(1));
    EXPECT_EQ(s.group_keys()[1], vardesc(2));
    EXPECT_EQ(s.group_keys()[2], vardesc(3));

    EXPECT_EQ(s.aggregations().size(), 0);
    EXPECT_EQ(s.mode(), group_mode::equivalence);
}

TEST_F(aggregate_test, aggregations) {
    aggregate s {
            {},
            {
                    { aggdesc("COUNT"), {}, vardesc(100) },
                    { aggdesc("MAX"), { vardesc(10) }, vardesc(200) },
                    { aggdesc("COVAR"), { vardesc(20), vardesc(30), }, vardesc(300) },
            },
    };

    ASSERT_EQ(s.source_columns().size(), 3);
    EXPECT_EQ(s.source_columns()[0], vardesc(10));
    EXPECT_EQ(s.source_columns()[1], vardesc(20));
    EXPECT_EQ(s.source_columns()[2], vardesc(30));

    ASSERT_EQ(s.destination_columns().size(), 3);
    EXPECT_EQ(s.destination_columns()[0], vardesc(100));
    EXPECT_EQ(s.destination_columns()[1], vardesc(200));
    EXPECT_EQ(s.destination_columns()[2], vardesc(300));

    EXPECT_TRUE(std::equal(
            s.input_columns().begin(), s.input_columns().end(),
            s.source_columns().begin(), s.source_columns().end()));
    EXPECT_TRUE(std::equal(
            s.output_columns().begin(), s.output_columns().end(),
            s.destination_columns().begin(), s.destination_columns().end()));
    EXPECT_EQ(s.group_keys().size(), 0);

    ASSERT_EQ(s.aggregations().size(), 3);
    {
        auto&& a = s.aggregations()[0];
        EXPECT_EQ(a.function(), aggdesc("COUNT"));
        ASSERT_EQ(a.arguments().size(), 0);
        EXPECT_EQ(a.destination(), vardesc(100));
    }
    {
        auto&& a = s.aggregations()[1];
        EXPECT_EQ(a.function(), aggdesc("MAX"));
        ASSERT_EQ(a.arguments().size(), 1);
        EXPECT_EQ(a.arguments()[0], vardesc(10));
        EXPECT_EQ(a.destination(), vardesc(200));
    }
    {
        auto&& a = s.aggregations()[2];
        EXPECT_EQ(a.function(), aggdesc("COVAR"));
        ASSERT_EQ(a.arguments().size(), 2);
        EXPECT_EQ(a.arguments()[0], vardesc(20));
        EXPECT_EQ(a.arguments()[1], vardesc(30));
        EXPECT_EQ(a.destination(), vardesc(300));
    }

    EXPECT_EQ(s.mode(), group_mode::equivalence);
}

TEST_F(aggregate_test, mode) {
    aggregate s {
            {},
            {
                    { aggdesc("COUNT"), {}, vardesc(1) },
            },
            group_mode::equivalence_or_whole,
    };

    EXPECT_EQ(s.source_columns().size(), 0);
    EXPECT_EQ(s.destination_columns().size(), 1);
    EXPECT_EQ(s.destination_columns()[0], vardesc(1));

    EXPECT_TRUE(std::equal(
            s.input_columns().begin(), s.input_columns().end(),
            s.source_columns().begin(), s.source_columns().end()));
    EXPECT_TRUE(std::equal(
            s.output_columns().begin(), s.output_columns().end(),
            s.destination_columns().begin(), s.destination_columns().end()));
    EXPECT_EQ(s.group_keys().size(), 0);
    EXPECT_EQ(s.aggregations().size(), 1);
    EXPECT_EQ(s.mode(), group_mode::equivalence_or_whole);
}

TEST_F(aggregate_test, complex) {
    aggregate s {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            {
                    { aggdesc("COUNT"), {}, vardesc(100) },
                    { aggdesc("MAX"), { vardesc(10) }, vardesc(200) },
                    { aggdesc("COVAR"), { vardesc(20), vardesc(30), }, vardesc(300) },
            },
            group_mode::equivalence_or_whole,
    };

    ASSERT_EQ(s.source_columns().size(), 6);
    EXPECT_EQ(s.source_columns()[0], vardesc(1));
    EXPECT_EQ(s.source_columns()[1], vardesc(2));
    EXPECT_EQ(s.source_columns()[2], vardesc(3));
    EXPECT_EQ(s.source_columns()[3], vardesc(10));
    EXPECT_EQ(s.source_columns()[4], vardesc(20));
    EXPECT_EQ(s.source_columns()[5], vardesc(30));

    ASSERT_EQ(s.destination_columns().size(), 6);
    EXPECT_EQ(s.destination_columns()[0], vardesc(1));
    EXPECT_EQ(s.destination_columns()[1], vardesc(2));
    EXPECT_EQ(s.destination_columns()[2], vardesc(3));
    EXPECT_EQ(s.destination_columns()[3], vardesc(100));
    EXPECT_EQ(s.destination_columns()[4], vardesc(200));
    EXPECT_EQ(s.destination_columns()[5], vardesc(300));

    EXPECT_TRUE(std::equal(
            s.input_columns().begin(), s.input_columns().end(),
            s.source_columns().begin(), s.source_columns().end()));
    EXPECT_TRUE(std::equal(
            s.output_columns().begin(), s.output_columns().end(),
            s.destination_columns().begin(), s.destination_columns().end()));

    ASSERT_EQ(s.group_keys().size(), 3);
    EXPECT_EQ(s.group_keys()[0], vardesc(1));
    EXPECT_EQ(s.group_keys()[1], vardesc(2));
    EXPECT_EQ(s.group_keys()[2], vardesc(3));

    EXPECT_EQ(s.aggregations().size(), 3);
    EXPECT_EQ(s.mode(), group_mode::equivalence_or_whole);
}

TEST_F(aggregate_test, clone) {
    aggregate s {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            {
                    { aggdesc("COUNT"), {}, vardesc(100) },
                    { aggdesc("MAX"), { vardesc(10) }, vardesc(200) },
                    { aggdesc("COVAR"), { vardesc(20), vardesc(30), }, vardesc(300) },
            },
            group_mode::equivalence_or_whole,
    };

    auto copy = util::clone_unique(s);
    EXPECT_EQ(s, *copy);
    EXPECT_NE(std::addressof(s), copy.get());
}

TEST_F(aggregate_test, clone_move) {
    aggregate s {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            {
                    { aggdesc("COUNT"), {}, vardesc(100) },
                    { aggdesc("MAX"), { vardesc(10) }, vardesc(200) },
                    { aggdesc("COVAR"), { vardesc(20), vardesc(30), }, vardesc(300) },
            },
            group_mode::equivalence_or_whole,
    };

    auto copy = util::clone_unique(s);
    EXPECT_EQ(s, *copy);
    EXPECT_NE(std::addressof(s), copy.get());

    auto move = util::clone_unique(std::move(s));
    EXPECT_NE(std::addressof(s), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(aggregate_test, output) {
    aggregate s {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            {
                    { aggdesc("COUNT"), {}, vardesc(100) },
                    { aggdesc("MAX"), { vardesc(10) }, vardesc(200) },
                    { aggdesc("COVAR"), { vardesc(20), vardesc(30), }, vardesc(300) },
            },
            group_mode::equivalence_or_whole,
    };

    std::cout << s << std::endl;
}

} // namespace takatori::plan
