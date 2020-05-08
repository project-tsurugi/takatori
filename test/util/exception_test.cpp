#include <takatori/util/exception.h>

#include <gtest/gtest.h>

namespace takatori::util {

class exception_test : public ::testing::Test {
public:
    [[noreturn]] void tt(bool with_trace = true) {
        if (with_trace) {
            throw_with_trace(std::domain_error("w/ trace"));
        } else {
            throw std::domain_error("w/o trace");
        }
    }
};

TEST_F(exception_test, simple) {
    EXPECT_THROW(throw_with_trace(std::out_of_range("OOR")), std::out_of_range);
}

TEST_F(exception_test, find_trace) {
    try {
        throw_with_trace(std::invalid_argument("IR"));
        FAIL();
    } catch (std::invalid_argument const& e) {
        ASSERT_NE(find_trace(e), nullptr);
    }
}

TEST_F(exception_test, find_trace_missing) {
    try {
        throw std::invalid_argument("IR");
    } catch (std::invalid_argument const& e) {
        ASSERT_EQ(find_trace(e), nullptr);
    }
}

TEST_F(exception_test, print_trace) {
    try {
        tt();
        FAIL();
    } catch (std::domain_error const& e) {
        std::cout << "domain_error: " << e.what() << std::endl;
        print_trace(std::cout, e);
    }
}

TEST_F(exception_test, print_trace_mssing) {
    try {
        tt(false);
        FAIL();
    } catch (std::domain_error const& e) {
        std::cout << "domain_error: " << e.what() << std::endl;
        print_trace(std::cout, e);
    }
}

} // namespace takatori::util
