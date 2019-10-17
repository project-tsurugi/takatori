#include "model/tree_element_util.h"

#include <gtest/gtest.h>

#include "generic_tree.h"

namespace model {

class tree_element_util_test : public ::testing::Test {
public:
    template<class T>
    static T const& make_const(T& object) { return object; }
};

TEST_F(tree_element_util_test, bless_element) {
    branch<int> b {
        std::make_unique<leaf<int>>(10),
        std::make_unique<leaf<int>>(20),
    };
    bless_element(b, *b.left());
    EXPECT_EQ(b.left()->parent_element(), &b);
}

TEST_F(tree_element_util_test, bless_element_ptr) {
    branch<int> b;
    b.left(bless_element(b, std::make_unique<leaf<int>>(10)));
    EXPECT_EQ(b.left()->parent_element(), &b);
}

TEST_F(tree_element_util_test, unbless_element) {
    branch<int> b {
            std::make_unique<leaf<int>>(10),
            std::make_unique<leaf<int>>(20),
    };
    bless_element(b, *b.left());
    bless_element(b, *b.right());

    unbless_element(*b.left());
    EXPECT_EQ(b.left()->parent_element(), nullptr);
}

TEST_F(tree_element_util_test, release_element_unique) {
    branch<int> b;
    auto l = std::make_unique<leaf<int>>(10);

    bless_element(b, *l);
    auto r = release_element(std::move(l));
    EXPECT_EQ(r->parent_element(), nullptr);
}

TEST_F(tree_element_util_test, assign_element) {
    branch<int> b;
    auto l = std::make_unique<leaf<int>>(10);
    assign_element(b, l, std::make_unique<leaf<int>>(20));
    EXPECT_EQ(l->value(), 20);
    EXPECT_EQ(l->parent_element(), &b);
}

} // namespace model
