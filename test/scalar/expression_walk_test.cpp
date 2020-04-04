#include <takatori/scalar/walk.h>

#include <gtest/gtest.h>

#include "test_utils.h"

namespace takatori::scalar {

class expression_walk_test : public ::testing::Test {};

template<std::size_t N>
inline void set(std::array<bool, N>& a, std::size_t i) {
    if (i >= N) {
        std::ostringstream s;
        s << "out of bound: " << i << " >= " << N;
        throw std::domain_error(s.str());
    }
    if (a[i]) {
        std::ostringstream s;
        s << "duplicate at " << i;
        throw std::domain_error(s.str());
    }
    a[i] = true;
}

template<std::size_t N>
inline void set(std::array<bool, N>& a, immediate const& expr) {
    auto i = value_of<value::int4>(expr);
    set(a, static_cast<std::size_t>(i));
}

template<std::size_t N>
inline void check(std::array<bool, N> const& a) {
    for (std::size_t i = 0; i < N; ++i) {
        if (!a[i]) {
            std::ostringstream s;
            s << "not set at " << i;
            throw std::domain_error(s.str());
        }
    }
}

template<std::size_t N>
inline void check_only_first(std::array<bool, N> const& a) {
    if (!a[0]) {
        throw std::domain_error("not set at 0");
    }
    for (std::size_t i = 1; i < N; ++i) {
        if (a[i]) {
            std::ostringstream s;
            s << "set at " << i;
            throw std::domain_error(s.str());
        }
    }
}

TEST_F(expression_walk_test, immediate) {
    struct cb {
        std::array<bool, 2> a {};
        bool recursive { true };
        int operator()(expression const&) { std::abort(); }
        bool operator()(immediate const&) {
            set(a, 0);
            return recursive;
        }
        void operator()(post_visit, immediate const&) {
            set(a, 1);
        }
    };
    cb c;
    immediate expr {
            value::int4(1),
            type::int4(),
    };
    walk(c, expr);
    check(c.a);

    cb d;
    d.recursive = false;
    walk(d, std::as_const(expr));
    check_only_first(d.a);
}

TEST_F(expression_walk_test, variable_reference) {
    struct cb {
        std::array<bool, 2> a {};
        bool recursive { true };
        int operator()(expression const&) { std::abort(); }
        bool operator()(variable_reference const&) {
            set(a, 0);
            return recursive;
        }
        void operator()(post_visit, variable_reference const&) {
            set(a, 1);
        }
    };
    cb c;
    variable_reference expr {
            vardesc(1),
    };
    walk(c, expr);
    check(c.a);

    cb d;
    d.recursive = false;
    walk(d, std::as_const(expr));
    check_only_first(d.a);
}

TEST_F(expression_walk_test, unary) {
    struct cb {
        std::array<bool, 3> a {};
        bool recursive { true };
        int operator()(expression const&) { std::abort(); }
        bool operator()(unary const&) {
            set(a, 0);
            return recursive;
        }
        void operator()(post_visit, unary const&) {
            set(a, 1);
        }
        bool operator()(immediate const& expr) {
            set(a, expr);
            return false;
        }
    };
    cb c;
    unary expr {
            unary_operator::sign_inversion,
            constant(2),
    };
    walk(c, expr);
    check(c.a);

    cb d;
    d.recursive = false;
    walk(d, std::as_const(expr));
    check_only_first(d.a);
}

TEST_F(expression_walk_test, cast) {
    struct cb {
        std::array<bool, 3> a {};
        bool recursive { true };
        int operator()(expression const&) { std::abort(); }
        bool operator()(cast const&) {
            set(a, 0);
            return recursive;
        }
        void operator()(post_visit, cast const&) {
            set(a, 1);
        }
        bool operator()(immediate const& expr) {
            set(a, expr);
            return false;
        }
    };
    cb c;
    cast expr {
        type::int8(),
        cast::loss_policy_type::ignore,
        constant(2),
    };
    walk(c, expr);
    check(c.a);

    cb d;
    d.recursive = false;
    walk(d, std::as_const(expr));
    check_only_first(d.a);
}

TEST_F(expression_walk_test, binary) {
    struct cb {
        std::array<bool, 4> a {};
        bool recursive { true };
        int operator()(expression const&) { std::abort(); }
        bool operator()(binary const&) {
            set(a, 0);
            return recursive;
        }
        void operator()(post_visit, binary const&) {
            set(a, 1);
        }
        bool operator()(immediate const& expr) {
            set(a, expr);
            return false;
        }
    };
    cb c;
    binary expr {
            binary_operator::add,
            constant(2),
            constant(3),
    };
    walk(c, expr);
    check(c.a);

    cb d;
    d.recursive = false;
    walk(d, std::as_const(expr));
    check_only_first(d.a);
}

TEST_F(expression_walk_test, compare) {
    struct cb {
        std::array<bool, 4> a {};
        bool recursive { true };
        int operator()(expression const&) { std::abort(); }
        bool operator()(compare const&) {
            set(a, 0);
            return recursive;
        }
        void operator()(post_visit, compare const&) {
            set(a, 1);
        }
        bool operator()(immediate const& expr) {
            set(a, expr);
            return false;
        }
    };
    cb c;
    compare expr {
            comparison_operator::equal,
            constant(2),
            constant(3),
    };
    walk(c, expr);
    check(c.a);

    cb d;
    d.recursive = false;
    walk(d, std::as_const(expr));
    check_only_first(d.a);
}

TEST_F(expression_walk_test, match) {
    struct cb {
        std::array<bool, 5> a {};
        bool recursive { true };
        int operator()(expression const&) { std::abort(); }
        bool operator()(match const&) {
            set(a, 0);
            return recursive;
        }
        void operator()(post_visit, match const&) {
            set(a, 1);
        }
        bool operator()(immediate const& expr) {
            set(a, expr);
            return false;
        }
    };
    cb c;
    match expr {
            match_operator::like,
            constant(2),
            constant(3),
            constant(4),
    };
    walk(c, expr);
    check(c.a);

    cb d;
    d.recursive = false;
    walk(d, std::as_const(expr));
    check_only_first(d.a);
}

TEST_F(expression_walk_test, conditional) {
    struct cb {
        std::array<bool, 9> a {};
        bool recursive { true };
        int operator()(expression const&) { std::abort(); }
        bool operator()(conditional const&) {
            set(a, 0);
            return recursive;
        }
        void operator()(post_visit, conditional const&) {
            set(a, 1);
        }
        bool operator()(immediate const& expr) {
            set(a, expr);
            return false;
        }
    };
    cb c;
    conditional expr {
            {
                    conditional::alternative { constant(2), constant(3) },
                    conditional::alternative { constant(4), constant(5) },
                    conditional::alternative { constant(6), constant(7) },
            },
            constant(8),
    };
    walk(c, expr);
    check(c.a);

    cb d;
    d.recursive = false;
    walk(d, std::as_const(expr));
    check_only_first(d.a);
}

TEST_F(expression_walk_test, coalesce) {
    struct cb {
        std::array<bool, 5> a {};
        bool recursive { true };
        int operator()(expression const&) { std::abort(); }
        bool operator()(coalesce const&) {
            set(a, 0);
            return recursive;
        }
        void operator()(post_visit, coalesce const&) {
            set(a, 1);
        }
        bool operator()(immediate const& expr) {
            set(a, expr);
            return false;
        }
    };
    cb c;
    coalesce expr {
            {
                    constant(2),
                    constant(3),
                    constant(4),
            }
    };
    walk(c, expr);
    check(c.a);

    cb d;
    d.recursive = false;
    walk(d, std::as_const(expr));
    check_only_first(d.a);
}

TEST_F(expression_walk_test, let) {
    struct cb {
        std::array<bool, 6> a {};
        bool recursive { true };
        int operator()(expression const&) { std::abort(); }
        bool operator()(let const&) {
            set(a, 0);
            return recursive;
        }
        void operator()(post_visit, let const&) {
            set(a, 1);
        }
        bool operator()(immediate const& expr) {
            set(a, expr);
            return false;
        }
    };
    cb c;
    let expr {
            {
                    let::declarator { vardesc(1), constant(2) },
                    let::declarator { vardesc(2), constant(3) },
                    let::declarator { vardesc(3), constant(4) },
            },
            constant(5),
    };
    walk(c, expr);
    check(c.a);

    cb d;
    d.recursive = false;
    walk(d, std::as_const(expr));
    check_only_first(d.a);
}

TEST_F(expression_walk_test, function_call) {
    struct cb {
        std::array<bool, 5> a {};
        bool recursive { true };
        int operator()(expression const&) { std::abort(); }
        bool operator()(function_call const&) {
            set(a, 0);
            return recursive;
        }
        void operator()(post_visit, function_call const&) {
            set(a, 1);
        }
        bool operator()(immediate const& expr) {
            set(a, expr);
            return false;
        }
    };
    cb c;
    function_call expr {
            funcdesc(1),
            {
                    constant(2),
                    constant(3),
                    constant(4),
            },
    };
    walk(c, expr);
    check(c.a);

    cb d;
    d.recursive = false;
    walk(d, std::as_const(expr));
    check_only_first(d.a);
}

TEST_F(expression_walk_test, pre_void) {
    struct cb {
        std::array<bool, 2> a {};
        int operator()(expression const&) { std::abort(); }
        void operator()(immediate const&) {
            set(a, 0);
        }
        void operator()(post_visit, immediate const&) {
            set(a, 1);
        }
    };
    cb c;
    immediate expr {
            value::int4(1),
            type::int4(),
    };
    walk(c, expr);
    check(c.a);
}

} // namespace takatori::scalar
