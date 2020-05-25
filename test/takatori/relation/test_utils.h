#pragma once

#include <takatori/testing/descriptors.h>
#include <takatori/scalar/test_utils.h>

#include <takatori/relation/expression.h>

namespace takatori::relation {

using testing::value_of;
using testing::vardesc;
using testing::funcdesc;
using testing::tabledesc;
using testing::exchangedesc;
using testing::columndesc;
using testing::aggdesc;

using scalar::value_of;
using scalar::constant;
using scalar::varref;

template<class T, graph::port_direction Direction>
bool is_valid_port_list(util::sequence_view<graph::port<T, Direction>> ports) {
    using index_t = typename graph::port<T, Direction>::index_type;
    index_t index = 0;
    for (auto&& p : ports) {
        if (p.index() != index) return false;
        ++index;
    }
    return true;
}

} // namespace takatori::scalar
