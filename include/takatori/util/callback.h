#pragma once

#include <functional>
#include <type_traits>
#include <utility>

#include "takatori/util/downcast.h"
#include "takatori/util/infect_qualifier.h"

namespace takatori::util {

/**
 * @brief apply downcast to the given object and invoke the callback.
 * This invokes Callback::operator()(T&&, Args...).
 * @tparam T the downcast target type
 * @tparam Callback the callback object type
 * @tparam Object the target object type
 * @tparam Args the callback extra parameter type
 * @param callback the callback object
 * @param object the target object
 * @param args the callback extra arguments
 * @return the invocation result
 */
template<class T, class Callback, class Object, class... Args>
inline std::enable_if_t<
        std::is_invocable_v<Callback, util::infect_qualifier_t<Object, T>, Args...>,
        std::invoke_result_t<Callback, util::infect_qualifier_t<Object, T>, Args...>>
polymorphic_callback(Callback&& callback, Object&& object, Args&&... args) {
    return std::invoke(
            std::forward<Callback>(callback),
            util::unsafe_downcast<T>(std::forward<Object>(object)),
            std::forward<Args>(args)...);
}

} // namespace takatori::util
