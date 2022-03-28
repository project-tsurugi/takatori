#pragma once

#include <stdexcept>
#include <utility>

#include "mock_binding_info.h"

#include <takatori/descriptor/variable.h>
#include <takatori/descriptor/relation.h>
#include <takatori/descriptor/function.h>
#include <takatori/descriptor/aggregate_function.h>
#include <takatori/descriptor/schema.h>
#include <takatori/descriptor/storage.h>
#include <takatori/descriptor/declared_type.h>

namespace takatori::testing {

template<class T, descriptor::descriptor_kind Kind>
inline T const& value_of(descriptor::element<Kind> const& desc) {
    return mock_binding_info<Kind, T>::extract(desc.entity());
}

template<class Desc, class Value>
using descriptor_binding_info = mock_binding_info<Desc::tag, Value>;

inline descriptor::variable vardesc(int v) {
    using descriptor_type = descriptor::variable;
    return descriptor_type { std::make_shared<descriptor_binding_info<descriptor_type, int>>(v) };
}

inline descriptor::function funcdesc(int v) {
    using descriptor_type = descriptor::function;
    return descriptor_type { std::make_shared<descriptor_binding_info<descriptor_type, int>>(v) };
}

inline descriptor::relation tabledesc(std::string_view name) {
    using descriptor_type = descriptor::relation;
    return descriptor_type { std::make_shared<descriptor_binding_info<descriptor_type, std::string>>(std::string { name }) };
}

inline descriptor::relation exchangedesc(std::string_view name) {
    using descriptor_type = descriptor::relation;
    return descriptor_type { std::make_shared<descriptor_binding_info<descriptor_type, std::string>>(std::string { name }) };
}

inline descriptor::variable columndesc(std::string_view name) {
    using descriptor_type = descriptor::variable;
    return descriptor_type { std::make_shared<descriptor_binding_info<descriptor_type, std::string>>(std::string { name }) };
}

inline descriptor::aggregate_function aggdesc(std::string_view name) {
    using descriptor_type = descriptor::aggregate_function;
    return descriptor_type { std::make_shared<descriptor_binding_info<descriptor_type, std::string>>(std::string { name }) };
}

inline descriptor::schema schemadesc(std::string_view name) {
    using descriptor_type = descriptor::schema;
    return descriptor_type { std::make_shared<descriptor_binding_info<descriptor_type, std::string>>(std::string { name }) };
}

inline descriptor::storage storagedesc(std::string_view name) {
    using descriptor_type = descriptor::storage;
    return descriptor_type { std::make_shared<descriptor_binding_info<descriptor_type, std::string>>(std::string { name }) };
}

inline descriptor::declared_type typedesc(std::string_view name) {
    using descriptor_type = descriptor::declared_type;
    return descriptor_type { std::make_shared<descriptor_binding_info<descriptor_type, std::string>>(std::string { name }) };
}

} // namespace takatori::testing
