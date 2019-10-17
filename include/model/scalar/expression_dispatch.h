#pragma once

#include <type_traits>

#include <cstdlib>

#include "expression.h"
#include "expression_kind.h"

#include "immediate.h"
#include "variable_reference.h"

#include "unary.h"
#include "cast.h"

#include "binary.h"
#include "compare.h"

#include "match.h"
#include "conditional.h"
#include "coalesce.h"
#include "let.h"
#include "function_call.h"

//#include "array_construct.h"
//#include "array_length.h"
//#include "array_get.h"
//#include "array_compare_quantification.h"

//#include "record_construct.h"
//#include "record_get.h"

//#include "subquery_scalar.h"
//#include "subquery_exists.h"
//#include "subquery_compare_quantification.h"

//#include "extension.h"

#include "util/downcast.h"

namespace model::scalar {

/// @private
namespace impl {

/// @private
template<class T, class Callback, class E, class... Args>
inline std::enable_if_t<
        std::is_invocable_v<Callback, util::infect_qualifier_t<E, T>, Args...>,
        std::invoke_result_t<Callback, util::infect_qualifier_t<E, T>, Args...>>
callback_expression(Callback&& callback, E&& expression, Args&&... args) {
    return std::forward<Callback>(callback)(util::unsafe_downcast<T>(std::forward<E>(expression)), std::forward<Args>(args)...);
}

/// @private
template<class Callback, class E, class... Args>
inline auto dispatch_expression(Callback&& callback, E&& expression, Args&&... args) {
    switch (expression.kind()) {
        case immediate::tag: return callback_expression<immediate>(std::forward<Callback>(callback), std::forward<E>(expression), std::forward<Args>(args)...);
        case variable_reference::tag: return callback_expression<variable_reference>(std::forward<Callback>(callback), std::forward<E>(expression), std::forward<Args>(args)...);
        case unary::tag: return callback_expression<unary>(std::forward<Callback>(callback), std::forward<E>(expression), std::forward<Args>(args)...);
        case cast::tag: return callback_expression<cast>(std::forward<Callback>(callback), std::forward<E>(expression), std::forward<Args>(args)...);
        case binary::tag: return callback_expression<binary>(std::forward<Callback>(callback), std::forward<E>(expression), std::forward<Args>(args)...);
        case compare::tag: return callback_expression<compare>(std::forward<Callback>(callback), std::forward<E>(expression), std::forward<Args>(args)...);
        case match::tag: return callback_expression<match>(std::forward<Callback>(callback), std::forward<E>(expression), std::forward<Args>(args)...);
        case conditional::tag: return callback_expression<conditional>(std::forward<Callback>(callback), std::forward<E>(expression), std::forward<Args>(args)...);
        case coalesce::tag: return callback_expression<coalesce>(std::forward<Callback>(callback), std::forward<E>(expression), std::forward<Args>(args)...);
        case let::tag: return callback_expression<let>(std::forward<Callback>(callback), std::forward<E>(expression), std::forward<Args>(args)...);
        case function_call::tag: return callback_expression<function_call>(std::forward<Callback>(callback), std::forward<E>(expression), std::forward<Args>(args)...);

        // FIXME: other cases
        default: break;
    }
    std::abort();
}

} // namespace impl

/**
 * @brief invoke callback function for individual expression's subclasses.
 * If the actual type of expr is T, this may invoke Callback::operator()(T, Args...).
 * You must declare all callback functions for individual subclasses,
 * or declare Callback::operator()(expression&, Args...) as "default" callback function.
 * Each return type of callback function must be identical.
 * @tparam Callback the callback object type
 * @tparam Args the callback argument types
 * @param callback the callback object
 * @param expression the target expression
 * @param args the callback arguments
 * @return the callback result
 */
template<class Callback, class... Args>
inline auto dispatch(Callback&& callback, expression& expr, Args&&... args) {
    return impl::dispatch_expression(std::forward<Callback>(callback), expr, std::forward<Args>(args)...);
}

/// @copydoc dispatch()
template<class Callback, class... Args>
inline auto dispatch(Callback&& callback, expression const& expr, Args&&... args) {
    return impl::dispatch_expression(std::forward<Callback>(callback), expr, std::forward<Args>(args)...);
}

/// @copydoc dispatch()
template<class Callback, class... Args>
inline auto dispatch(Callback&& callback, expression&& expr, Args&&... args) {
    return impl::dispatch_expression(std::forward<Callback>(callback), expr, std::forward<Args>(args)...);
}

} // namespace model::scalar
