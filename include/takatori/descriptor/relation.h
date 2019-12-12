#pragma once

#include "element.h"

namespace takatori::descriptor {

/**
 * @brief describes an external relation (index, broadcast exchange, or etc.).
 */
using relation = element<descriptor_kind::relation>;

} // namespace takatori::descriptor
