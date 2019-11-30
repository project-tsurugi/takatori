#pragma once

#include "element.h"

namespace takatori::descriptor {

/**
 * @brief describes an external relation (table, index, broadcast data, or etc.).
 */
using relation = element<descriptor_kind::relation>;

} // namespace takatori::descriptor
