#include <takatori/value/character.h>

#include <gtest/gtest.h>

#include <string_view>

namespace takatori::value {

class truncate_utf8_test : public ::testing::Test {};

TEST_F(truncate_utf8_test, in_size) {
    std::string_view input {
            "01234",
    };
    auto r = truncate_utf8(input, 5);
    EXPECT_EQ(r, input);
}

TEST_F(truncate_utf8_test, trim1_1b) {
    std::string_view input {
            "01234A",
    };
    auto r = truncate_utf8(input, input.size() - 1);
    EXPECT_EQ(r, input.substr(0, input.size() - 1));
}

TEST_F(truncate_utf8_test, trim1_2b_lo) {
    std::string_view input {
            "01234\xc2\x80",
    };
    auto r = truncate_utf8(input, input.size() - 1);
    EXPECT_EQ(r, input.substr(0, input.size() - 2));
}

TEST_F(truncate_utf8_test, trim1_2b_hi) {
    std::string_view input {
            "01234\xdf\xbf",
    };
    auto r = truncate_utf8(input, input.size() - 1);
    EXPECT_EQ(r, input.substr(0, input.size() - 2));
}

TEST_F(truncate_utf8_test, trim1_3b_lo) {
    std::string_view input {
            "01234\xe0\x80\x80",
    };
    auto r = truncate_utf8(input, input.size() - 1);
    EXPECT_EQ(r, input.substr(0, input.size() - 3));
}

TEST_F(truncate_utf8_test, trim1_3b_hi) {
    std::string_view input {
            "01234\xef\xbf\xbf",
    };
    auto r = truncate_utf8(input, input.size() - 1);
    EXPECT_EQ(r, input.substr(0, input.size() - 3));
}

TEST_F(truncate_utf8_test, trim2_3b_lo) {
    std::string_view input {
            "01234\xe0\x80\x80",
    };
    auto r = truncate_utf8(input, input.size() - 2);
    EXPECT_EQ(r, input.substr(0, input.size() - 3));
}

TEST_F(truncate_utf8_test, trim2_3b_hi) {
    std::string_view input {
            "01234\xef\xbf\xbf",
    };
    auto r = truncate_utf8(input, input.size() - 2);
    EXPECT_EQ(r, input.substr(0, input.size() - 3));
}

TEST_F(truncate_utf8_test, trim1_4b_lo) {
    std::string_view input {
            "01234\xf0\x80\x80\x80",
    };
    auto r = truncate_utf8(input, input.size() - 1);
    EXPECT_EQ(r, input.substr(0, input.size() - 4));
}

TEST_F(truncate_utf8_test, trim1_4b_hi) {
    std::string_view input {
            "01234\xf4\xbf\xbf\xbf",
    };
    auto r = truncate_utf8(input, input.size() - 1);
    EXPECT_EQ(r, input.substr(0, input.size() - 4));
}

TEST_F(truncate_utf8_test, trim2_4b_lo) {
    std::string_view input {
            "01234\xf0\x80\x80\x80",
    };
    auto r = truncate_utf8(input, input.size() - 2);
    EXPECT_EQ(r, input.substr(0, input.size() - 4));
}

TEST_F(truncate_utf8_test, trim2_4b_hi) {
    std::string_view input {
            "01234\xf4\xbf\xbf\xbf",
    };
    auto r = truncate_utf8(input, input.size() - 2);
    EXPECT_EQ(r, input.substr(0, input.size() - 4));
}

TEST_F(truncate_utf8_test, trim3_4b_lo) {
    std::string_view input {
            "01234\xf0\x80\x80\x80",
    };
    auto r = truncate_utf8(input, input.size() - 3);
    EXPECT_EQ(r, input.substr(0, input.size() - 4));
}

TEST_F(truncate_utf8_test, trim3_4b_hi) {
    std::string_view input {
            "01234\xf4\xbf\xbf\xbf",
    };
    auto r = truncate_utf8(input, input.size() - 3);
    EXPECT_EQ(r, input.substr(0, input.size() - 4));
}

TEST_F(truncate_utf8_test, trim1_broken_lo) {
    std::string_view input {
            "01234\x80\x80\x80\x80",
    };
    auto r = truncate_utf8(input, input.size() - 1);
    EXPECT_EQ(r, input.substr(0, input.size() - 1));
}

TEST_F(truncate_utf8_test, trim1_broken_hi) {
    std::string_view input {
            "01234\xff_",
    };
    auto r = truncate_utf8(input, input.size() - 1);
    EXPECT_EQ(r, input.substr(0, input.size() - 1));
}

TEST_F(truncate_utf8_test, trim2_broken_lo) {
    std::string_view input {
            "01234\x80\x80\x80\x80",
    };
    auto r = truncate_utf8(input, input.size() - 2);
    EXPECT_EQ(r, input.substr(0, input.size() - 4));
}

TEST_F(truncate_utf8_test, trim2_broken_hi) {
    std::string_view input {
            "01234\xff__",
    };
    auto r = truncate_utf8(input, input.size() - 2);
    EXPECT_EQ(r, input.substr(0, input.size() - 2));
}

TEST_F(truncate_utf8_test, trim3_broken_lo) {
    std::string_view input {
            "01234\x80\x80\x80\x80",
    };
    auto r = truncate_utf8(input, input.size() - 3);
    EXPECT_EQ(r, input.substr(0, input.size() - 4));
}

TEST_F(truncate_utf8_test, trim3_broken_hi) {
    std::string_view input {
            "01234\xff___",
    };
    auto r = truncate_utf8(input, input.size() - 3);
    EXPECT_EQ(r, input.substr(0, input.size() - 3));
}

TEST_F(truncate_utf8_test, trim1_shorten1b) {
    std::string_view input {
            "\x80_",
    };
    auto r = truncate_utf8(input, input.size() - 1);
    EXPECT_EQ(r, input.substr(0, input.size() - 1));
}

TEST_F(truncate_utf8_test, trim1_shorten2b) {
    std::string_view input {
            "\x80\x80_",
    };
    auto r = truncate_utf8(input, input.size() - 1);
    EXPECT_EQ(r, input.substr(0, input.size() - 1));
}

TEST_F(truncate_utf8_test, trim1_shorten3b) {
    std::string_view input {
            "\x80\x80\x80_",
    };
    auto r = truncate_utf8(input, input.size() - 1);
    EXPECT_EQ(r, input.substr(0, input.size() - 1));
}

} // namespace takatori::value
