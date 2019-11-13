#include "takatori/util/forwarding_memory_resource.h"

#include <iostream>
#include <limits>
#include <type_traits>

#include <cstdint>

int main() {
    std::int64_t v = std::numeric_limits<std::int64_t>::max();
    v /= 86400LL * 1'000'000'000LL;
    std::cout << v << ":" << (v / 365) << std::endl;

    return 0;
}
