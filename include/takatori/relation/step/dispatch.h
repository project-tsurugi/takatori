#pragma once

#include <type_traits>

#include "../expression.h"
#include "../expression_kind.h"

#include "../find.h"
#include "../scan.h"
#include "../join_find.h"
#include "../join_scan.h"
#include "../project.h"
#include "../filter.h"
#include "../buffer.h"
#include "../emit.h"
#include "../write.h"

#include "join.h"
#include "aggregate.h"
#include "intersection.h"
#include "difference.h"
#include "flatten.h"
#include "take_flat.h"
#include "take_group.h"
#include "take_cogroup.h"
#include "offer.h"

#include <takatori/util/callback.h>

namespace takatori::relation::step {

/// @private
namespace impl {

[[noreturn]] inline void invalid_expression(expression const& object) {
    throw std::invalid_argument(
            std::string { "unsupported relational operator" } += to_string_view(object.kind()));
}

/// @private
template<class Callback, class E, class... Args>
inline auto dispatch_expression(Callback&& callback, E&& object, Args&&... args) {
    switch (object.kind()) {
        case find::tag: return util::polymorphic_callback<find>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case scan::tag: return util::polymorphic_callback<scan>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case join_find::tag: return util::polymorphic_callback<join_find>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case join_scan::tag: return util::polymorphic_callback<join_scan>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case project::tag: return util::polymorphic_callback<project>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case filter::tag: return util::polymorphic_callback<filter>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case buffer::tag: return util::polymorphic_callback<buffer>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case emit::tag: return util::polymorphic_callback<emit>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case write::tag: return util::polymorphic_callback<write>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);

        case join::tag: return util::polymorphic_callback<join>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case aggregate::tag: return util::polymorphic_callback<aggregate>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case intersection::tag: return util::polymorphic_callback<intersection>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case difference::tag: return util::polymorphic_callback<difference>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case flatten::tag: return util::polymorphic_callback<flatten>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case take_flat::tag: return util::polymorphic_callback<take_flat>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case take_group::tag: return util::polymorphic_callback<take_group>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case take_cogroup::tag: return util::polymorphic_callback<take_cogroup>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case offer::tag: return util::polymorphic_callback<offer>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);

        // FIXME: extension
        default: break;
    }
    invalid_expression(object);
}

} // namespace impl

/**
 * @brief invoke callback function for individual subclasses of expression only for step execution plan.
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
 * @brief invoke callback function for individual subclasses of expression only for step execution plan.
 * @details if the input object has type of T, this may invoke Callback::operator()(T const&, Args...).
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
 * @brief invoke callback function for individual subclasses of expression only for step execution plan.
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

} // namespace takatori::relation::step
