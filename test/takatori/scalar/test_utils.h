#pragma once

#include <stdexcept>
#include <unordered_map>

#include <takatori/testing/descriptors.h>

#include <takatori/type/primitive.h>
#include <takatori/value/primitive.h>

#include <takatori/scalar/immediate.h>
#include <takatori/scalar/variable_reference.h>

namespace takatori::scalar {

using testing::value_of;
using testing::vardesc;
using testing::funcdesc;

template<class T>
inline typename T::view_type value_of(expression const& expr) {
    if (auto e = util::dynamic_pointer_cast<immediate>(expr)) {
        if (auto v = util::dynamic_pointer_cast<T>(e->value())) {
            return v->get();
        }
        throw std::domain_error("inconsistent value type");
    }
    throw std::domain_error("unsupported expression");
}

inline immediate constant(int v, type::data&& type = type::int4()) {
    return immediate { value::int4(v), std::move(type) };
}

inline variable_reference varref(int v) {
    return variable_reference { vardesc(v) };
}

} // namespace takatori::scalar
