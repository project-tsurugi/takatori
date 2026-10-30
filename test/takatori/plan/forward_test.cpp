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
    EXPECT_EQ(s.ordering(), forward::ordering_type {});
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

TEST_F(forward_test, ordering) {
    forward s {
            row_slice { {}, 5 },
            {
                    forward_ordering::respect_input,
            },
    };
    EXPECT_EQ(s.columns().size(), 0);
    EXPECT_EQ(s.row_slice(), (row_slice { {}, 5 }));
    EXPECT_EQ(s.ordering(), (forward::ordering_type {
            forward_ordering::respect_input,
    }));
}

TEST_F(forward_test, ordering_columns) {
    forward s {
            {
                    vardesc(1),
                    vardesc(2),
            },
            {},
            {
                    forward_ordering::respect_input,
                    forward_ordering::preserve_output,
            },
    };
    EXPECT_EQ(s.columns().size(), 2);
    EXPECT_EQ(s.row_slice(), row_slice {});
    EXPECT_EQ(s.ordering(), (forward::ordering_type {
            forward_ordering::respect_input,
            forward_ordering::preserve_output,
    }));
}

TEST_F(forward_test, ordering_vector) {
    std::vector<descriptor::variable> columns {
            vardesc(1),
            vardesc(2),
    };
    forward s {
            std::move(columns),
            row_slice { {}, 5 },
            {
                    forward_ordering::preserve_output,
            },
    };
    EXPECT_EQ(s.columns().size(), 2);
    EXPECT_EQ(s.row_slice(), (row_slice { {}, 5 }));
    EXPECT_EQ(s.ordering(), (forward::ordering_type {
            forward_ordering::preserve_output,
    }));
}

TEST_F(forward_test, ordering_setter) {
    forward s {};
    s.ordering().insert(forward_ordering::respect_input);
    EXPECT_EQ(s.ordering(), (forward::ordering_type {
            forward_ordering::respect_input,
    }));
}

TEST_F(forward_test, equals_ordering) {
    forward a { row_slice { {}, 5 } };
    forward b { row_slice { {}, 5 }, { forward_ordering::respect_input } };
    forward c { row_slice { {}, 5 }, { forward_ordering::respect_input } };
    EXPECT_NE(a, b);
    EXPECT_EQ(b, c);
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
            {
                    forward_ordering::respect_input,
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
            {
                    forward_ordering::respect_input,
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
            {
                    forward_ordering::respect_input,
                    forward_ordering::preserve_output,
            },
    };

    std::cout << s << std::endl;
}

} // namespace takatori::plan
