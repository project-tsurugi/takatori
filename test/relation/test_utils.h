#pragma once

#include <map>
#include <string>
#include <string_view>

#include "takatori/descriptor/aggregate_function.h"
#include "takatori/descriptor/relation.h"
#include "takatori/descriptor/variable.h"

#include "../scalar/test_utils.h"

namespace takatori::relation {

using scalar::value_object;
using scalar::value_of;
using scalar::vardesc;
using scalar::funcdesc;
using scalar::constant;
using scalar::varref;

inline descriptor::relation tabledesc(std::string_view name) {
    return descriptor::relation { std::make_shared<value_object<std::string>>(std::string {name}) };
}

inline descriptor::relation exchangedesc(std::string_view name) {
    return descriptor::relation { std::make_shared<value_object<std::string>>(std::string {name}) };
}

inline descriptor::variable columndesc(std::string_view name) {
    return descriptor::variable { std::make_shared<value_object<std::string>>(std::string {name}) };
}

inline descriptor::aggregate_function aggdesc(std::string_view name) {
    return descriptor::aggregate_function { std::make_shared<value_object<std::string>>(std::string {name}) };
}

} // namespace takatori::scalar
