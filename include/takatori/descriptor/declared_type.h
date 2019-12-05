#pragma once

#include "element.h"

namespace takatori::descriptor {

/**
 * @brief describes a declared type (a.k.a. user defined type).
 */
using declared_type = element<descriptor_kind::declared_type>;

} // namespace takatori::descriptor
