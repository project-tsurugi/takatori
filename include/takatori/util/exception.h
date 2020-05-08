#pragma once

#include <stdexcept>

#include "stacktrace.h"

#include <boost/exception/all.hpp>

// FIXME: move to common core module
namespace takatori::util {

/// @brief extended error information about stacktrace.
using stacktrace_info = ::boost::error_info<struct tag_stacktrace, boost::stacktrace::stacktrace>;

/**
 * @brief throws the given exception with stacktrace information.
 * @details the added stacktrace information can obtain using find_trace() later.
 * @tparam T the exception type
 * @param exception the exception to throw
 * @param trace the stacktrace information
 * @see stacktrace_info
 */
template<class T>
[[noreturn]] inline void throw_with_trace(T const& exception, ::boost::stacktrace::stacktrace&& trace = {}) {
    throw ::boost::enable_error_info(exception) << stacktrace_info { std::move(trace) }; // NOLINT
}

/**
 * @brief obtains the stacktrace information in the given exception.
 * @param exception the target exception
 * @return the corresponded stacktrace information
 * @return empty if the exception has no stacktrace information
 * @see throw_with_trace()
 * @see stacktrace_info
 */
::boost::stacktrace::stacktrace const* find_trace(std::exception const& exception);

/**
 * @brief appends stacktrace information into the given output stream.
 * @param out the destination output stream
 * @param exception the source exception
 * @see throw_with_trace()
 * @see stacktrace_info
 */
void print_trace(std::ostream& out, std::exception const& exception);

} // namespace takatori::util
