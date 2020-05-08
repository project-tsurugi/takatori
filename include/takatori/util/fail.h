#pragma once

#include <string_view>

namespace takatori::util {

/**
 * @brief abort this execution.
 * @details This finally calls std::abort(), so that you can aware calling this by SIGABRT handler.
 * @param message the optional failure message
 */
[[noreturn]] void fail(std::string_view message = {}) noexcept;

} // namespace takatori::util
