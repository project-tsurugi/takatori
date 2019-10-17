#pragma once

#if defined(USE_BOOST_MEMORY_RESOURCE)

#include <boost/container/pmr/memory_resource.hpp>
#include <boost/container/pmr/polymorphic_allocator.hpp>

#include <boost/container/pmr/global_resource.hpp>
#include <boost/container/pmr/monotonic_buffer_resource.hpp>
#include <boost/container/pmr/synchronized_pool_resource.hpp>
#include <boost/container/pmr/unsynchronized_pool_resource.hpp>

namespace util::pmr {
using namespace boost::container::pmr;
} // namespace util::pmr

#elif __has_include(<memory_resource>)

#include <memory_resource>
namespace util::pmr {
using namespace std::pmr;
} // namespace util::pmr

#elif __has_include(<experimental/memory_resource>)

#include <experimental/memory_resource>
namespace util::pmr {
using namespace std::experimental::pmr;
} // namespace util::pmr

#else

// FIXME: fallback to boost::pmr

#error "missing memory_resource"

#endif
