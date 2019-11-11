#pragma once

#include <type_traits>

#include <cstdlib>

#include "data_type.h"
#include "data_type_kind.h"

#include "boolean.h"
#include "int.h"
#include "float.h"
#include "decimal.h"
#include "character.h"
#include "bit.h"
#include "date.h"
#include "time_of_day.h"
#include "time_point.h"
#include "time_interval.h"
// #include "array.h"
// #include "record.h"
// #include "user.h"
#include "unknown.h"
// #include "row_reference.h"
// #include "row_id.h"
// #include "unresolved.h"
// #include "error.h"

#include "takatori/util/callback.h"

namespace takatori::type {

/**
 * @brief invoke callback function for individual data_type's subclasses.
 * If the actual type of expr is T, this may invoke Callback::operator()(T const&, Args...).
 * You must declare all callback functions for individual subclasses,
 * or declare Callback::operator()(data_type const&, Args...) as "default" callback function.
 * Each return type of callback function must be identical.
 * @tparam Callback the callback object type
 * @tparam Args the callback argument types
 * @param callback the callback object
 * @param type the target data type
 * @param args the callback arguments
 * @return the callback result
 */
template<class Callback, class... Args>
inline auto dispatch(Callback&& callback, data_type const& type, Args&&... args) {
    switch (type.kind()) {
        case boolean::tag: return util::polymorphic_callback<boolean>(std::forward<Callback>(callback), type, std::forward<Args>(args)...);
        case int1::tag: return util::polymorphic_callback<int1>(std::forward<Callback>(callback), type, std::forward<Args>(args)...);
        case int2::tag: return util::polymorphic_callback<int2>(std::forward<Callback>(callback), type, std::forward<Args>(args)...);
        case int4::tag: return util::polymorphic_callback<int4>(std::forward<Callback>(callback), type, std::forward<Args>(args)...);
        case int8::tag: return util::polymorphic_callback<int8>(std::forward<Callback>(callback), type, std::forward<Args>(args)...);
        case float4::tag: return util::polymorphic_callback<float4>(std::forward<Callback>(callback), type, std::forward<Args>(args)...);
        case float8::tag: return util::polymorphic_callback<float8>(std::forward<Callback>(callback), type, std::forward<Args>(args)...);
        case decimal::tag: return util::polymorphic_callback<decimal>(std::forward<Callback>(callback), type, std::forward<Args>(args)...);
        case character::tag: return util::polymorphic_callback<character>(std::forward<Callback>(callback), type, std::forward<Args>(args)...);
        case bit::tag: return util::polymorphic_callback<bit>(std::forward<Callback>(callback), type, std::forward<Args>(args)...);
        case date::tag: return util::polymorphic_callback<date>(std::forward<Callback>(callback), type, std::forward<Args>(args)...);
        case time_of_day::tag: return util::polymorphic_callback<time_of_day>(std::forward<Callback>(callback), type, std::forward<Args>(args)...);
        case time_point::tag: return util::polymorphic_callback<time_point>(std::forward<Callback>(callback), type, std::forward<Args>(args)...);
        case time_interval::tag: return util::polymorphic_callback<time_interval>(std::forward<Callback>(callback), type, std::forward<Args>(args)...);
//        case array::tag: return util::polymorphic_callback<array>(std::forward<Callback>(callback), type, std::forward<Args>(args)...);
//        case record::tag: return util::polymorphic_callback<record>(std::forward<Callback>(callback), type, std::forward<Args>(args)...);
//        case user::tag: return util::polymorphic_callback<user>(std::forward<Callback>(callback), type, std::forward<Args>(args)...);
        case unknown::tag: return util::polymorphic_callback<unknown>(std::forward<Callback>(callback), type, std::forward<Args>(args)...);
//        case row_reference::tag: return util::polymorphic_callback<row_reference>(std::forward<Callback>(callback), type, std::forward<Args>(args)...);
//        case row_id::tag: return util::polymorphic_callback<row_id>(std::forward<Callback>(callback), type, std::forward<Args>(args)...);
//        case unresolved::tag: return util::polymorphic_callback<unresolved>(std::forward<Callback>(callback), type, std::forward<Args>(args)...);
//        case error::tag: return util::polymorphic_callback<error>(std::forward<Callback>(callback), type, std::forward<Args>(args)...);

        // FIXME: other cases
        default: break;
    }
    std::abort();
}

} // namespace takatori::type
