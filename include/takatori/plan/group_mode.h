#pragma once

#include <ostream>
#include <string>
#include <string_view>

#include <cstdlib>

namespace takatori::plan {

/**
 * @brief kind of how organize groups in shuffle exchange.
 */
enum class group_mode {
    /**
     * @brief group by equivalence relation.
     * @details This mode is differ with equivalence_or_whole when the group key is absent.
     *      If group key is absent and the input relation is empty, the exchange will not organizes any groups.
     */
    equivalence,

    /**
     * @brief group by equivalence relation, or use whole relation as a group if group key is absent.
     * @details This is designed as SQL compatible mode.
     *     In the original SQL, "SELECT COUNT(*) FROM TBL" always returns a single row even if the "TBL" is empty.
     */
    equivalence_or_whole,
};

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
inline constexpr std::string_view to_string_view(group_mode value) noexcept {
    using namespace std::string_view_literals;
    using kind = group_mode;
    switch (value) {
        case kind::equivalence: return "equivalence"sv;
        case kind::equivalence_or_whole: return "equivalence_or_whole"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, group_mode value) {
    return out << to_string_view(value);
}

} // namespace takatori::plan
