#pragma once

#include "element.h"

namespace takatori::descriptor {

/**
 * @brief describes a function.
 */
using function = element<descriptor_kind::function, void>;

} // namespace takatori::descriptor
