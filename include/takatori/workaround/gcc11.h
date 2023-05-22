#pragma once

#include <type_traits>

#ifdef __GNUC__
    #include <features.h>
    #if __GNUC_PREREQ(11, 0)
        #define TSURUGI_GCC11_WORKAROUND
    #endif
#endif

namespace takatori::workaround::gcc11 {

/**
 * @brief enables the Type only if the Condition is satisfied.
 * @tparam Condition the condition
 * @tparam Type the enabling type
 */
template<bool Condition, class Type = void>
#ifdef TSURUGI_GCC11_WORKAROUND
using enable_if = std::enable_if<true, Type>;
#else
using enable_if = std::enable_if<Condition, Type>;
#endif

/**
 * @brief enables the Type only if the Condition is satisfied.
 * @tparam Condition the condition
 * @tparam Type the enabling type
 */
template<bool Condition, class Type = void>
using enable_if_t = typename enable_if<Condition, Type>::type;


} // namespace takatori::workaround::gcc11
