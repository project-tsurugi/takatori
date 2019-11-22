#pragma once

#include <iostream>

#include "descriptor_kind.h"
#include "element.h"
#include "takatori/util/object.h"

#include "takatori/util/optional_ptr.h"

namespace takatori::descriptor {

/**
 * @brief describes an external relation (table, index, broadcast data, or etc.).
 */
using relation = element<descriptor_kind::relation>;

} // namespace takatori::descriptor
