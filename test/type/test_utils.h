#pragma once

#include <map>

#include "takatori/descriptor/element.h"
#include "takatori/descriptor/time_zone.h"

namespace takatori::type {

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

inline descriptor::time_zone tz(int v) {
    return impl::get_descriptor<descriptor::time_zone>(v);
}

} // namespace takatori::type
