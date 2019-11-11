#pragma once

#include <map>
#include <optional>

#include "takatori/descriptor/element_descriptor.h"

#include "takatori/scalar/immediate.h"
#include "takatori/scalar/variable_reference.h"

#include "takatori/util/optional_ptr.h"

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

template<descriptor::descriptor_kind Kind>
inline int resolve(descriptor::element_descriptor<Kind> const& desc) {
    return *std::reinterpret_pointer_cast<int>(desc.entity());
}

inline descriptor::value_descriptor value(int v) {
    return impl::get_descriptor<descriptor::value_descriptor>(v);
}

inline descriptor::variable_descriptor vardesc(int v) {
    return impl::get_descriptor<descriptor::variable_descriptor>(v);
}

inline descriptor::function_descriptor funcdesc(int v) {
    return impl::get_descriptor<descriptor::function_descriptor>(v);
}

inline descriptor::type_descriptor typedesc(int v) {
    return impl::get_descriptor<descriptor::type_descriptor>(v);
}

inline immediate constant(int v, descriptor::type_descriptor type = typedesc(-1)) {
    return { value(v), std::move(type) };
}

inline variable_reference varref(int v) {
    return {vardesc(v) };
}

} // namespace takatori::scalar
