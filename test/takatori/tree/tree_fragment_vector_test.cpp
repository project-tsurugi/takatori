#include <takatori/tree/tree_fragment_vector.h>

#include <gtest/gtest.h>

#include "generic_tree.h"

namespace takatori::tree {

class tree_fragment_vector_test : public ::testing::Test {
public:
    template<class T>
    static T& lv(T&& object) { return object; }

    template<class T>
    static T&& rv(T& object) { return std::move(object); }
};

namespace {

template<bool>
class copyable {};

template<>
class copyable<false> {
public:
    copyable() = default;
    ~copyable() = default;
    copyable(copyable const&) = delete;
    copyable& operator=(copyable const&) = delete;
    copyable(copyable&&) noexcept = default;
    copyable& operator=(copyable&&) noexcept = default;
};

template<class T, bool Copyable = true>
class C : public copyable<Copyable> {
public:
    C() = default;
    template<class U, class = std::enable_if_t<std::is_constructible_v<T, U>>>
    C(U&& v) : v_(std::forward<U>(v)) {} // NOLINT

    T& v() noexcept { return v_; }
    T const& v() const noexcept { return v_; }

    explicit operator T&() noexcept { return v_; }
    explicit operator T const&() const noexcept { return v_; }

    void parent_element(T* p) noexcept { p_ = p; }
    T const* parent_element() const noexcept { return p_; }

    bool operator==(C const& o) const noexcept { return v_ == o.v_; }
    bool operator!=(C const& o) const noexcept { return v_ != o.v_; }
    bool operator<(C const& o) const noexcept { return v_ < o.v_; }
    bool operator>(C const& o) const noexcept { return v_ > o.v_; }
    bool operator<=(C const& o) const noexcept { return v_ <= o.v_; }
    bool operator>=(C const& o) const noexcept { return v_ >= o.v_; }

private:
    T v_ {};
    T* p_ {};
};

template<class T>
C(T&&) -> C<std::remove_reference_t<T>>;

} // namespace

static_assert(is_tree_fragment_v<tree_fragment_vector<C<int>>>);
static_assert(std::is_same_v<
        typename tree_fragment_traits<tree_fragment_vector<C<int>>>::parent_type,
        typename tree_fragment_traits<C<int>>::parent_type>);

template<class T>
using entity_vector = std::vector<T, typename tree_fragment_vector<T>::allocator_type>;

TEST_F(tree_fragment_vector_test, simple) {
    int root = -1;
    tree_fragment_vector<C<int>> v { root, { 1, 2, 3 }};

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);

    EXPECT_EQ(v[0].parent_element(), &root);
    EXPECT_EQ(v[1].parent_element(), &root);
    EXPECT_EQ(v[2].parent_element(), &root);
}

TEST_F(tree_fragment_vector_test, ctor_object_creator) {
    int root = -1;
    tree_fragment_vector<C<int>> v { root, { 1, 2, 3 }};

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);

    EXPECT_EQ(v[0].parent_element(), &root);
    EXPECT_EQ(v[1].parent_element(), &root);
    EXPECT_EQ(v[2].parent_element(), &root);
}

TEST_F(tree_fragment_vector_test, ctor_vector) {
    int root = -1;
    entity_vector<C<int>> v0 { 1, 2, 3 };
    tree_fragment_vector v { root, std::move(v0) };

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);

    EXPECT_EQ(v[0].parent_element(), &root);
    EXPECT_EQ(v[1].parent_element(), &root);
    EXPECT_EQ(v[2].parent_element(), &root);
}

TEST_F(tree_fragment_vector_test, ctor_vector_creator) {
    util::pmr::monotonic_buffer_resource resource;

    int root = -1;
    using c = C<int, false>;
    std::vector<c> v0;
    v0.emplace_back(1);
    v0.emplace_back(2);
    v0.emplace_back(3);
    tree_fragment_vector<c> v { root, std::move(v0), util::object_creator { &resource } };

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);

    EXPECT_EQ(v[0].parent_element(), &root);
    EXPECT_EQ(v[1].parent_element(), &root);
    EXPECT_EQ(v[2].parent_element(), &root);
}

#ifdef ENABLE_OBJECT_CREATOR_PMR
TEST_F(tree_fragment_vector_test, ctor_vector_creator_compatible) {
    util::pmr::monotonic_buffer_resource resource;

    int root = -1;
    using c = C<int, false>;
    entity_vector<c> v0 { util::pmr::polymorphic_allocator<c>(&resource) };
    v0.emplace_back(1);
    v0.emplace_back(2);
    v0.emplace_back(3);
    auto* p0 = &v0[0];

    tree_fragment_vector<c> v { root, std::move(v0), util::object_creator { &resource } };
    EXPECT_EQ(&v[0], p0);

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);

    EXPECT_EQ(v[0].parent_element(), &root);
    EXPECT_EQ(v[1].parent_element(), &root);
    EXPECT_EQ(v[2].parent_element(), &root);
}

TEST_F(tree_fragment_vector_test, ctor_vector_creator_incompatible) {
    util::pmr::monotonic_buffer_resource resource;

    int root = -1;
    using c = C<int, false>;
    entity_vector<c> v0 { util::pmr::polymorphic_allocator<c>(util::pmr::new_delete_resource()) };
    v0.emplace_back(1);
    v0.emplace_back(2);
    v0.emplace_back(3);
    auto* p0 = &v0[0];

    tree_fragment_vector<c> v { root, std::move(v0), util::object_creator { &resource } };
    EXPECT_NE(&v[0], p0);

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);

    EXPECT_EQ(v[0].parent_element(), &root);
    EXPECT_EQ(v[1].parent_element(), &root);
    EXPECT_EQ(v[2].parent_element(), &root);
}
#endif // ENABLE_OBJECT_CREATOR_PMR

TEST_F(tree_fragment_vector_test, ctor_initializer_list) {
    int root = -1;
    using c = C<int>;
    tree_fragment_vector v { root, { c(1), c(2), c(3) } };

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);

    EXPECT_EQ(v[0].parent_element(), &root);
    EXPECT_EQ(v[1].parent_element(), &root);
    EXPECT_EQ(v[2].parent_element(), &root);
}

TEST_F(tree_fragment_vector_test, ctor_rvalue_initializer_list) {
    int root = -1;
    using c = C<int, false>;
    tree_fragment_vector v { root, util::rvalue_initializer_list<c> { c(1), c(2), c(3) } };

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);

    EXPECT_EQ(v[0].parent_element(), &root);
    EXPECT_EQ(v[1].parent_element(), &root);
    EXPECT_EQ(v[2].parent_element(), &root);
}

TEST_F(tree_fragment_vector_test, ctor_iter) {
    int root = -1;
    using c = C<int>;
    c c1(1), c2(2), c3(3);
    std::initializer_list<c> i { c1, c2, c3 };
    tree_fragment_vector v { root, i.begin(), i.end() };

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);

    EXPECT_EQ(v[0].parent_element(), &root);
    EXPECT_EQ(v[1].parent_element(), &root);
    EXPECT_EQ(v[2].parent_element(), &root);
}

TEST_F(tree_fragment_vector_test, ctor_iter_move) {
    int root = -1;
    using c = C<int, false>;
    c c1(1), c2(2), c3(3);
    util::rvalue_initializer_list<c> i { rv(c1), rv(c2), rv(c3) };
    tree_fragment_vector v { root, i.begin(), i.end() };

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);

    EXPECT_EQ(v[0].parent_element(), &root);
    EXPECT_EQ(v[1].parent_element(), &root);
    EXPECT_EQ(v[2].parent_element(), &root);
}

TEST_F(tree_fragment_vector_test, reparent) {
    int root = -1;
    tree_fragment_vector<C<int>> v { {}, { 1, 2, 3 }};

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);

    EXPECT_EQ(v[0].parent_element(), nullptr);
    EXPECT_EQ(v[1].parent_element(), nullptr);
    EXPECT_EQ(v[2].parent_element(), nullptr);

    v.parent_element(&root);
    EXPECT_EQ(v[0].parent_element(), &root);
    EXPECT_EQ(v[1].parent_element(), &root);
    EXPECT_EQ(v[2].parent_element(), &root);
}

TEST_F(tree_fragment_vector_test, at) {
    int root = -1;
    tree_fragment_vector<C<int>> v { root, { 1, 2, 3 }};

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v.at(0), 1);
    EXPECT_EQ(v.at(1), 2);
    EXPECT_EQ(v.at(2), 3);

    EXPECT_THROW(v.at(3), std::out_of_range);
}

TEST_F(tree_fragment_vector_test, at_const) {
    int root = -1;
    tree_fragment_vector<C<int>> v0 { root, { 1, 2, 3 }};
    auto&& v = std::as_const(v0);

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v.at(0), 1);
    EXPECT_EQ(v.at(1), 2);
    EXPECT_EQ(v.at(2), 3);

    EXPECT_THROW(v.at(3), std::out_of_range);
}

TEST_F(tree_fragment_vector_test, operator_at) {
    int root = -1;
    tree_fragment_vector<C<int>> v { root, { 1, 2, 3 }};

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST_F(tree_fragment_vector_test, operator_at_const) {
    int root = -1;
    tree_fragment_vector<C<int>> v0 { root, { 1, 2, 3 }};
    auto&& v = std::as_const(v0);

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST_F(tree_fragment_vector_test, front_back) {
    int root = -1;
    tree_fragment_vector<C<int>> v { root, { 1, 2, 3 }};

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v.front(), 1);
    EXPECT_EQ(v.back(), 3);
}

TEST_F(tree_fragment_vector_test, front_back_const) {
    int root = -1;
    tree_fragment_vector<C<int>> v0 { root, { 1, 2, 3 }};
    auto&& v = std::as_const(v0);

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v.front(), 1);
    EXPECT_EQ(v.back(), 3);
}

TEST_F(tree_fragment_vector_test, empty) {
    int root = -1;
    tree_fragment_vector<C<int>> v { root };
    EXPECT_TRUE(v.empty());

    v.emplace_back(1);
    EXPECT_FALSE(v.empty());
}

TEST_F(tree_fragment_vector_test, size) {
    int root = -1;
    tree_fragment_vector<C<int>> v { root };
    EXPECT_EQ(v.size(), 0);

    v.emplace_back(1);
    v.emplace_back(2);
    v.emplace_back(3);
    EXPECT_EQ(v.size(), 3);
}

TEST_F(tree_fragment_vector_test, reserve) {
    int root = -1;
    tree_fragment_vector<C<int>> v { root };
    v.reserve(100);
    EXPECT_GE(v.capacity(), 100);
}

TEST_F(tree_fragment_vector_test, clear) {
    int root = -1;
    tree_fragment_vector<C<int>> v { root, { 1, 2, 3 }};
    EXPECT_EQ(v.size(), 3);

    v.clear();
    EXPECT_EQ(v.size(), 0);
}

TEST_F(tree_fragment_vector_test, assign_initializer_list) {
    int root = -1;
    tree_fragment_vector<C<int>> v { root, { -1 }};
    v.assign({ 1, 2, 3 });

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);

    EXPECT_EQ(v[0].parent_element(), &root);
    EXPECT_EQ(v[1].parent_element(), &root);
    EXPECT_EQ(v[2].parent_element(), &root);
}

TEST_F(tree_fragment_vector_test, assign_rvalue_initializer_list) {
    int root = -1;
    using c = C<int, false>;
    tree_fragment_vector v { root, util::rvalue_initializer_list<c> { c(-1) }};
    v.assign(util::rvalue_initializer_list<c> { c(1), c(2), c(3) });

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);

    EXPECT_EQ(v[0].parent_element(), &root);
    EXPECT_EQ(v[1].parent_element(), &root);
    EXPECT_EQ(v[2].parent_element(), &root);
}

TEST_F(tree_fragment_vector_test, assign_iter) {
    int root = -1;
    using c = C<int>;
    c c1(1), c2(2), c3(3);
    std::initializer_list<c> i { c1, c2, c3 };

    tree_fragment_vector<c> v { root, { -1 }};
    v.assign(i.begin(), i.end());

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);

    EXPECT_EQ(v[0].parent_element(), &root);
    EXPECT_EQ(v[1].parent_element(), &root);
    EXPECT_EQ(v[2].parent_element(), &root);
}

TEST_F(tree_fragment_vector_test, assign_rvalue_iter) {
    int root = -1;
    using c = C<int, false>;
    c c1(1), c2(2), c3(3);
    util::rvalue_initializer_list<c> i { rv(c1), rv(c2), rv(c3) };

    tree_fragment_vector<c> v { root, util::rvalue_initializer_list<c>{ c(-1) }};
    v.assign(i.begin(), i.end());

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);

    EXPECT_EQ(v[0].parent_element(), &root);
    EXPECT_EQ(v[1].parent_element(), &root);
    EXPECT_EQ(v[2].parent_element(), &root);
}

TEST_F(tree_fragment_vector_test, pop_back) {
    int root = -1;
    using c = C<int>;
    tree_fragment_vector<c> v { root, { 1, 2, 3 } };
    v.pop_back();

    ASSERT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
}

TEST_F(tree_fragment_vector_test, insert) {
    int root = -1;
    using c = C<int>;
    tree_fragment_vector<c> v { root, { 1, 2, 3 }};

    auto&& iter = v.insert(v.begin() + 1, lv(c(-1)));

    ASSERT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], -1);
    EXPECT_EQ(v[2], 2);
    EXPECT_EQ(v[3], 3);

    EXPECT_EQ(&*iter, &v[1]);
    EXPECT_EQ(iter->parent_element(), &root);
}

TEST_F(tree_fragment_vector_test, insert_rvalue) {
    int root = -1;
    using c = C<int, false>;
    tree_fragment_vector<c> v { root, util::rvalue_initializer_list<c> { c(1), c(2), c(3) }};

    auto&& iter = v.insert(v.begin() + 1, c(-1));

    ASSERT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], -1);
    EXPECT_EQ(v[2], 2);
    EXPECT_EQ(v[3], 3);

    EXPECT_EQ(&*iter, &v[1]);
    EXPECT_EQ(iter->parent_element(), &root);
}

TEST_F(tree_fragment_vector_test, insert_initializer_list) {
    int root = -1;
    using c = C<int>;
    tree_fragment_vector<c> v { root, { 1, 2 }};

    auto&& iter = v.insert(v.begin() + 1, { -1, -2 } );

    ASSERT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], -1);
    EXPECT_EQ(v[2], -2);
    EXPECT_EQ(v[3], 2);

    EXPECT_EQ(&*iter, &v[1]);
    EXPECT_EQ(v[1].parent_element(), &root);
    EXPECT_EQ(v[2].parent_element(), &root);
}

TEST_F(tree_fragment_vector_test, insert_rvalue_initializer_list) {
    int root = -1;
    using c = C<int, false>;
    tree_fragment_vector<c> v { root, util::rvalue_initializer_list<c> { c(1), c(2) }};

    auto&& iter = v.insert(v.begin() + 1, util::rvalue_initializer_list<c> { c(-1), c(-2) } );

    ASSERT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], -1);
    EXPECT_EQ(v[2], -2);
    EXPECT_EQ(v[3], 2);

    EXPECT_EQ(&*iter, &v[1]);
    EXPECT_EQ(v[1].parent_element(), &root);
    EXPECT_EQ(v[2].parent_element(), &root);
}

TEST_F(tree_fragment_vector_test, insert_iter) {
    int root = -1;
    using c = C<int>;
    tree_fragment_vector<c> v { root, { 1, 2 } };

    c c1(-1), c2(-2);
    std::initializer_list<c> i { c1, c2 };
    auto&& iter = v.insert(v.begin() + 1, i.begin(), i.end());

    ASSERT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], -1);
    EXPECT_EQ(v[2], -2);
    EXPECT_EQ(v[3], 2);

    EXPECT_EQ(&*iter, &v[1]);
    EXPECT_EQ(v[1].parent_element(), &root);
    EXPECT_EQ(v[2].parent_element(), &root);
}

TEST_F(tree_fragment_vector_test, insert_iter_rvalue) {
    int root = -1;
    using c = C<int, false>;
    tree_fragment_vector<c> v { root, util::rvalue_initializer_list<c> { c(1), c(2) } };

    c c1(-1), c2(-2);
    util::rvalue_initializer_list<c> i { rv(c1), rv(c2) };
    auto&& iter = v.insert(v.begin() + 1, i.begin(), i.end());

    ASSERT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], -1);
    EXPECT_EQ(v[2], -2);
    EXPECT_EQ(v[3], 2);

    EXPECT_EQ(&*iter, &v[1]);
    EXPECT_EQ(v[1].parent_element(), &root);
    EXPECT_EQ(v[2].parent_element(), &root);
}

TEST_F(tree_fragment_vector_test, push_back) {
    int root = -1;
    using c = C<int>;
    tree_fragment_vector<c> v { root, { 1, 2, 3 } };

    auto&& result = v.push_back(-1);

    ASSERT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], -1);

    EXPECT_EQ(&result, &v[3]);
    EXPECT_EQ(v[3].parent_element(), &root);
}

TEST_F(tree_fragment_vector_test, push_back_rvalue) {
    int root = -1;
    using c = C<int, false>;
    tree_fragment_vector<c> v { root, util::rvalue_initializer_list<c> { c(1), c(2), c(3) } };

    auto&& result = v.push_back(c(-1));

    ASSERT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], -1);

    EXPECT_EQ(&result, &v[3]);
    EXPECT_EQ(v[3].parent_element(), &root);
}

TEST_F(tree_fragment_vector_test, emplace) {
    int root = -1;
    using c = C<int, false>;
    tree_fragment_vector<c> v { root, util::rvalue_initializer_list<c> { c(1), c(2), c(3) } };

    auto&& iter = v.emplace(v.begin() + 1, -1);

    ASSERT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], -1);
    EXPECT_EQ(v[2], 2);
    EXPECT_EQ(v[3], 3);

    EXPECT_EQ(&*iter, &v[1]);
    EXPECT_EQ(v[1].parent_element(), &root);
}

TEST_F(tree_fragment_vector_test, emplace_back) {
    int root = -1;
    using c = C<int, false>;
    tree_fragment_vector<c> v { root, util::rvalue_initializer_list<c> { c(1), c(2), c(3) } };

    auto&& result = v.emplace_back(-1);

    ASSERT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], -1);

    EXPECT_EQ(&result, &v[3]);
    EXPECT_EQ(v[3].parent_element(), &root);
}

TEST_F(tree_fragment_vector_test, release_elements) {
    int root = -1;
    using c = C<int, false>;
    tree_fragment_vector<c> v { root, util::rvalue_initializer_list<c>{ c(1), c(2), c(3) } };

    auto r = v.release_elements();

    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.get_object_creator(), util::object_creator(r.get_allocator()));

    ASSERT_EQ(r.size(), 3);
    EXPECT_EQ(r[0], 1);
    EXPECT_EQ(r[1], 2);
    EXPECT_EQ(r[2], 3);

    EXPECT_EQ(r[0].parent_element(), nullptr);
    EXPECT_EQ(r[1].parent_element(), nullptr);
    EXPECT_EQ(r[2].parent_element(), nullptr);
}

TEST_F(tree_fragment_vector_test, compare) {
    int root = -1;
    using c = C<int>;

    EXPECT_EQ(tree_fragment_vector<c>(root, { 10 }), tree_fragment_vector<c>(root, { 10 }));
    EXPECT_LE(tree_fragment_vector<c>(root, { 10 }), tree_fragment_vector<c>(root, { 10 }));
    EXPECT_GE(tree_fragment_vector<c>(root, { 10 }), tree_fragment_vector<c>(root, { 10 }));

    EXPECT_NE(tree_fragment_vector<c>(root, { 10, 20 }), tree_fragment_vector<c>(root, { 20, 10 }));

    EXPECT_NE(tree_fragment_vector<c>(root, { 10 }), tree_fragment_vector<c>(root, { 20 }));
    EXPECT_LT(tree_fragment_vector<c>(root, { 10 }), tree_fragment_vector<c>(root, { 20 }));
    EXPECT_LE(tree_fragment_vector<c>(root, { 10 }), tree_fragment_vector<c>(root, { 20 }));

    EXPECT_NE(tree_fragment_vector<c>(root, { 10 }), tree_fragment_vector<c>(root, { 10, 20 }));
    EXPECT_LT(tree_fragment_vector<c>(root, { 10 }), tree_fragment_vector<c>(root, { 10, 20 }));
    EXPECT_LE(tree_fragment_vector<c>(root, { 10 }), tree_fragment_vector<c>(root, { 10, 20 }));

    EXPECT_NE(tree_fragment_vector<c>(root, { 20 }), tree_fragment_vector<c>(root, { 10 }));
    EXPECT_GT(tree_fragment_vector<c>(root, { 20 }), tree_fragment_vector<c>(root, { 10 }));
    EXPECT_GE(tree_fragment_vector<c>(root, { 20 }), tree_fragment_vector<c>(root, { 10 }));

    EXPECT_NE(tree_fragment_vector<c>(root, { 10, 0 }), tree_fragment_vector<c>(root, { 10 }));
    EXPECT_GT(tree_fragment_vector<c>(root, { 10, 0 }), tree_fragment_vector<c>(root, { 10 }));
    EXPECT_GE(tree_fragment_vector<c>(root, { 10, 0 }), tree_fragment_vector<c>(root, { 10 }));
}

} // namespace takatori::tree
