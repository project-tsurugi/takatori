#include "takatori/util/either.h"
#include "takatori/util/forwarding_memory_resource.h"

#include "takatori/descriptor/variable.h"

template<class T>
class copy_move_ambiguity {
public:
    operator T&&() { return std::declval<T>(); }  // NOLINT
    operator T const&() { return std::declval<T>(); }  // NOLINT
};

template<class T>
struct is_strict_move_constructible
    : std::conjunction<
            std::is_move_constructible<T>,
            std::negation<std::is_constructible<T, copy_move_ambiguity<T>>>>
{};

template<class T>
struct is_strict_move_assignable
        : std::conjunction<
                std::is_move_assignable<T>,
                std::negation<std::is_assignable<T&, copy_move_ambiguity<T>>>>
{};

template<class T>
constexpr bool is_strict_move_constructible_v = is_strict_move_constructible<T>::value;

template<class T>
constexpr bool is_strict_move_assignable_v = is_strict_move_assignable<T>::value;

int main() {
    using t = takatori::descriptor::variable;
    std::cout << std::boolalpha << "move constr: " << is_strict_move_constructible_v<t> << std::endl;
    std::cout << std::boolalpha << "move assign: " << is_strict_move_assignable_v<t> << std::endl;
    return 0;
}
