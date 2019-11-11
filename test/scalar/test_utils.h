#pragma once

#include <map>

#include "takatori/type/data_type.h"
#include "takatori/type/int.h"

#include "takatori/descriptor/value.h"
#include "takatori/descriptor/variable.h"
#include "takatori/descriptor/function.h"

#include "takatori/scalar/immediate.h"
#include "takatori/scalar/variable_reference.h"

namespace takatori::scalar {

namespace impl {

inline std::map<int, std::shared_ptr<int>> descriptor_values {};

template<class desc>
inline desc get_descriptor(int v) {
    if (auto iter = descriptor_values.find(v); iter != descriptor_values.end()) {
        return desc(iter->second);
    }
    auto value = std::make_shared<int>(v);
    descriptor_values[v] = value;
    return desc(std::move(value));
}

} // namespace impl

template<descriptor::descriptor_kind K, class E>
inline int resolve(descriptor::element<K, E> const& desc) {
    return *std::reinterpret_pointer_cast<int>(desc.entity());
}

inline descriptor::value value(int v) {
    return impl::get_descriptor<descriptor::value>(v);
}

inline descriptor::variable vardesc(int v) {
    return impl::get_descriptor<descriptor::variable>(v);
}

inline descriptor::function funcdesc(int v) {
    return impl::get_descriptor<descriptor::function>(v);
}

inline immediate constant(int v, type::data_type&& type = type::int4()) {
    return { value(v), std::move(type) };
}

inline variable_reference varref(int v) {
    return {vardesc(v) };
}

} // namespace takatori::scalar
