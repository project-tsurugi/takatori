#pragma once

#include "element.h"

namespace takatori::descriptor {

/**
 * @brief describes a variable.
 */
using variable = element<descriptor_kind::variable, void>;

} // namespace takatori::descriptor
