#pragma once

#include <functional>

#include "element.h"

namespace takatori::descriptor {

/**
 * @brief describes timezone information.
 */
using time_zone = element<descriptor_kind::time_zone, void>;

/*
 * FIXME:
 * - change entity type to internal time zone object
 * - override operator==
 * - override operator==
 * - override hash
 */

} // namespace takatori::descriptor
