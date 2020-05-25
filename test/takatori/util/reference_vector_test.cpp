#include <takatori/util/reference_vector.h>

#include <gtest/gtest.h>

namespace takatori::util {

class reference_vector_test : public ::testing::Test {
public:
    template<class... Elements>
    static reference_vector<std::remove_reference_t<std::common_type_t<Elements...>>>
    make_vector(util::object_creator creator, Elements&&... elements) {
        reference_vector<int> result { creator };
        result.reserve(sizeof...(Elements));
        (..., result.template emplace_back<Elements>(std::forward<Elements>(elements)));
        return result;
    }
};

namespace {

struct Base {
    Base() = default;
    virtual ~Base() = default;
    virtual Base* clone(object_creator) const = 0;
    virtual std::string const& get() const = 0;
};

struct Sub : Base {
    explicit Sub(std::string value) : value_(std::move(value)) {}
    ~Sub() override = default;
    Sub* clone(object_creator allocator) const override { return allocator.create_object<Sub>(value_); };
    std::string const& get() const override { return value_; }

private:
    std::string value_;
};

} // namespace

static_assert(std::is_base_of_v<null_copier<std::unique_ptr<int>>, typename reference_vector<std::unique_ptr<int>>::copier_type>);
static_assert(std::is_base_of_v<standard_copier<int>, typename reference_vector<int>::copier_type>);
static_assert(std::is_base_of_v<clonable_copier<Base>, typename reference_vector<Base>::copier_type>);
static_assert(std::is_base_of_v<clonable_copier<Sub>, typename reference_vector<Sub>::copier_type>);

static_assert(!std::is_copy_constructible_v<reference_vector<int, null_copier<int>>>);
static_assert(!std::is_copy_assignable_v<reference_vector<int, null_copier<int>>>);
static_assert(std::is_move_constructible_v<reference_vector<int, null_copier<int>>>);
static_assert(std::is_move_assignable_v<reference_vector<int, null_copier<int>>>);

static_assert(std::is_copy_constructible_v<reference_vector<int>>);
static_assert(std::is_copy_assignable_v<reference_vector<int>>);
static_assert(std::is_move_constructible_v<reference_vector<int>>);
static_assert(std::is_move_assignable_v<reference_vector<int>>);

static_assert(std::is_copy_constructible_v<reference_vector<Base>>);
static_assert(std::is_copy_assignable_v<reference_vector<Base>>);
static_assert(std::is_move_constructible_v<reference_vector<Base>>);
static_assert(std::is_move_assignable_v<reference_vector<Base>>);

static_assert(std::is_copy_constructible_v<reference_vector<Sub>>);
static_assert(std::is_copy_assignable_v<reference_vector<Sub>>);
static_assert(std::is_move_constructible_v<reference_vector<Sub>>);
static_assert(std::is_move_assignable_v<reference_vector<Sub>>);

TEST_F(reference_vector_test, simple) {
    reference_vector<int> v;
    v.emplace_back(1);
    v.emplace_back(2);
    v.emplace_back(3);

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST_F(reference_vector_test, polymorphic) {
    reference_vector<Base, null_copier<Base>> v;
    v.emplace_back<Sub>("A");
    v.emplace_back<Sub>("B");
    v.emplace_back<Sub>("C");

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0].get(), "A");
    EXPECT_EQ(v[1].get(), "B");
    EXPECT_EQ(v[2].get(), "C");
}

TEST_F(reference_vector_test, copy) {
    reference_vector<int> v;
    v.emplace_back(1);
    v.emplace_back(2);
    v.emplace_back(3);

    reference_vector<int> c { v };
    ASSERT_EQ(c.size(), 3);
    EXPECT_EQ(c[0], 1);
    EXPECT_EQ(c[1], 2);
    EXPECT_EQ(c[2], 3);

    EXPECT_NE(&c[0], &v[0]);
    EXPECT_NE(&c[1], &v[1]);
    EXPECT_NE(&c[2], &v[2]);
}

TEST_F(reference_vector_test, copy_assign) {
    reference_vector<int> v;
    v.emplace_back(1);
    v.emplace_back(2);
    v.emplace_back(3);

    reference_vector<int> c;
    ASSERT_EQ(c.size(), 0);
    c = v;

    ASSERT_EQ(c.size(), 3);
    EXPECT_EQ(c[0], 1);
    EXPECT_EQ(c[1], 2);
    EXPECT_EQ(c[2], 3);

    EXPECT_NE(&c[0], &v[0]);
    EXPECT_NE(&c[1], &v[1]);
    EXPECT_NE(&c[2], &v[2]);
}

TEST_F(reference_vector_test, clone) {
    reference_vector<Base> v;
    v.emplace_back<Sub>("A");
    v.emplace_back<Sub>("B");
    v.emplace_back<Sub>("C");

    reference_vector<Base> c { v };
    ASSERT_EQ(c.size(), 3);
    EXPECT_EQ(c[0].get(), "A");
    EXPECT_EQ(c[1].get(), "B");
    EXPECT_EQ(c[2].get(), "C");

    EXPECT_NE(&c[0], &v[0]);
    EXPECT_NE(&c[1], &v[1]);
    EXPECT_NE(&c[2], &v[2]);
}

TEST_F(reference_vector_test, clone_assign) {
    reference_vector<Base> v;
    v.emplace_back<Sub>("A");
    v.emplace_back<Sub>("B");
    v.emplace_back<Sub>("C");

    reference_vector<Base> c;
    ASSERT_EQ(c.size(), 0);
    c = v;

    ASSERT_EQ(c.size(), 3);
    EXPECT_EQ(c[0].get(), "A");
    EXPECT_EQ(c[1].get(), "B");
    EXPECT_EQ(c[2].get(), "C");

    EXPECT_NE(&c[0], &v[0]);
    EXPECT_NE(&c[1], &v[1]);
    EXPECT_NE(&c[2], &v[2]);
}

TEST_F(reference_vector_test, move) {
    reference_vector<int> v;
    v.emplace_back(1);
    v.emplace_back(2);
    v.emplace_back(3);

    std::array<int*, 3> ps { &v[0], &v[1], &v[2] };

    reference_vector<int> c { std::move(v) };
    EXPECT_TRUE(v.empty());

    ASSERT_EQ(c.size(), 3);
    EXPECT_EQ(c[0], 1);
    EXPECT_EQ(c[1], 2);
    EXPECT_EQ(c[2], 3);

    EXPECT_EQ(&c[0], ps[0]);
    EXPECT_EQ(&c[1], ps[1]);
    EXPECT_EQ(&c[2], ps[2]);
}

TEST_F(reference_vector_test, move_null_copier) {
    reference_vector<int, null_copier<int>> v;
    v.emplace_back(1);
    v.emplace_back(2);
    v.emplace_back(3);

    std::array<int*, 3> ps { &v[0], &v[1], &v[2] };

    reference_vector<int> c { std::move(v) };
    EXPECT_TRUE(v.empty());

    ASSERT_EQ(c.size(), 3);
    EXPECT_EQ(c[0], 1);
    EXPECT_EQ(c[1], 2);
    EXPECT_EQ(c[2], 3);

    EXPECT_EQ(&c[0], ps[0]);
    EXPECT_EQ(&c[1], ps[1]);
    EXPECT_EQ(&c[2], ps[2]);
}

TEST_F(reference_vector_test, move_assign) {
    reference_vector<int> v;
    v.emplace_back(1);
    v.emplace_back(2);
    v.emplace_back(3);

    std::array<int*, 3> ps { &v[0], &v[1], &v[2] };

    reference_vector<int> c;
    ASSERT_EQ(c.size(), 0);

    c = std::move(v);
    EXPECT_TRUE(v.empty());

    ASSERT_EQ(c.size(), 3);
    EXPECT_EQ(c[0], 1);
    EXPECT_EQ(c[1], 2);
    EXPECT_EQ(c[2], 3);

    EXPECT_EQ(&c[0], ps[0]);
    EXPECT_EQ(&c[1], ps[1]);
    EXPECT_EQ(&c[2], ps[2]);
}

TEST_F(reference_vector_test, move_assign_null_copier) {
    reference_vector<int, null_copier<int>> v;
    v.emplace_back(1);
    v.emplace_back(2);
    v.emplace_back(3);

    std::array<int*, 3> ps { &v[0], &v[1], &v[2] };

    reference_vector<int> c;
    ASSERT_EQ(c.size(), 0);

    c = std::move(v);
    EXPECT_TRUE(v.empty());

    ASSERT_EQ(c.size(), 3);
    EXPECT_EQ(c[0], 1);
    EXPECT_EQ(c[1], 2);
    EXPECT_EQ(c[2], 3);

    EXPECT_EQ(&c[0], ps[0]);
    EXPECT_EQ(&c[1], ps[1]);
    EXPECT_EQ(&c[2], ps[2]);
}

TEST_F(reference_vector_test, custom_creator) {
    pmr::monotonic_buffer_resource resource;
    object_creator custom { &resource };

    reference_vector<int> v { custom };

    EXPECT_EQ(v.get_object_creator(), custom);

    v.emplace_back(1);
    v.emplace_back(2);
    v.emplace_back(3);

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST_F(reference_vector_test, initializer_list) {
    reference_vector v { 1, 2 };

    ASSERT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
}

TEST_F(reference_vector_test, initializer_list_ref) {
    Sub a { "A" };
    Sub b { "B" };
    Sub c { "C" };

    reference_vector<Base> v { std::ref(a), std::ref(b), std::ref(c) };

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0].get(), "A");
    EXPECT_EQ(v[1].get(), "B");
    EXPECT_EQ(v[2].get(), "C");
}

TEST_F(reference_vector_test, conversion) {
    static_assert(std::is_constructible_v<reference_vector<Base>, reference_vector<Sub>>);
    static_assert(!std::is_constructible_v<reference_vector<Sub>, reference_vector<Base>>);

    static_assert(std::is_assignable_v<Base*&, Sub*>);

    reference_vector<Sub> v;
    v.emplace_back<Sub>("A");
    v.emplace_back<Sub>("B");
    v.emplace_back<Sub>("C");
    std::array<Base*, 3> ps { &v[0], &v[1], &v[2] };

    reference_vector<Base> c { std::move(v) };
    ASSERT_EQ(c.size(), 3);
    EXPECT_EQ(&c[0], ps[0]);
    EXPECT_EQ(&c[1], ps[1]);
    EXPECT_EQ(&c[2], ps[2]);
}

TEST_F(reference_vector_test, conversion_assign) {
    static_assert(std::is_constructible_v<reference_vector<Base>, reference_vector<Sub>>);
    static_assert(!std::is_constructible_v<reference_vector<Sub>, reference_vector<Base>>);

    static_assert(std::is_assignable_v<Base*&, Sub*>);

    reference_vector<Sub> v;
    v.emplace_back<Sub>("A");
    v.emplace_back<Sub>("B");
    v.emplace_back<Sub>("C");

    std::array<Sub*, 3> ps { &v[0], &v[1], &v[2] };

    reference_vector<Base> c;
    c.emplace_back<Sub>("D");
    c = std::move(v);

    ASSERT_EQ(c.size(), 3);
    EXPECT_EQ(&c[0], ps[0]);
    EXPECT_EQ(&c[1], ps[1]);
    EXPECT_EQ(&c[2], ps[2]);
}

TEST_F(reference_vector_test, copy_with_creator) {
    reference_vector<int> v;
    v.emplace_back(1);
    v.emplace_back(2);
    v.emplace_back(3);

    pmr::monotonic_buffer_resource r;
    object_creator custom { &r };

    reference_vector<int> c { v, custom };
    EXPECT_EQ(c.get_object_creator(), custom);
    ASSERT_EQ(c.size(), 3);
    EXPECT_EQ(c[0], 1);
    EXPECT_EQ(c[1], 2);
    EXPECT_EQ(c[2], 3);

    EXPECT_NE(&c[0], &v[0]);
    EXPECT_NE(&c[1], &v[1]);
    EXPECT_NE(&c[2], &v[2]);
}

TEST_F(reference_vector_test, move_with_creator) {
    reference_vector<int> v;
    v.emplace_back(1);
    v.emplace_back(2);
    v.emplace_back(3);

    pmr::monotonic_buffer_resource r;
    object_creator custom { &r };

    reference_vector<int> c { std::move(v), custom };
    EXPECT_EQ(c.get_object_creator(), custom);
    ASSERT_EQ(c.size(), 3);
    EXPECT_EQ(c[0], 1);
    EXPECT_EQ(c[1], 2);
    EXPECT_EQ(c[2], 3);

    EXPECT_TRUE(v.empty()); // NOLINT
}

TEST_F(reference_vector_test, move_with_creator_compatible) {
    pmr::monotonic_buffer_resource r;
    object_creator custom { &r };

    reference_vector<int> v { custom };
    v.emplace_back(1);
    v.emplace_back(2);
    v.emplace_back(3);

    reference_vector<int> c { std::move(v), custom };
    EXPECT_EQ(c.get_object_creator(), custom);
    ASSERT_EQ(c.size(), 3);
    EXPECT_EQ(c[0], 1);
    EXPECT_EQ(c[1], 2);
    EXPECT_EQ(c[2], 3);

    EXPECT_TRUE(v.empty()); // NOLINT
}

TEST_F(reference_vector_test, at) {
    reference_vector v { 1, 2, 3 };

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v.at(0), 1);
    EXPECT_EQ(v.at(1), 2);
    EXPECT_EQ(v.at(2), 3);
    EXPECT_THROW(v.at(3), std::out_of_range);
}

TEST_F(reference_vector_test, at_const) {
    reference_vector const v { 1, 2, 3 };

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v.at(0), 1);
    EXPECT_EQ(v.at(1), 2);
    EXPECT_EQ(v.at(2), 3);
    EXPECT_THROW(v.at(3), std::out_of_range);
}

TEST_F(reference_vector_test, operator_at) {
    reference_vector v { 1, 2, 3 };

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST_F(reference_vector_test, operator_at_const) {
    reference_vector const v { 1, 2, 3 };

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST_F(reference_vector_test, front_back) {
    reference_vector v { 1, 2, 3 };

    EXPECT_EQ(v.front(), 1);
    EXPECT_EQ(v.back(), 3);
}

TEST_F(reference_vector_test, front_back_const) {
    reference_vector const v { 1, 2, 3 };

    EXPECT_EQ(v.front(), 1);
    EXPECT_EQ(v.back(), 3);
}

TEST_F(reference_vector_test, empty) {
    reference_vector<int> const v0 {};
    EXPECT_TRUE(v0.empty());

    reference_vector<int> v1;
    EXPECT_TRUE(v1.empty());

    v1.push_back(1);
    EXPECT_FALSE(v1.empty());
}

TEST_F(reference_vector_test, size) {
    reference_vector<int> const v0 { 1, 2, 3 };
    EXPECT_EQ(v0.size(), 3);

    reference_vector<int> v1;
    EXPECT_EQ(v1.size(), 0);

    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);
    EXPECT_EQ(v1.size(), 3);
}

TEST_F(reference_vector_test, capacity) {
    reference_vector<int> const v0 { 1, 2, 3 };
    EXPECT_GE(v0.capacity(), 3);

    reference_vector<int> v1;
    v1.reserve(100);
    EXPECT_GE(v1.capacity(), 100);
}

TEST_F(reference_vector_test, shrink_to_fit) {
    reference_vector<int> v;
    v.reserve(100);
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    v.shrink_to_fit();
}

TEST_F(reference_vector_test, clear) {
    reference_vector<int> v { 1, 2, 3 };
    v.clear();
    EXPECT_TRUE(v.empty());
}

TEST_F(reference_vector_test, assign) {
    reference_vector<int> v;
    v.push_back(0);

    v.assign({ 1, 2, 3 });

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST_F(reference_vector_test, assign_iter) {
    reference_vector<int> v;
    v.push_back(0);

    std::initializer_list<int> il { 1, 2, 3 };
    v.assign(il.begin(), il.end());

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST_F(reference_vector_test, erase) {
    reference_vector<int> v { 1, 2, 3 };
    auto iter = v.erase(v.begin() + 1);

    ASSERT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 3);

    EXPECT_EQ(v.begin() + 1, iter);
}

TEST_F(reference_vector_test, erase_range) {
    reference_vector<int> v { 1, 2, 3, 4, 5 };
    auto iter = v.erase(v.begin() + 1, v.begin() + 4);

    ASSERT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 5);

    EXPECT_EQ(v.begin() + 1, iter);
}

TEST_F(reference_vector_test, erase_range_empty) {
    reference_vector<int> v { 1, 2, 3 };
    auto iter = v.erase(v.begin() + 1, v.begin() + 1);

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);

    EXPECT_EQ(v.begin() + 1, iter);
}

TEST_F(reference_vector_test, pop_back) {
    reference_vector<int> v { 1, 2, 3 };
    v.pop_back();

    ASSERT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
}

TEST_F(reference_vector_test, insert) {
    reference_vector<int> v { 1, 2, 3 };
    int const i = -1;
    auto iter = v.insert(v.begin() + 2, i);

    ASSERT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], -1);
    EXPECT_EQ(v[3], 3);

    EXPECT_EQ(iter, v.begin() + 2);
}

TEST_F(reference_vector_test, insert_rvalue) {
    reference_vector<std::vector<int>> v;
    v.emplace_back(std::initializer_list<int> { 1 });
    v.emplace_back(std::initializer_list<int> { 2, 3 });

    std::vector<int> e { -1 };
    auto iter = v.insert(v.begin() + 1, std::move(e));

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], (std::vector { 1 }));
    EXPECT_EQ(v[1], (std::vector { -1 }));
    EXPECT_EQ(v[2], (std::vector { 2, 3 }));

    EXPECT_TRUE(e.empty());

    EXPECT_EQ(iter, v.begin() + 1);
}

TEST_F(reference_vector_test, insert_unique) {
    reference_vector<int> v { 1, 2, 3 };
    auto x = v.get_object_creator().create_unique<int>(-1);
    auto*p = x.get();
    auto iter = v.insert(v.begin() + 2, std::move(x));

    ASSERT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], -1);
    EXPECT_EQ(v[3], 3);

    EXPECT_EQ(&v[2], p);

    EXPECT_EQ(iter, v.begin() + 2);
}

TEST_F(reference_vector_test, insert_unique_clone) {
    pmr::monotonic_buffer_resource resource;
    object_creator custom { &resource };

    reference_vector<int> v { 1, 2, 3 };

    auto x = custom.create_unique<int>(-1);
    auto*p = x.get();
    auto iter = v.insert(v.begin() + 2, std::move(x));

    ASSERT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], -1);
    EXPECT_EQ(v[3], 3);

    EXPECT_NE(&v[2], p);

    EXPECT_EQ(iter, v.begin() + 2);
}

TEST_F(reference_vector_test, insert_initializer_list) {
    reference_vector<int> v { 1, 2, 3 };
    auto iter = v.insert(v.begin() + 2, { -1, -2 });

    ASSERT_EQ(v.size(), 5);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], -1);
    EXPECT_EQ(v[3], -2);
    EXPECT_EQ(v[4], 3);

    EXPECT_EQ(iter, v.begin() + 2);
}

TEST_F(reference_vector_test, insert_iter) {
    reference_vector<int> v { 1, 2, 3 };
    std::initializer_list<int> il { -1, -2 };
    auto iter = v.insert(v.begin() + 2, il.begin(), il.end());

    ASSERT_EQ(v.size(), 5);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], -1);
    EXPECT_EQ(v[3], -2);
    EXPECT_EQ(v[4], 3);

    EXPECT_EQ(iter, v.begin() + 2);
}

TEST_F(reference_vector_test, push_back) {
    reference_vector<int> v { 1, 2 };
    int const i = 3;
    v.push_back(i);

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST_F(reference_vector_test, push_back_rvalue) {
    reference_vector<std::vector<int>> v;
    v.emplace_back(std::initializer_list<int> { 1 });
    v.emplace_back(std::initializer_list<int> { 2, 3 });

    std::vector<int> e { -1 };
    v.push_back(std::move(e));

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], (std::vector { 1 }));
    EXPECT_EQ(v[1], (std::vector { 2, 3 }));
    EXPECT_EQ(v[2], (std::vector { -1 }));

    EXPECT_TRUE(e.empty());
}

TEST_F(reference_vector_test, push_back_unique) {
    reference_vector<int> v { 1, 2 };

    auto x = v.get_object_creator().create_unique<int>(-1);
    auto*p = x.get();
    v.push_back(std::move(x));

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], -1);

    EXPECT_EQ(&v[2], p);
}

TEST_F(reference_vector_test, push_back_unique_clone) {
    pmr::monotonic_buffer_resource resource;
    object_creator custom { &resource };

    reference_vector<int> v { 1, 2 };

    auto x = custom.create_unique<int>(-1);
    auto*p = x.get();
    v.push_back(std::move(x));

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], -1);

    EXPECT_NE(&v[2], p);
}

TEST_F(reference_vector_test, emplace) {
    reference_vector<Base> v {
        Sub { "A" },
        Sub { "B" },
    };
    auto iter = v.emplace<Sub>(v.begin() + 1, "X");

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0].get(), "A");
    EXPECT_EQ(v[1].get(), "X");
    EXPECT_EQ(v[2].get(), "B");

    EXPECT_EQ(&*iter, &v[1]);
}

TEST_F(reference_vector_test, emplace_back) {
    reference_vector<Base> v {
            Sub { "A" },
            Sub { "B" },
    };
    auto&& e = v.emplace_back<Sub>("X");

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0].get(), "A");
    EXPECT_EQ(v[1].get(), "B");
    EXPECT_EQ(v[2].get(), "X");

    EXPECT_EQ(&e, &v[2]);
}

TEST_F(reference_vector_test, put) {
    reference_vector v { 1, 2, 3 };
    int const i = -1;
    auto&& e = v.put(v.begin() + 1, i);

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], -1);
    EXPECT_EQ(v[2], 3);

    EXPECT_EQ(&e, &v[1]);
}

TEST_F(reference_vector_test, put_back_rvalue) {
    reference_vector<std::vector<int>> v;
    v.emplace_back(std::initializer_list<int> { 1 });
    v.emplace_back(std::initializer_list<int> { 2, 3 });
    v.emplace_back(std::initializer_list<int> { 4, 5, 6 });

    std::vector<int> e { -1 };
    v.put(v.begin() + 1, std::move(e));

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], (std::vector { 1 }));
    EXPECT_EQ(v[1], (std::vector { -1 }));
    EXPECT_EQ(v[2], (std::vector { 4, 5, 6 }));

    EXPECT_TRUE(e.empty());
}

TEST_F(reference_vector_test, put_unique) {
    reference_vector v { 1, 2, 3 };

    auto x = v.get_object_creator().create_unique<int>(-1);
    auto*p = x.get();
    auto&& e = v.put(v.begin() + 1, std::move(x));

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], -1);
    EXPECT_EQ(v[2], 3);

    EXPECT_EQ(&e, &v[1]);
    EXPECT_EQ(&e, p);
}

TEST_F(reference_vector_test, put_unique_clone) {
    pmr::monotonic_buffer_resource resource;
    object_creator custom { &resource };

    reference_vector v { 1, 2, 3 };

    auto x = custom.create_unique<int>(-1);
    auto*p = x.get();
    auto&& e = v.put(v.begin() + 1, std::move(x));

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], -1);
    EXPECT_EQ(v[2], 3);

    EXPECT_EQ(&e, &v[1]);
    EXPECT_NE(&e, p);
}

TEST_F(reference_vector_test, replace) {
    reference_vector<Base> v {
            Sub { "A" },
            Sub { "B" },
            Sub { "C" },
    };
    auto&& e = v.replace<Sub>(v.begin() + 1, "X");

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0].get(), "A");
    EXPECT_EQ(v[1].get(), "X");
    EXPECT_EQ(v[2].get(), "C");

    EXPECT_EQ(&e, &v[1]);
}

TEST_F(reference_vector_test, release) {
    reference_vector<Base> v {
            Sub { "A" },
            Sub { "B" },
            Sub { "C" },
    };
    auto [e, iter] = v.release(v.begin() + 1);

    ASSERT_EQ(v.size(), 2);
    EXPECT_EQ(v[0].get(), "A");
    EXPECT_EQ(v[1].get(), "C");

    EXPECT_EQ(e->get(), "B");
    EXPECT_EQ(&*iter, &v[1]);
}

TEST_F(reference_vector_test, release_range) {
    reference_vector<Base> v {
            Sub { "A" },
            Sub { "B" },
            Sub { "C" },
            Sub { "D" },
            Sub { "E" },
    };
    auto [e, iter] = v.release(v.begin() + 1, v.begin() + 4);

    ASSERT_EQ(v.size(), 2);
    EXPECT_EQ(v[0].get(), "A");
    EXPECT_EQ(v[1].get(), "E");

    ASSERT_EQ(e.size(), 3);
    EXPECT_EQ(e[0].get(), "B");
    EXPECT_EQ(e[1].get(), "C");
    EXPECT_EQ(e[2].get(), "D");

    EXPECT_EQ(&*iter, &v[1]);
}

TEST_F(reference_vector_test, release_back) {
    reference_vector<Base> v {
            Sub { "A" },
            Sub { "B" },
            Sub { "C" },
    };
    auto e = v.release_back();

    ASSERT_EQ(v.size(), 2);
    EXPECT_EQ(v[0].get(), "A");
    EXPECT_EQ(v[1].get(), "B");

    EXPECT_EQ(e->get(), "C");
}

TEST_F(reference_vector_test, ownership) {
    reference_vector<Base> v {
            Sub { "A" },
            Sub { "B" },
            Sub { "C" },
    };
    auto o = v.ownership(v.begin() + 1);

    EXPECT_EQ(o->get(), "B");

    o = v.get_object_creator().create_unique<Sub>("X");
    EXPECT_EQ(v[1].get(), "X");
}

TEST_F(reference_vector_test, begin_end) {
    reference_vector v { 1, 2, 3 };
    auto iter = v.begin();

    ASSERT_NE(iter, v.end());
    EXPECT_EQ(*iter, 1);
    ++iter;

    ASSERT_NE(iter, v.end());
    EXPECT_EQ(*iter, 2);
    ++iter;

    ASSERT_NE(iter, v.end());
    EXPECT_EQ(*iter, 3);
    ++iter;

    ASSERT_EQ(iter, v.end());
}

TEST_F(reference_vector_test, begin_end_const) {
    reference_vector const v { 1, 2, 3 };
    auto iter = v.begin();

    ASSERT_NE(iter, v.end());
    EXPECT_EQ(*iter, 1);
    ++iter;

    ASSERT_NE(iter, v.end());
    EXPECT_EQ(*iter, 2);
    ++iter;

    ASSERT_NE(iter, v.end());
    EXPECT_EQ(*iter, 3);
    ++iter;

    ASSERT_EQ(iter, v.end());
}

TEST_F(reference_vector_test, cbegin_cend) {
    reference_vector v { 1, 2, 3 };
    auto iter = v.cbegin();

    ASSERT_NE(iter, v.cend());
    EXPECT_EQ(*iter, 1);
    ++iter;

    ASSERT_NE(iter, v.cend());
    EXPECT_EQ(*iter, 2);
    ++iter;

    ASSERT_NE(iter, v.cend());
    EXPECT_EQ(*iter, 3);
    ++iter;

    ASSERT_EQ(iter, v.cend());
}

TEST_F(reference_vector_test, rbegin_rend) {
    reference_vector v { 1, 2, 3 };
    auto iter = v.rbegin();

    ASSERT_NE(iter, v.rend());
    EXPECT_EQ(*iter, 3);
    ++iter;

    ASSERT_NE(iter, v.rend());
    EXPECT_EQ(*iter, 2);
    ++iter;

    ASSERT_NE(iter, v.rend());
    EXPECT_EQ(*iter, 1);
    ++iter;

    ASSERT_EQ(iter, v.rend());
}

TEST_F(reference_vector_test, rbegin_rend_const) {
    reference_vector const v { 1, 2, 3 };
    auto iter = v.rbegin();

    ASSERT_NE(iter, v.rend());
    EXPECT_EQ(*iter, 3);
    ++iter;

    ASSERT_NE(iter, v.rend());
    EXPECT_EQ(*iter, 2);
    ++iter;

    ASSERT_NE(iter, v.rend());
    EXPECT_EQ(*iter, 1);
    ++iter;

    ASSERT_EQ(iter, v.rend());
}

TEST_F(reference_vector_test, crbegin_crend) {
    reference_vector v { 1, 2, 3 };
    auto iter = v.crbegin();

    ASSERT_NE(iter, v.crend());
    EXPECT_EQ(*iter, 3);
    ++iter;

    ASSERT_NE(iter, v.crend());
    EXPECT_EQ(*iter, 2);
    ++iter;

    ASSERT_NE(iter, v.crend());
    EXPECT_EQ(*iter, 1);
    ++iter;

    ASSERT_EQ(iter, v.crend());
}

TEST_F(reference_vector_test, swap) {
    reference_vector a { 1 };
    reference_vector b { 2, 3 };

    a.swap(b);

    ASSERT_EQ(a.size(), 2);
    EXPECT_EQ(a[0], 2);
    EXPECT_EQ(a[1], 3);

    ASSERT_EQ(b.size(), 1);
    EXPECT_EQ(b[0], 1);
}

TEST_F(reference_vector_test, unsafe_assign) {
    object_creator oc;
    reference_vector<int> v;
    v.unsafe_assign({
            oc.create_object<int>(1),
            oc.create_object<int>(2),
            oc.create_object<int>(3),
    });

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST_F(reference_vector_test, unsafe_assign_ref) {
    object_creator oc;
    reference_vector<Base> v;
    v.unsafe_assign({
            oc.create_object<Sub>("A"),
            oc.create_object<Sub>("B"),
            oc.create_object<Sub>("C"),
    });

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0].get(), "A");
    EXPECT_EQ(v[1].get(), "B");
    EXPECT_EQ(v[2].get(), "C");
}

TEST_F(reference_vector_test, unsafe_assign_iter) {
    object_creator oc;
    std::initializer_list<int*> l {
            oc.create_object<int>(1),
            oc.create_object<int>(2),
            oc.create_object<int>(3),
    };
    reference_vector<int> v;
    v.unsafe_assign(l.begin(), l.end());

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST_F(reference_vector_test, unsafe_assign_iter_ref) {
    object_creator oc;
    std::initializer_list<Base*> l {
            oc.create_object<Sub>("A"),
            oc.create_object<Sub>("B"),
            oc.create_object<Sub>("C"),
    };
    reference_vector<Base> v;
    v.unsafe_assign(l.begin(), l.end());

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0].get(), "A");
    EXPECT_EQ(v[1].get(), "B");
    EXPECT_EQ(v[2].get(), "C");
}

TEST_F(reference_vector_test, unsafe_insert) {
    object_creator oc;
    reference_vector<int> v { 1, 2, 3 };
    auto iter = v.unsafe_insert(v.begin() + 2, oc.create_object<int>(-1));

    ASSERT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], -1);
    EXPECT_EQ(v[3], 3);

    EXPECT_EQ(&*iter, &v[2]);
}

TEST_F(reference_vector_test, unsafe_insert_list) {
    object_creator oc;
    reference_vector<int> v { 1, 2 };
    auto iter = v.unsafe_insert(v.begin() + 1, {
            oc.create_object<int>(-1),
            oc.create_object<int>(-2),
    });

    ASSERT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], -1);
    EXPECT_EQ(v[2], -2);
    EXPECT_EQ(v[3], 2);

    EXPECT_EQ(&*iter, &v[1]);
}

TEST_F(reference_vector_test, unsafe_insert_iter) {
    object_creator oc;
    reference_vector<int> v { 1, 2 };
    std::initializer_list<int*> il {
            oc.create_object<int>(-1),
            oc.create_object<int>(-2),
    };
    auto iter = v.unsafe_insert(v.begin() + 1, il.begin(), il.end());

    ASSERT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], -1);
    EXPECT_EQ(v[2], -2);
    EXPECT_EQ(v[3], 2);

    EXPECT_EQ(&*iter, &v[1]);
}

TEST_F(reference_vector_test, unsafe_push_back) {
    object_creator oc;
    reference_vector<int> v { 1, 2, 3 };
    auto&& e = v.unsafe_push_back(oc.create_object<int>(-1));

    ASSERT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], -1);

    EXPECT_EQ(&e, &v[3]);
}

TEST_F(reference_vector_test, unsafe_put) {
    object_creator oc;
    reference_vector<int> v { 1, 2, 3 };
    auto&& e = v.unsafe_put(v.begin() + 1, oc.create_object<int>(-1));

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], -1);
    EXPECT_EQ(v[2], 3);

    EXPECT_EQ(&e, &v[1]);
}

TEST_F(reference_vector_test, compare) {
    EXPECT_EQ(make_vector({}, 10), make_vector({}, 10));
    EXPECT_LE(make_vector({}, 10), make_vector({}, 10));
    EXPECT_GE(make_vector({}, 10), make_vector({}, 10));

    EXPECT_NE(make_vector({}, 10, 20), make_vector({}, 20, 10));

    EXPECT_NE(make_vector({}, 10), make_vector({}, 20));
    EXPECT_LT(make_vector({}, 10), make_vector({}, 20));
    EXPECT_LE(make_vector({}, 10), make_vector({}, 20));

    EXPECT_NE(make_vector({}, 10), make_vector({}, 10, 20));
    EXPECT_LT(make_vector({}, 10), make_vector({}, 10, 20));
    EXPECT_LE(make_vector({}, 10), make_vector({}, 10, 20));

    EXPECT_NE(make_vector({}, 20), make_vector({}, 10));
    EXPECT_GT(make_vector({}, 20), make_vector({}, 10));
    EXPECT_GE(make_vector({}, 20), make_vector({}, 10));

    EXPECT_NE(make_vector({}, 10, 0), make_vector({}, 10));
    EXPECT_GT(make_vector({}, 10, 0), make_vector({}, 10));
    EXPECT_GE(make_vector({}, 10, 0), make_vector({}, 10));
}

} // namespace takatori::util
