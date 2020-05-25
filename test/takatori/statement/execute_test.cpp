#include <takatori/statement/execute.h>

#include <type_traits>

#include <gtest/gtest.h>

#include <takatori/plan/forward.h>
#include <takatori/util/clonable.h>

namespace takatori::statement {

class execute_test : public ::testing::Test {};

static_assert(!std::is_copy_constructible_v<execute>);
static_assert(!std::is_copy_assignable_v<execute>);
static_assert(!std::is_move_constructible_v<execute>);
static_assert(!std::is_move_assignable_v<execute>);

TEST_F(execute_test, simple) {
    execute stmt {};
    EXPECT_EQ(stmt.kind(), statement_kind::execute);
    EXPECT_EQ(stmt.execution_plan().size(), 0);
}

TEST_F(execute_test, graph) {
    plan::graph_type g;
    auto&& s0 = g.insert(plan::forward {});

    execute stmt { std::move(g) };
    EXPECT_EQ(stmt.execution_plan().size(), 1);
    EXPECT_TRUE(stmt.execution_plan().contains(s0));
}

TEST_F(execute_test, eq) {
    execute s0 {};
    execute s1 {};
    s0.execution_plan().insert(plan::forward {});
    s1.execution_plan().insert(plan::forward {});

    EXPECT_EQ(s0, s0);
    EXPECT_NE(s0, s1);
}

TEST_F(execute_test, clone) {
    execute stmt {};
    auto&& s0 = stmt.execution_plan().insert(plan::forward {});

    auto copy = util::clone_unique(stmt);
    EXPECT_NE(stmt, *copy);
    EXPECT_EQ(copy->execution_plan().size(), 1);
    EXPECT_FALSE(copy->execution_plan().contains(s0));
    EXPECT_EQ(copy->execution_plan().begin()->kind(), plan::forward::tag);
}

TEST_F(execute_test, clone_move) {
    execute stmt {};
    auto&& s0 = stmt.execution_plan().insert(plan::forward {});

    auto copy = util::clone_unique(std::move(stmt));
    EXPECT_EQ(copy->execution_plan().size(), 1);
    EXPECT_TRUE(copy->execution_plan().contains(s0));
}

TEST_F(execute_test, output) {
    execute stmt {};
    stmt.execution_plan().insert(plan::forward {});

    std::cout << stmt << std::endl;
}

} // namespace takatori::statement
