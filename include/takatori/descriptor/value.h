#pragma once

#include "element.h"

namespace takatori::descriptor {

// FIXME: migrate to flat values

/**
 * @brief describes a value.
 */
using value = element<descriptor_kind::value, void>;

} // namespace takatori::descriptor
