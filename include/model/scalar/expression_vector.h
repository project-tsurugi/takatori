#pragma once

#include "util/reference_vector.h"

namespace model::scalar {

/**
 * @brief a vector of expressions.
 * @tparam T the element type
 */
template<class T>
using expression_vector = util::reference_vector<T, util::null_copier<T>>;

} // namespace model::scalar
