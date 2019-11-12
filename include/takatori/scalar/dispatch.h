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

//#include "extension.h"

#include "takatori/util/callback.h"

namespace takatori::scalar {

/// @private
namespace impl {

/// @private
template<class Callback, class E, class... Args>
inline auto dispatch_expression(Callback&& callback, E&& expression, Args&&... args) {
    switch (expression.kind()) {
        case immediate::tag: return util::polymorphic_callback<immediate>(std::forward<Callback>(callback), std::forward<E>(expression), std::forward<Args>(args)...);
        case variable_reference::tag: return util::polymorphic_callback<variable_reference>(std::forward<Callback>(callback), std::forward<E>(expression), std::forward<Args>(args)...);
        case unary::tag: return util::polymorphic_callback<unary>(std::forward<Callback>(callback), std::forward<E>(expression), std::forward<Args>(args)...);
        case cast::tag: return util::polymorphic_callback<cast>(std::forward<Callback>(callback), std::forward<E>(expression), std::forward<Args>(args)...);
        case binary::tag: return util::polymorphic_callback<binary>(std::forward<Callback>(callback), std::forward<E>(expression), std::forward<Args>(args)...);
        case compare::tag: return util::polymorphic_callback<compare>(std::forward<Callback>(callback), std::forward<E>(expression), std::forward<Args>(args)...);
        case match::tag: return util::polymorphic_callback<match>(std::forward<Callback>(callback), std::forward<E>(expression), std::forward<Args>(args)...);
        case conditional::tag: return util::polymorphic_callback<conditional>(std::forward<Callback>(callback), std::forward<E>(expression), std::forward<Args>(args)...);
        case coalesce::tag: return util::polymorphic_callback<coalesce>(std::forward<Callback>(callback), std::forward<E>(expression), std::forward<Args>(args)...);
        case let::tag: return util::polymorphic_callback<let>(std::forward<Callback>(callback), std::forward<E>(expression), std::forward<Args>(args)...);
        case function_call::tag: return util::polymorphic_callback<function_call>(std::forward<Callback>(callback), std::forward<E>(expression), std::forward<Args>(args)...);

        // FIXME: other cases
        default: break;
    }
    std::abort();
}

} // namespace impl

/**
 * @brief invoke callback function for individual subclasses of expression.
 * If the input object is ancestor type of T, this may invoke Callback::operator()(T&, Args...).
 * You must declare all callback functions for individual subclasses,
 * or declare Callback::operator()(expression&, Args...) as "default" callback function.
 * Each return type of callback function must be identical.
 * @tparam Callback the callback object type
 * @tparam Args the callback argument types
 * @param callback the callback object
 * @param object the target expression
 * @param args the callback arguments
 * @return the callback result
 */
template<class Callback, class... Args>
inline auto dispatch(Callback&& callback, expression& object, Args&&... args) {
    return impl::dispatch_expression(std::forward<Callback>(callback), object, std::forward<Args>(args)...);
}

/**
 * @brief invoke callback function for individual subclasses of expression.
 * If the input object is ancestor type of T, this may invoke Callback::operator()(T const&, Args...).
 * You must declare all callback functions for individual subclasses,
 * or declare Callback::operator()(expression const&, Args...) as "default" callback function.
 * Each return type of callback function must be identical.
 * @tparam Callback the callback object type
 * @tparam Args the callback argument types
 * @param callback the callback object
 * @param object the target expression
 * @param args the callback arguments
 * @return the callback result
 */
template<class Callback, class... Args>
inline auto dispatch(Callback&& callback, expression const& object, Args&&... args) {
    return impl::dispatch_expression(std::forward<Callback>(callback), object, std::forward<Args>(args)...);
}

/**
 * @brief invoke callback function for individual subclasses of expression.
 * If the input object is ancestor type of T, this may invoke Callback::operator()(T&&, Args...).
 * You must declare all callback functions for individual subclasses,
 * or declare Callback::operator()(expression&&, Args...) as "default" callback function.
 * Each return type of callback function must be identical.
 * @tparam Callback the callback object type
 * @tparam Args the callback argument types
 * @param callback the callback object
 * @param object the target expression
 * @param args the callback arguments
 * @return the callback result
 */
template<class Callback, class... Args>
inline auto dispatch(Callback&& callback, expression&& object, Args&&... args) {
    return impl::dispatch_expression(std::forward<Callback>(callback), std::move(object), std::forward<Args>(args)...);
}

} // namespace takatori::scalar
