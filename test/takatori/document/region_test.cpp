#include <takatori/document/region.h>

#include <gtest/gtest.h>

#include <takatori/document/basic_document.h>

namespace takatori::document {

class region_test : public ::testing::Test {
protected:
    basic_document doc {
            "testing.sql",
            "0\n"
                    "23\n"
                    "567",
    };
};

TEST_F(region_test, simple) {
    region r { doc,2, 4 };

    EXPECT_EQ(r.source().get(), std::addressof(doc));
    EXPECT_EQ(r.contents().value(), "23");
    EXPECT_EQ(r.first(), 2);
    EXPECT_EQ(r.last(), 4);
    EXPECT_EQ(r.gap(), 2);
    EXPECT_TRUE(r);
}

TEST_F(region_test, invalid) {
    region r;

    EXPECT_EQ(r.source().get(), nullptr);
    EXPECT_FALSE(r.contents());
    EXPECT_FALSE(r.first());
    EXPECT_FALSE(r.last());
    EXPECT_FALSE(r.first_position());
    EXPECT_FALSE(r.last_position());
    EXPECT_FALSE(r.gap());
    EXPECT_FALSE(r);
}

TEST_F(region_test, position) {
    region r {
            doc,
            position(2, 1),
            position(2, 3),
    };

    EXPECT_EQ(r.source().get(), std::addressof(doc));
    EXPECT_EQ(r.first(), 2);
    EXPECT_EQ(r.last(), 4);
    EXPECT_EQ(r.first_position(), position(2, 1));
    EXPECT_EQ(r.last_position(), position(2, 3));
}

TEST_F(region_test, output) {
    region r {
            doc,
            position(2, 1),
            position(2, 3),
    };
    std::cout << r << std::endl;
}

} // namespace takatori::document
