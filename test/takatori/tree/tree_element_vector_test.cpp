#include <takatori/tree/tree_element_vector.h>

#include <gtest/gtest.h>

#include "generic_tree.h"

namespace takatori::tree {

class tree_element_vector_test : public ::testing::Test {
public:
    template<class T, class... Elements>
    static tree_element_vector<node<T>>
    make_vector(node<T>& parent, Elements&&... elements) {
        tree_element_vector<node<T>> result { parent };
        result.reserve(sizeof...(elements));
        (..., result.template emplace_back<leaf<T>>(std::forward<Elements>(elements)));
        return result;
    }

    template<class T>
    static auto get(node<T> const& node) {
        return node.template cast<leaf>().value();
    }
};

TEST_F(tree_element_vector_test, simple) {
    branch<int> root;
    tree_element_vector<node<int>> v { root };

    v.reserve(3);
    v.emplace_back<leaf<int>>(10);
    v.emplace_back<leaf<int>>(20);
    v.emplace_back<leaf<int>>(30);

    EXPECT_EQ(get(v[0]), 10);
    EXPECT_EQ(get(v[1]), 20);
    EXPECT_EQ(get(v[2]), 30);

    EXPECT_EQ(v[0].parent_element(), &root);
    EXPECT_EQ(v[1].parent_element(), &root);
    EXPECT_EQ(v[2].parent_element(), &root);
}

TEST_F(tree_element_vector_test, ctor_vector) {
    branch<int> root;
    util::reference_vector<leaf<int>> rv;
    rv.emplace_back(10);
    rv.emplace_back(20);
    rv.emplace_back(30);

    tree_element_vector<node<int>> v { root, std::move(rv) };

    EXPECT_EQ(get(v[0]), 10);
    EXPECT_EQ(get(v[1]), 20);
    EXPECT_EQ(get(v[2]), 30);

    EXPECT_EQ(v[0].parent_element(), &root);
    EXPECT_EQ(v[1].parent_element(), &root);
    EXPECT_EQ(v[2].parent_element(), &root);
}

TEST_F(tree_element_vector_test, assign_vector) {
    branch<int> root;
    util::reference_vector<leaf<int>> rv;
    rv.emplace_back(10);
    rv.emplace_back(20);
    rv.emplace_back(30);

    tree_element_vector<node<int>> v { root };
    v = std::move(rv);

    EXPECT_EQ(get(v[0]), 10);
    EXPECT_EQ(get(v[1]), 20);
    EXPECT_EQ(get(v[2]), 30);

    EXPECT_EQ(v[0].parent_element(), &root);
    EXPECT_EQ(v[1].parent_element(), &root);
    EXPECT_EQ(v[2].parent_element(), &root);
}

TEST_F(tree_element_vector_test, at) {
    branch<int> root;
    auto v = make_vector(root, 10, 20, 30);

    EXPECT_EQ(get(v.at(0)), 10);
    EXPECT_EQ(get(v.at(1)), 20);
    EXPECT_EQ(get(v.at(2)), 30);

    EXPECT_THROW((void) v.at(3), std::out_of_range);
}

TEST_F(tree_element_vector_test, at_const) {
    branch<int> root;
    auto v = make_vector(root, 10, 20, 30);
    auto&& cv = std::as_const(v);

    EXPECT_EQ(get(cv.at(0)), 10);
    EXPECT_EQ(get(cv.at(1)), 20);
    EXPECT_EQ(get(cv.at(2)), 30);

    EXPECT_THROW((void) cv.at(3), std::out_of_range);
}

TEST_F(tree_element_vector_test, operator_at) {
    branch<int> root;
    auto v = make_vector(root, 10, 20, 30);

    EXPECT_EQ(get(v[0]), 10);
    EXPECT_EQ(get(v[1]), 20);
    EXPECT_EQ(get(v[2]), 30);
}

TEST_F(tree_element_vector_test, operator_at_const) {
    branch<int> root;
    auto v = make_vector(root, 10, 20, 30);
    auto&& cv = std::as_const(v);

    EXPECT_EQ(get(cv[0]), 10);
    EXPECT_EQ(get(cv[1]), 20);
    EXPECT_EQ(get(cv[2]), 30);
}

TEST_F(tree_element_vector_test, front_back) {
    branch<int> root;
    auto v = make_vector(root, 10, 20, 30);

    EXPECT_EQ(get(v.front()), 10);
    EXPECT_EQ(get(v.back()), 30);
}

TEST_F(tree_element_vector_test, front_back_const) {
    branch<int> root;
    auto v = make_vector(root, 10, 20, 30);
    auto&& cv = std::as_const(v);

    EXPECT_EQ(get(cv.front()), 10);
    EXPECT_EQ(get(cv.back()), 30);
}

TEST_F(tree_element_vector_test, empty) {
    branch<int> root;
    auto v = make_vector(root);
    auto&& cv = std::as_const(v);
    EXPECT_TRUE(cv.empty());

    v.emplace_back<leaf<int>>(1);
    EXPECT_FALSE(cv.empty());

    v.emplace_back<leaf<int>>(2);
    EXPECT_FALSE(cv.empty());
}

TEST_F(tree_element_vector_test, size) {
    branch<int> root;
    auto v = make_vector(root);
    auto&& cv = std::as_const(v);
    EXPECT_EQ(cv.size(), 0);

    v.emplace_back<leaf<int>>(1);
    EXPECT_EQ(cv.size(), 1);

    v.emplace_back<leaf<int>>(2);
    EXPECT_EQ(cv.size(), 2);
}

TEST_F(tree_element_vector_test, clear) {
    branch<int> root;
    auto v = make_vector(root, 10, 20, 30);
    EXPECT_FALSE(v.empty());

    v.clear();
    EXPECT_TRUE(v.empty());
}

TEST_F(tree_element_vector_test, assign) {
    branch<int> root;
    util::reference_vector<leaf<int>> rv;
    rv.emplace_back(10);
    rv.emplace_back(20);
    rv.emplace_back(30);

    tree_element_vector<node<int>> v { root };
    v.assign(std::move(rv));

    EXPECT_EQ(get(v[0]), 10);
    EXPECT_EQ(get(v[1]), 20);
    EXPECT_EQ(get(v[2]), 30);

    EXPECT_EQ(v[0].parent_element(), &root);
    EXPECT_EQ(v[1].parent_element(), &root);
    EXPECT_EQ(v[2].parent_element(), &root);
}

TEST_F(tree_element_vector_test, erase) {
    branch<int> root;
    auto v = make_vector(root, 10, 20, 30);
    auto iter = v.erase(v.cbegin() + 1);

    ASSERT_EQ(v.size(), 2);
    EXPECT_EQ(get(v[0]), 10);
    EXPECT_EQ(get(v[1]), 30);

    EXPECT_EQ(iter, v.begin() + 1);
}

TEST_F(tree_element_vector_test, erase_range) {
    branch<int> root;
    auto v = make_vector(root, 10, 20, 30, 40, 50);
    auto iter = v.erase(v.cbegin() + 1, v.cbegin() + 4);

    ASSERT_EQ(v.size(), 2);
    EXPECT_EQ(get(v[0]), 10);
    EXPECT_EQ(get(v[1]), 50);

    EXPECT_EQ(iter, v.begin() + 1);
}

TEST_F(tree_element_vector_test, pop_back) {
    branch<int> root;
    auto v = make_vector(root, 10, 20, 30);
    v.pop_back();

    ASSERT_EQ(v.size(), 2);
    EXPECT_EQ(get(v[0]), 10);
    EXPECT_EQ(get(v[1]), 20);
}

TEST_F(tree_element_vector_test, insert) {
    branch<int> root;
    auto v = make_vector(root, 10, 20, 30);
    auto iter = v.insert(v.begin() + 1, std::make_unique<leaf<int>>(-1));

    ASSERT_EQ(v.size(), 4);
    EXPECT_EQ(get(v[0]), 10);
    EXPECT_EQ(get(v[1]), -1);
    EXPECT_EQ(get(v[2]), 20);
    EXPECT_EQ(get(v[3]), 30);

    EXPECT_EQ(iter, v.begin() + 1);
    EXPECT_EQ(iter->parent_element(), &root);
}

TEST_F(tree_element_vector_test, emplace) {
    branch<int> root;
    auto v = make_vector(root, 10, 20, 30);
    auto iter = v.template emplace<leaf<int>>(v.begin() + 1, -1);

    ASSERT_EQ(v.size(), 4);
    EXPECT_EQ(get(v[0]), 10);
    EXPECT_EQ(get(v[1]), -1);
    EXPECT_EQ(get(v[2]), 20);
    EXPECT_EQ(get(v[3]), 30);

    EXPECT_EQ(iter, v.begin() + 1);
    EXPECT_EQ(iter->parent_element(), &root);
}

TEST_F(tree_element_vector_test, emplace_back) {
    branch<int> root;
    auto v = make_vector(root, 10, 20, 30);
    auto&& r = v.template emplace_back<leaf<int>>(-1);

    ASSERT_EQ(v.size(), 4);
    EXPECT_EQ(get(v[0]), 10);
    EXPECT_EQ(get(v[1]), 20);
    EXPECT_EQ(get(v[2]), 30);
    EXPECT_EQ(get(v[3]), -1);

    EXPECT_EQ(&r, &v[3]);
    EXPECT_EQ(r.parent_element(), &root);
}

TEST_F(tree_element_vector_test, put) {
    branch<int> root;
    auto v = make_vector(root, 10, 20, 30);
    auto&& r = v.put(v.begin() + 1, std::make_unique<leaf<int>>(-1));

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(get(v[0]), 10);
    EXPECT_EQ(get(v[1]), -1);
    EXPECT_EQ(get(v[2]), 30);

    EXPECT_EQ(&r, &v[1]);
    EXPECT_EQ(r.parent_element(), &root);
}

TEST_F(tree_element_vector_test, replace) {
    branch<int> root;
    auto v = make_vector(root, 10, 20, 30);
    auto&& r = v.template replace<leaf<int>>(v.begin() + 1, -1);

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(get(v[0]), 10);
    EXPECT_EQ(get(v[1]), -1);
    EXPECT_EQ(get(v[2]), 30);

    EXPECT_EQ(&r, &v[1]);
    EXPECT_EQ(r.parent_element(), &root);
}

TEST_F(tree_element_vector_test, release) {
    branch<int> root;
    auto v = make_vector(root, 10, 20, 30);
    auto&& [r, iter] = v.release(v.begin() + 1);

    ASSERT_EQ(v.size(), 2);
    EXPECT_EQ(get(v[0]), 10);
    EXPECT_EQ(get(v[1]), 30);

    EXPECT_EQ(get(*r), 20);
    EXPECT_EQ(r->parent_element(), nullptr);

    EXPECT_EQ(iter, v.begin() + 1);
}

TEST_F(tree_element_vector_test, release_range) {
    branch<int> root;
    auto v = make_vector(root, 10, 20, 30, 40, 50);
    auto&& [r, iter] = v.release(v.begin() + 1, v.begin() + 4);

    ASSERT_EQ(v.size(), 2);
    EXPECT_EQ(get(v[0]), 10);
    EXPECT_EQ(get(v[1]), 50);

    ASSERT_EQ(r.size(), 3);
    EXPECT_EQ(get(r[0]), 20);
    EXPECT_EQ(get(r[1]), 30);
    EXPECT_EQ(get(r[2]), 40);

    EXPECT_EQ(r[0].parent_element(), nullptr);
    EXPECT_EQ(r[1].parent_element(), nullptr);
    EXPECT_EQ(r[2].parent_element(), nullptr);

    EXPECT_EQ(iter, v.begin() + 1);
}

TEST_F(tree_element_vector_test, release_back) {
    branch<int> root;
    auto v = make_vector(root, 10, 20, 30);
    auto&& r = v.release_back();

    ASSERT_EQ(v.size(), 2);
    EXPECT_EQ(get(v[0]), 10);
    EXPECT_EQ(get(v[1]), 20);

    EXPECT_EQ(get(*r), 30);
    EXPECT_EQ(r->parent_element(), nullptr);
}

TEST_F(tree_element_vector_test, release_elements) {
    branch<int> root;
    auto v = make_vector(root, 10, 20, 30);
    auto r = v.release_elements();

    ASSERT_EQ(v.size(), 0);

    ASSERT_EQ(r.size(), 3);
    EXPECT_EQ(get(r[0]), 10);
    EXPECT_EQ(get(r[1]), 20);
    EXPECT_EQ(get(r[2]), 30);

    EXPECT_EQ(r[0].parent_element(), nullptr);
    EXPECT_EQ(r[1].parent_element(), nullptr);
    EXPECT_EQ(r[2].parent_element(), nullptr);
}

TEST_F(tree_element_vector_test, ownership) {
    branch<int> root;
    auto v = make_vector(root, 10, 20, 30);
    auto o = v.ownership(v.begin() + 1);

    EXPECT_EQ(get(*o), 20);

    auto old = o.exchange(std::make_unique<leaf<int>>(-1));
    EXPECT_EQ(get(*o), -1);
    EXPECT_EQ(get(*old), 20);
    EXPECT_EQ(o->parent_element(), &root);
}

TEST_F(tree_element_vector_test, compare) {
    branch<int> root;

    EXPECT_EQ(make_vector(root, 10), make_vector(root, 10));
    EXPECT_LE(make_vector(root, 10), make_vector(root, 10));
    EXPECT_GE(make_vector(root, 10), make_vector(root, 10));

    EXPECT_NE(make_vector(root, 10, 20), make_vector(root, 20, 10));

    EXPECT_NE(make_vector(root, 10), make_vector(root, 20));
    EXPECT_LT(make_vector(root, 10), make_vector(root, 20));
    EXPECT_LE(make_vector(root, 10), make_vector(root, 20));

    EXPECT_NE(make_vector(root, 10), make_vector(root, 10, 20));
    EXPECT_LT(make_vector(root, 10), make_vector(root, 10, 20));
    EXPECT_LE(make_vector(root, 10), make_vector(root, 10, 20));

    EXPECT_NE(make_vector(root, 20), make_vector(root, 10));
    EXPECT_GT(make_vector(root, 20), make_vector(root, 10));
    EXPECT_GE(make_vector(root, 20), make_vector(root, 10));

    EXPECT_NE(make_vector(root, 10, 0), make_vector(root, 10));
    EXPECT_GT(make_vector(root, 10, 0), make_vector(root, 10));
    EXPECT_GE(make_vector(root, 10, 0), make_vector(root, 10));
}

} // namespace takatori::tree
