#include <takatori/document/basic_document.h>

#include <gtest/gtest.h>

namespace takatori::document {

class basic_document_test : public ::testing::Test {};

TEST_F(basic_document_test, simple) {
    std::string location { "testing.sql" };
    std::string contents { "Hello, world!" };
    basic_document doc { location, contents, };

    EXPECT_EQ(doc.location(), location);
    EXPECT_EQ(doc.size(), contents.size());
    EXPECT_EQ(doc.contents(0, contents.size()), contents);

    EXPECT_EQ(doc.position(0), position(1, 1));
    EXPECT_EQ(doc.position(contents.size() - 1), position(1, contents.size()));
}

TEST_F(basic_document_test, eol) {
    std::string location { "testing.sql" };
    std::string contents {
            "A\n"
            "B\n"
            "C\n"
            "DEF"
    };
    basic_document doc { location, contents, };

    EXPECT_EQ(doc.contents(0, 1), "A");
    EXPECT_EQ(doc.contents(2, 1), "B");
    EXPECT_EQ(doc.contents(4, 1), "C");

    EXPECT_EQ(doc.position(0), position(1, 1));
    EXPECT_EQ(doc.position(1), position(1, 2));
    EXPECT_EQ(doc.position(2), position(2, 1));
    EXPECT_EQ(doc.position(3), position(2, 2));
    EXPECT_EQ(doc.position(4), position(3, 1));
    EXPECT_EQ(doc.position(5), position(3, 2));
    EXPECT_EQ(doc.position(6), position(4, 1));
    EXPECT_EQ(doc.position(8), position(4, 3));
    EXPECT_EQ(doc.position(9), position(4, 4));
    EXPECT_EQ(doc.position(10), position());

    EXPECT_EQ(doc.offset({}), document::npos);
    EXPECT_EQ(doc.offset({ 1, 0 }), 0);
    EXPECT_EQ(doc.offset({ 1, 1 }), 0);
    EXPECT_EQ(doc.offset({ 1, 2 }), 1);
    EXPECT_EQ(doc.offset({ 2, 0 }), 2);
    EXPECT_EQ(doc.offset({ 2, 1 }), 2);
    EXPECT_EQ(doc.offset({ 2, 2 }), 3);
    EXPECT_EQ(doc.offset({ 3, 0 }), 4);
    EXPECT_EQ(doc.offset({ 3, 1 }), 4);
    EXPECT_EQ(doc.offset({ 3, 2 }), 5);
    EXPECT_EQ(doc.offset({ 4, 0 }), 6);
    EXPECT_EQ(doc.offset({ 4, 1 }), 6);
    EXPECT_EQ(doc.offset({ 4, 3 }), 8);
    EXPECT_EQ(doc.offset({ 4, 4 }), 9);
    EXPECT_EQ(doc.offset({ 4, 5 }), document::npos);
    EXPECT_EQ(doc.offset({ 5, 0 }), document::npos);
}

} // namespace takatori::document
