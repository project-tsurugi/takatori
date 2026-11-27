#include <takatori/util/stacktrace.h>

#include <atomic>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <vector>

#include <sys/wait.h>
#include <unistd.h>

#include <gtest/gtest.h>

#include <boost/exception/diagnostic_information.hpp>

#include <takatori/util/exception.h>
#include <takatori/util/symbolizer.h>

namespace takatori::util {

// NOTE: these tests do not pin the contents of human readable strings.

class stacktrace_test : public ::testing::Test {};

TEST_F(stacktrace_test, default_is_empty) {
    stacktrace trace {};
    EXPECT_TRUE(trace.empty());
    EXPECT_TRUE(trace.native().empty());
    EXPECT_TRUE(trace.to_string().empty());
}

TEST_F(stacktrace_test, current) {
    auto trace = current_stacktrace();
    EXPECT_FALSE(trace.empty());
    EXPECT_FALSE(trace.to_string().empty());
}

TEST_F(stacktrace_test, output_operator) {
    auto trace = current_stacktrace();
    std::ostringstream buf {};
    buf << trace;
    EXPECT_EQ(buf.str(), trace.to_string());
}

TEST_F(stacktrace_test, find_trace) {
    try {
        throw_exception(std::out_of_range { "testing" });
    } catch (std::out_of_range const& e) {
        auto const* trace = find_trace(e);
        ASSERT_NE(trace, nullptr);
        EXPECT_FALSE(trace->empty());
        EXPECT_FALSE(trace->to_string().empty());
    }
}

TEST_F(stacktrace_test, diagnostic_information) {
    // boost::diagnostic_information() prints the attached trace through takatori::util::stacktrace
    try {
        throw_exception(std::out_of_range { "testing" });
    } catch (std::out_of_range const& e) {
        auto const* trace = find_trace(e);
        ASSERT_NE(trace, nullptr);
        auto info = ::boost::diagnostic_information(e);
        EXPECT_NE(info.find(trace->to_string()), std::string::npos);
    }
}

TEST_F(stacktrace_test, concurrent) {
    constexpr std::size_t threads = 8;
    constexpr std::size_t iterations = 20;
    std::atomic_size_t failures { 0 };
    std::vector<std::thread> workers {};
    workers.reserve(threads);
    for (std::size_t i = 0; i < threads; ++i) {
        workers.emplace_back([&failures] {
            for (std::size_t j = 0; j < iterations; ++j) {
                if (current_stacktrace().to_string().empty()) {
                    failures.fetch_add(1);
                }
            }
        });
    }
    for (auto& worker : workers) {
        worker.join();
    }
    EXPECT_EQ(failures.load(), 0);
}

TEST_F(stacktrace_test, fork_child) {
    // make sure the process-wide symbolizer exists in the parent (if enabled in this build)
    ASSERT_FALSE(current_stacktrace().to_string().empty());
    ::pid_t pid = ::fork();
    ASSERT_GE(pid, 0);
    if (pid == 0) {
        // in the child: the process-wide symbolizer is not used, but stacktraces are still available
        bool ok = symbolizer::find() == nullptr && !current_stacktrace().to_string().empty();
        ::_exit(ok ? 0 : 1);
    }
    int status = 0;
    ASSERT_EQ(::waitpid(pid, &status, 0), pid);
    ASSERT_TRUE(WIFEXITED(status));
    EXPECT_EQ(WEXITSTATUS(status), 0);
}

} // namespace takatori::util
