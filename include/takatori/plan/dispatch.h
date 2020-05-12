#pragma once

#include <string>
#include <type_traits>

#include "step.h"
#include "step_kind.h"

#include "process.h"
#include "forward.h"
#include "group.h"
#include "aggregate.h"
#include "broadcast.h"
#include "discard.h"

#include <takatori/util/exception.h>
#include <takatori/util/callback.h>
#include <takatori/util/string_builder.h>

namespace takatori::plan {

/// @private
namespace impl {

[[noreturn]] inline void invalid_step(step const& object) {
    using ::takatori::util::string_builder;
    using ::takatori::util::throw_exception;
    throw_exception(std::invalid_argument(string_builder {}
            << "unsupported step kind: "
            << object.kind()
            << string_builder::to_string));
}

/// @private
template<class Callback, class E, class... Args>
inline auto dispatch_step(Callback&& callback, E&& object, Args&&... args) {
    switch (object.kind()) {
        case process::tag: return util::polymorphic_callback<process>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case forward::tag: return util::polymorphic_callback<forward>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case group::tag: return util::polymorphic_callback<group>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case aggregate::tag: return util::polymorphic_callback<aggregate>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case broadcast::tag: return util::polymorphic_callback<broadcast>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
        case discard::tag: return util::polymorphic_callback<discard>(std::forward<Callback>(callback), std::forward<E>(object), std::forward<Args>(args)...);
    }
    invalid_step(object);
}

} // namespace impl

/**
 * @brief invoke callback function for individual subclasses of step only for intermediate execution plan.
 * @details if the input object has type of T, this may invoke Callback::operator()(T&, Args...).
 * @attention You must declare all callback functions for individual subclasses,
 * or declare Callback::operator()(step&, Args...) as "default" callback function.
 * Each return type of callback function must be identical.
 * @tparam Callback the callback object type
 * @tparam Args the callback argument types
 * @param callback the callback object
 * @param object the target step
 * @param args the callback arguments
 * @return the callback result
 * @throws std::invalid_argument if the step is not available in step execution plan
 */
template<class Callback, class... Args>
inline auto dispatch(Callback&& callback, step& object, Args&&... args) {
    return impl::dispatch_step(std::forward<Callback>(callback), object, std::forward<Args>(args)...);
}

/**
 * @brief invoke callback function for individual subclasses of step only for intermediate execution plan.
 * @details if the input object has type of T, this may invoke Callback::operator()(T&, Args...).
 * @attention You must declare all callback functions for individual subclasses,
 * or declare Callback::operator()(step const&, Args...) as "default" callback function.
 * Each return type of callback function must be identical.
 * @tparam Callback the callback object type
 * @tparam Args the callback argument types
 * @param callback the callback object
 * @param object the target step
 * @param args the callback arguments
 * @return the callback result
 * @throws std::invalid_argument if the step is not available in step execution plan
 */
template<class Callback, class... Args>
inline auto dispatch(Callback&& callback, step const& object, Args&&... args) {
    return impl::dispatch_step(std::forward<Callback>(callback), object, std::forward<Args>(args)...);
}

/**
 * @brief invoke callback function for individual subclasses of step only for intermediate execution plan.
 * @details if the input object has type of T, this may invoke Callback::operator()(T&&, Args...).
 * @attention You must declare all callback functions for individual subclasses,
 * or declare Callback::operator()(step&&, Args...) as "default" callback function.
 * Each return type of callback function must be identical.
 * @tparam Callback the callback object type
 * @tparam Args the callback argument types
 * @param callback the callback object
 * @param object the target step
 * @param args the callback arguments
 * @return the callback result
 * @throws std::invalid_argument if the step is not available in step execution plan
 */
template<class Callback, class... Args>
inline auto dispatch(Callback&& callback, step&& object, Args&&... args) {
    return impl::dispatch_step(std::forward<Callback>(callback), std::move(object), std::forward<Args>(args)...);
}

} // namespace takatori::plan
