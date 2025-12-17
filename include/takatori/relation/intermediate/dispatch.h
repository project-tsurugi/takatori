#pragma once

#include <string>
#include <type_traits>

#include "../expression.h"
#include "../expression_kind.h"

#include "../find.h"
#include "../scan.h"
#include "../join_find.h"
#include "../join_scan.h"
#include "../apply.h"
#include "../project.h"
#include "../filter.h"
#include "../buffer.h"
#include "../identify.h"
#include "../emit.h"
#include "../write.h"
#include "../values.h"

#include "join.h"
#include "aggregate.h"
#include "distinct.h"
#include "limit.h"
#include "union.h"
#include "intersection.h"
#include "difference.h"
#include "escape.h"

#include <takatori/util/callback.h>
#include <takatori/util/exception.h>
#include <takatori/util/string_builder.h>

namespace takatori::relation::intermediate {

/// @private
namespace impl {

[[noreturn]] inline void invalid_expression(expression const& object) {
    using ::takatori::util::string_builder;
    using ::takatori::util::throw_exception;
    throw_exception(std::invalid_argument(string_builder {}
            << "unsupported relational operator: "
            << object.kind()
            << string_builder::to_string));
}

/// @private
template<class Callback, class E, class... Args>
inline auto dispatch_expression(Callback&& callback, E&& object, Args&&... args) {
    switch (object.kind()) {
        case find::tag: return util::polymorphic_callback<find>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case scan::tag: return util::polymorphic_callback<scan>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case join_find::tag: return util::polymorphic_callback<join_find>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case join_scan::tag: return util::polymorphic_callback<join_scan>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case apply::tag: return util::polymorphic_callback<apply>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case project::tag: return util::polymorphic_callback<project>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case filter::tag: return util::polymorphic_callback<filter>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case buffer::tag: return util::polymorphic_callback<buffer>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case identify::tag: return util::polymorphic_callback<identify>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case emit::tag: return util::polymorphic_callback<emit>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case write::tag: return util::polymorphic_callback<write>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case values::tag: return util::polymorphic_callback<values>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);

        case join::tag: return util::polymorphic_callback<join>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case aggregate::tag: return util::polymorphic_callback<aggregate>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case distinct::tag: return util::polymorphic_callback<distinct>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case limit::tag: return util::polymorphic_callback<limit>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case union_::tag: return util::polymorphic_callback<union_>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case intersection::tag: return util::polymorphic_callback<intersection>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case difference::tag: return util::polymorphic_callback<difference>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case escape::tag: return util::polymorphic_callback<escape>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);

        // FIXME: extension
        default: break;
    }
    invalid_expression(object);
}

} // namespace impl

/**
 * @brief invoke callback function for individual subclasses of expression only for intermediate execution plan.
 * @details if the input object has type of T, this may invoke Callback::operator()(T&, Args...).
 * @attention You must declare all callback functions for individual subclasses,
 * or declare Callback::operator()(expression&, Args...) as "default" callback function.
 * Each return type of callback function must be identical.
 * @tparam Callback the callback object type
 * @tparam Args the callback argument types
 * @param callback the callback object
 * @param object the target expression
 * @param args the callback arguments
 * @return the callback result
 * @throws std::invalid_argument if the expression is not available in step execution plan
 */
template<class Callback, class... Args>
inline auto dispatch(Callback&& callback, expression& object, Args&&... args) {
    return impl::dispatch_expression(std::forward<Callback>(callback), object, std::forward<Args>(args)...);
}

/**
 * @brief invoke callback function for individual subclasses of expression only for intermediate execution plan.
 * @details if the input object has type of T, this may invoke Callback::operator()(T&, Args...).
 * @attention You must declare all callback functions for individual subclasses,
 * or declare Callback::operator()(expression const&, Args...) as "default" callback function.
 * Each return type of callback function must be identical.
 * @tparam Callback the callback object type
 * @tparam Args the callback argument types
 * @param callback the callback object
 * @param object the target expression
 * @param args the callback arguments
 * @return the callback result
 * @throws std::invalid_argument if the expression is not available in step execution plan
 */
template<class Callback, class... Args>
inline auto dispatch(Callback&& callback, expression const& object, Args&&... args) {
    return impl::dispatch_expression(std::forward<Callback>(callback), object, std::forward<Args>(args)...);
}

/**
 * @brief invoke callback function for individual subclasses of expression only for intermediate execution plan.
 * @details if the input object has type of T, this may invoke Callback::operator()(T&&, Args...).
 * @attention You must declare all callback functions for individual subclasses,
 * or declare Callback::operator()(expression&&, Args...) as "default" callback function.
 * Each return type of callback function must be identical.
 * @tparam Callback the callback object type
 * @tparam Args the callback argument types
 * @param callback the callback object
 * @param object the target expression
 * @param args the callback arguments
 * @return the callback result
 * @throws std::invalid_argument if the expression is not available in step execution plan
 */
template<class Callback, class... Args>
inline auto dispatch(Callback&& callback, expression&& object, Args&&... args) {
    return impl::dispatch_expression(std::forward<Callback>(callback), std::move(object), std::forward<Args>(args)...);
}

} // namespace takatori::relation::intermediate
