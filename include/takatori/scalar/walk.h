#pragma once

#include <functional>
#include <type_traits>

#include "dispatch.h"

#include <takatori/util/post_visit.h>

namespace takatori::scalar {

namespace impl {

/**
 * @private
 * @brief walks over takatori::scalar models.
 */
class walker_adapter {
public:
    template<class Callback, class... Args>
    void operator()(immediate& expr, Callback&& callback, Args&&... args) {
        do_nullary(std::forward<Callback>(callback), expr, std::forward<Args>(args)...);
    }

    template<class Callback, class... Args>
    void operator()(immediate const& expr, Callback&& callback, Args&&... args) {
        do_nullary(std::forward<Callback>(callback), expr, std::forward<Args>(args)...);
    }

    template<class Callback, class... Args>
    void operator()(variable_reference& expr, Callback&& callback, Args&&... args) {
        do_nullary(std::forward<Callback>(callback), expr, std::forward<Args>(args)...);
    }

    template<class Callback, class... Args>
    void operator()(variable_reference const& expr, Callback&& callback, Args&&... args) {
        do_nullary(std::forward<Callback>(callback), expr, std::forward<Args>(args)...);
    }

    template<class Callback, class... Args>
    void operator()(unary& expr, Callback&& callback, Args&&... args) {
        do_unary(std::forward<Callback>(callback), expr, std::forward<Args>(args)...);
    }

    template<class Callback, class... Args>
    void operator()(unary const& expr, Callback&& callback, Args&&... args) {
        do_unary(std::forward<Callback>(callback), expr, std::forward<Args>(args)...);
    }

    template<class Callback, class... Args>
    void operator()(cast& expr, Callback&& callback, Args&&... args) {
        do_unary(std::forward<Callback>(callback), expr, std::forward<Args>(args)...);
    }

    template<class Callback, class... Args>
    void operator()(cast const& expr, Callback&& callback, Args&&... args) {
        do_unary(std::forward<Callback>(callback), expr, std::forward<Args>(args)...);
    }

    template<class Callback, class... Args>
    void operator()(binary& expr, Callback&& callback, Args&&... args) {
        do_binary(std::forward<Callback>(callback), expr, std::forward<Args>(args)...);
    }

    template<class Callback, class... Args>
    void operator()(binary const& expr, Callback&& callback, Args&&... args) {
        do_binary(std::forward<Callback>(callback), expr, std::forward<Args>(args)...);
    }

    template<class Callback, class... Args>
    void operator()(compare& expr, Callback&& callback, Args&&... args) {
        do_binary(std::forward<Callback>(callback), expr, std::forward<Args>(args)...);
    }

    template<class Callback, class... Args>
    void operator()(compare const& expr, Callback&& callback, Args&&... args) {
        do_binary(std::forward<Callback>(callback), expr, std::forward<Args>(args)...);
    }

    template<class Callback, class... Args>
    void operator()(match& expr, Callback&& callback, Args&&... args) {
        do_match(std::forward<Callback>(callback), expr, std::forward<Args>(args)...);
    }

    template<class Callback, class... Args>
    void operator()(match const& expr, Callback&& callback, Args&&... args) {
        do_match(std::forward<Callback>(callback), expr, std::forward<Args>(args)...);
    }

    template<class Callback, class... Args>
    void operator()(conditional& expr, Callback&& callback, Args&&... args) {
        do_conditional(std::forward<Callback>(callback), expr, std::forward<Args>(args)...);
    }

    template<class Callback, class... Args>
    void operator()(conditional const& expr, Callback&& callback, Args&&... args) {
        do_conditional(std::forward<Callback>(callback), expr, std::forward<Args>(args)...);
    }

    template<class Callback, class... Args>
    void operator()(coalesce& expr, Callback&& callback, Args&&... args) {
        do_coalesce(std::forward<Callback>(callback), expr, std::forward<Args>(args)...);
    }

    template<class Callback, class... Args>
    void operator()(coalesce const& expr, Callback&& callback, Args&&... args) {
        do_coalesce(std::forward<Callback>(callback), expr, std::forward<Args>(args)...);
    }

    template<class Callback, class... Args>
    void operator()(let& expr, Callback&& callback, Args&&... args) {
        do_let(std::forward<Callback>(callback), expr, std::forward<Args>(args)...);
    }

    template<class Callback, class... Args>
    void operator()(let const& expr, Callback&& callback, Args&&... args) {
        do_let(std::forward<Callback>(callback), expr, std::forward<Args>(args)...);
    }

    template<class Callback, class... Args>
    void operator()(function_call& expr, Callback&& callback, Args&&... args) {
        do_function_call(std::forward<Callback>(callback), expr, std::forward<Args>(args)...);
    }

    template<class Callback, class... Args>
    void operator()(function_call const& expr, Callback&& callback, Args&&... args) {
        do_function_call(std::forward<Callback>(callback), expr, std::forward<Args>(args)...);
    }

    template<class Callback, class... Args>
    void operator()(extension& expr, Callback&& callback, Args&&... args) {
        do_nullary(std::forward<Callback>(callback), expr, std::forward<Args>(args)...);
    }

    template<class Callback, class... Args>
    void operator()(extension const& expr, Callback&& callback, Args&&... args) {
        do_nullary(std::forward<Callback>(callback), expr, std::forward<Args>(args)...);
    }

    template<class Callback, class Expr, class... Args>
    void operator()(Expr& expr, Callback&& callback, Args&&... args) {
        do_nullary(std::forward<Callback>(callback), expr, std::forward<Args>(args)...);
    }

    template<class Callback, class Expr, class... Args>
    void operator()(Expr const& expr, Callback&& callback, Args&&... args) {
        do_nullary(std::forward<Callback>(callback), expr, std::forward<Args>(args)...);
    }

private:
    template<class Callback, class Expr, class... Args>
    bool do_pre(Callback&& callback, Expr&& expr, Args&&... args) {
        if constexpr (std::is_same_v<std::invoke_result_t<Callback, Expr, Args...>, void>) { // NOLINT
            std::invoke(std::forward<Callback>(callback), std::forward<Expr>(expr), std::forward<Args>(args)...);
            return true;
        } else { // NOLINT
            return std::invoke(std::forward<Callback>(callback), std::forward<Expr>(expr), std::forward<Args>(args)...);
        }
    }

    template<class Callback, class Expr, class... Args>
    void do_post(Callback&& callback, Expr&& expr, Args&&... args) {
        if constexpr (std::is_invocable_v<Callback, util::post_visit, Expr, Args...>) {
            std::invoke(std::forward<Callback>(callback), util::post_visit {}, std::forward<Expr>(expr), std::forward<Args>(args)...);
        }
    }

    template<class Callback, class Expr, class... Args>
    void do_member(Callback&& callback, Expr&& expr, Args&&... args) {
        scalar::dispatch(*this, std::forward<Expr>(expr), std::forward<Callback>(callback), std::forward<Args>(args)...);
    }

    template<class Callback, class Expr, class... Args>
    void do_member(Callback&& callback, util::optional_ptr<Expr> expr, Args&&... args) {
        if (expr) {
            do_member(std::forward<Callback>(callback), *expr, std::forward<Args>(args)...);
        }
    }

    template<class Callback, class Expr, class... Args>
    void do_nullary(Callback&& callback, Expr&& expr, Args&&... args) {
        if (do_pre(std::forward<Callback>(callback), std::forward<Expr>(expr), std::forward<Args>(args)...)) {
            do_post(
                    std::forward<Callback>(callback),
                    std::forward<Expr>(expr),
                    std::forward<Args>(args)...);
        }
    }

    template<class Callback, class Expr, class... Args>
    void do_unary(Callback&& callback, Expr&& expr, Args&&... args) {
        if (do_pre(std::forward<Callback>(callback), std::forward<Expr>(expr), std::forward<Args>(args)...)) {
            do_member(std::forward<Callback>(callback), expr.operand(), std::forward<Args>(args)...);
            do_post(std::forward<Callback>(callback), std::forward<Expr>(expr), std::forward<Args>(args)...);
        }
    }

    template<class Callback, class Expr, class... Args>
    void do_binary(Callback&& callback, Expr&& expr, Args&&... args) {
        if (do_pre(std::forward<Callback>(callback), std::forward<Expr>(expr), std::forward<Args>(args)...)) {
            do_member(std::forward<Callback>(callback), expr.left(), std::forward<Args>(args)...);
            do_member(std::forward<Callback>(callback), expr.right(), std::forward<Args>(args)...);
            do_post(std::forward<Callback>(callback), std::forward<Expr>(expr), std::forward<Args>(args)...);
        }
    }

    template<class Callback, class Expr, class... Args>
    void do_match(Callback&& callback, Expr&& expr, Args&&... args) {
        if (do_pre(std::forward<Callback>(callback), std::forward<Expr>(expr), std::forward<Args>(args)...)) {
            do_member(std::forward<Callback>(callback), expr.input(), std::forward<Args>(args)...);
            do_member(std::forward<Callback>(callback), expr.pattern(), std::forward<Args>(args)...);
            do_member(std::forward<Callback>(callback), expr.escape(), std::forward<Args>(args)...);
            do_post(std::forward<Callback>(callback), std::forward<Expr>(expr), std::forward<Args>(args)...);
        }
    }

    template<class Callback, class Expr, class... Args>
    void do_conditional(Callback&& callback, Expr&& expr, Args&&... args) {
        if (do_pre(std::forward<Callback>(callback), std::forward<Expr>(expr), std::forward<Args>(args)...)) {
            for (auto&& alternative : expr.alternatives()) {
                do_member(std::forward<Callback>(callback), alternative.condition(), std::forward<Args>(args)...);
                do_member(std::forward<Callback>(callback), alternative.body(), std::forward<Args>(args)...);
            }
            do_member(std::forward<Callback>(callback), expr.default_expression(), std::forward<Args>(args)...);
            do_post(std::forward<Callback>(callback), std::forward<Expr>(expr), std::forward<Args>(args)...);
        }
    }

    template<class Callback, class Expr, class... Args>
    void do_coalesce(Callback&& callback, Expr&& expr, Args&&... args) {
        if (do_pre(std::forward<Callback>(callback), std::forward<Expr>(expr), std::forward<Args>(args)...)) {
            for (auto&& e : expr.alternatives()) {
                do_member(std::forward<Callback>(callback), e, std::forward<Args>(args)...);
            }
            do_post(std::forward<Callback>(callback), std::forward<Expr>(expr), std::forward<Args>(args)...);
        }
    }

    template<class Callback, class Expr, class... Args>
    void do_let(Callback&& callback, Expr&& expr, Args&&... args) {
        if (do_pre(std::forward<Callback>(callback), std::forward<Expr>(expr), std::forward<Args>(args)...)) {
            for (auto&& decl : expr.variables()) {
                do_member(std::forward<Callback>(callback), decl.value(), std::forward<Args>(args)...);
            }
            do_member(std::forward<Callback>(callback), expr.body(), std::forward<Args>(args)...);
            do_post(std::forward<Callback>(callback), std::forward<Expr>(expr), std::forward<Args>(args)...);
        }
    }

    template<class Callback, class Expr, class... Args>
    void do_function_call(Callback&& callback, Expr&& expr, Args&&... args) {
        if (do_pre(std::forward<Callback>(callback), std::forward<Expr>(expr), std::forward<Args>(args)...)) {
            for (auto&& arg : expr.arguments()) {
                do_member(std::forward<Callback>(callback), arg, std::forward<Args>(args)...);
            }
            do_post(std::forward<Callback>(callback), std::forward<Expr>(expr), std::forward<Args>(args)...);
        }
    }
};

} // namespace impl

/// @copydoc takatori::util::post_visit
using util::post_visit;

/**
 * @brief visits the given expression and its member expressions recursively.
 * @details First, this invokes `bool Callback::operator()(T&, Args...)`,
 *      where `T` is the concrete type of the expression.
 *      If the above function returns `true`, then this invokes `bool Callback::operator()(U&, Args...)`
 *      for each member expression of `T` recursively.
 *      After invoking callback functions for each `T`'s member, finally this invokes
 *      `X Callback::operator()(post_visit, T&, Args...)` only if it is declared.
 * @attention You must declare all callback functions for individual subclasses,
 *      or declare Callback::operator()(expression&, Args...) as "default" callback function.
 * @note The result type of each callback function can be `void` instead of `bool`.
 *      In that case, this considers as if it always returns `true`.
 * @tparam Callback the callback object type
 * @tparam Args the callback argument types
 * @param callback the callback object
 * @param object the target expression
 * @param args the callback arguments
 */
template<class Callback, class... Args>
inline void walk(Callback&& callback, expression& object, Args&&... args) {
    impl::walker_adapter adapter;
    dispatch(adapter, object, std::forward<Callback>(callback), std::forward<Args>(args)...);
}

/**
 * @brief visits the given expression and its member expressions recursively.
 * @details First, this invokes `bool Callback::operator()(T const&, Args...)`,
 *      where `T` is the concrete type of the expression.
 *      If the above function returns `true`, then this invokes `bool Callback::operator()(U const&, Args...)`
 *      for each member expression of `T` recursively.
 *      After invoking callback functions for each `T`'s member, finally this invokes
 *      `X Callback::operator()(post_visit, T const&, Args...)` only if it is declared.
 * @attention You must declare all callback functions for individual subclasses,
 *      or declare Callback::operator()(expression const&, Args...) as "default" callback function.
 * @note The result type of each callback function can be `void` instead of `bool`.
 *      In that case, this considers as if it always returns `true`.
 * @tparam Callback the callback object type
 * @tparam Args the callback argument types
 * @param callback the callback object
 * @param object the target expression
 * @param args the callback arguments
 */
template<class Callback, class... Args>
inline void walk(Callback&& callback, expression const& object, Args&&... args) {
    impl::walker_adapter adapter;
    dispatch(adapter, object, std::forward<Callback>(callback), std::forward<Args>(args)...);
}

} // namespace takatori::scalar
