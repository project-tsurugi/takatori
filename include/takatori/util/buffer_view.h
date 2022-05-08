#pragma once

#include "basic_buffer_view.h"

namespace takatori::util {

/**
 * @brief provides a view of buffer without buffer ownership.
 */
using buffer_view = basic_buffer_view<char>;

/**
 * @brief provides a view of unmodifiable buffer without buffer ownership.
 */
using const_buffer_view = basic_buffer_view<char const>;

} // namespace takatori::util
