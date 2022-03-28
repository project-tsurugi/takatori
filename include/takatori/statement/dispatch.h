#pragma once

#include <type_traits>

#include <cstdlib>

#include "statement.h"
#include "statement_kind.h"

#include "execute.h"
#include "write.h"
#include "create_table.h"
#include "drop_table.h"
#include "create_index.h"
#include "drop_index.h"
#include "empty.h"
#include "extension.h"

#include <takatori/util/callback.h>

namespace takatori::statement {

/// @cond IMPL_DEFS
namespace impl {

template<class Callback, class E, class... Args>
inline auto dispatch_statement(Callback&& callback, E&& statement, Args&&... args) {
    switch (statement.kind()) {
        case execute::tag: return util::polymorphic_callback<execute>(std::forward<Callback>(callback), std::forward<E>(statement), std::forward<Args>(args)...);
        case write::tag: return util::polymorphic_callback<write>(std::forward<Callback>(callback), std::forward<E>(statement), std::forward<Args>(args)...);
        case create_table::tag: return util::polymorphic_callback<create_table>(std::forward<Callback>(callback), std::forward<E>(statement), std::forward<Args>(args)...);
        case drop_table::tag: return util::polymorphic_callback<drop_table>(std::forward<Callback>(callback), std::forward<E>(statement), std::forward<Args>(args)...);
        case create_index::tag: return util::polymorphic_callback<create_index>(std::forward<Callback>(callback), std::forward<E>(statement), std::forward<Args>(args)...);
        case drop_index::tag: return util::polymorphic_callback<drop_index>(std::forward<Callback>(callback), std::forward<E>(statement), std::forward<Args>(args)...);
        case empty::tag: return util::polymorphic_callback<empty>(std::forward<Callback>(callback), std::forward<E>(statement), std::forward<Args>(args)...);
        case extension::tag: return util::polymorphic_callback<extension>(std::forward<Callback>(callback), std::forward<E>(statement), std::forward<Args>(args)...);
        default: break;
    }
    std::abort();
}

} // namespace impl
/// @endcond

/**
 * @brief invoke callback function for individual subclasses of statement.
 * @details If the input object has type of T, this may invoke Callback::operator()(T&, Args...).
 * @attention You must declare all callback functions for individual subclasses,
 * or declare Callback::operator()(statement&, Args...) as "default" callback function.
 * Each return type of callback function must be identical.
 * @tparam Callback the callback object type
 * @tparam Args the callback argument types
 * @param callback the callback object
 * @param object the target statement
 * @param args the callback arguments
 * @return the callback result
 */
template<class Callback, class... Args>
inline auto dispatch(Callback&& callback, statement& object, Args&&... args) {
    return impl::dispatch_statement(std::forward<Callback>(callback), object, std::forward<Args>(args)...);
}

/**
 * @brief invoke callback function for individual subclasses of statement.
 * @details If the input object has type of T, this may invoke Callback::operator()(T const&, Args...).
 * @attention You must declare all callback functions for individual subclasses,
 * or declare Callback::operator()(statement const&, Args...) as "default" callback function.
 * Each return type of callback function must be identical.
 * @tparam Callback the callback object type
 * @tparam Args the callback argument types
 * @param callback the callback object
 * @param object the target statement
 * @param args the callback arguments
 * @return the callback result
 */
template<class Callback, class... Args>
inline auto dispatch(Callback&& callback, statement const& object, Args&&... args) {
    return impl::dispatch_statement(std::forward<Callback>(callback), object, std::forward<Args>(args)...);
}

/**
 * @brief invoke callback function for individual subclasses of statement.
 * @details If the input object has type of T, this may invoke Callback::operator()(T&&, Args...).
 * @attention You must declare all callback functions for individual subclasses,
 * or declare Callback::operator()(statement&&, Args...) as "default" callback function.
 * Each return type of callback function must be identical.
 * @tparam Callback the callback object type
 * @tparam Args the callback argument types
 * @param callback the callback object
 * @param object the target statement
 * @param args the callback arguments
 * @return the callback result
 */
template<class Callback, class... Args>
inline auto dispatch(Callback&& callback, statement&& object, Args&&... args) {
    return impl::dispatch_statement(std::forward<Callback>(callback), std::move(object), std::forward<Args>(args)...);
}

} // namespace takatori::statement
