#pragma once

#include "takatori/util/reference_vector.h"

namespace takatori::scalar {

/**
 * @brief a vector of expressions.
 * @tparam T the element type
 */
template<class T>
using expression_vector = util::reference_vector<T, util::null_copier<T>>;

} // namespace takatori::scalar
