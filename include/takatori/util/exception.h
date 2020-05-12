#pragma once

#include <stdexcept>

#include "stacktrace.h"

#include <boost/exception/all.hpp>

// FIXME: move to common core module
namespace takatori::util {

/// @brief extended error information about stacktrace.
using stacktrace_info = ::boost::error_info<struct tag_stacktrace, boost::stacktrace::stacktrace>;

/**
 * @brief throws the given exception.
 * @details this may attach stacktrace information to the given exception if it is enabled.
 *      Developers can obtain it by find_trace() in the corresponded catch block.
 * @tparam T the exception type
 * @param exception the exception to throw
 * @see find_trace()
 * @see print_trace()
 */
template<class T>
[[noreturn]] void throw_exception(T const& exception) {
    throw ::boost::enable_error_info(exception) << stacktrace_info { ::boost::stacktrace::stacktrace {} }; // NOLINT
}

/**
 * @brief obtains the stacktrace information in the given exception.
 * @param exception the target exception
 * @return the corresponded stacktrace information
 * @return empty if the exception has no stacktrace information
 * @see throw_exception()
 * @see stacktrace_info
 */
::boost::stacktrace::stacktrace const* find_trace(std::exception const& exception);

/**
 * @brief appends stacktrace information into the given output stream.
 * @details This will do nothing if the given exception has no stacktrace information.
 * @param out the destination output stream
 * @param exception the source exception
 * @see throw_exception()
 * @see stacktrace_info
 */
void print_trace(std::ostream& out, std::exception const& exception);

} // namespace takatori::util
