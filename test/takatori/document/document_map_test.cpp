#include <takatori/document/document_map.h>

#include <gtest/gtest.h>

#include <takatori/document/basic_document.h>

namespace takatori::document {

class document_map_test : public ::testing::Test {};

TEST_F(document_map_test, naked) {
    basic_document doc {
            "testing.sql",
            "Hello, world!",
    };

    document_map docs { doc };
    EXPECT_EQ(docs.find("testing.sql").get(), &doc);
    EXPECT_FALSE(docs.find("MISSING"));
}

TEST_F(document_map_test, shared) {
    document_map docs { std::make_shared<basic_document>("testing.sql", "Hello, world!") };
    auto d = docs.find("testing.sql");

    ASSERT_TRUE(d);
    EXPECT_EQ(d->location(), "testing.sql");
    EXPECT_EQ(d->contents(0, d->size()), "Hello, world!");
}

TEST_F(document_map_test, multiple) {
    document_map docs;
    docs.reserve(3);

    docs.add(std::make_shared<basic_document>("a.sql", "A"));
    docs.add(std::make_shared<basic_document>("b.sql", "B"));
    docs.add(std::make_shared<basic_document>("c.sql", "C"));

    auto a = docs.find("a.sql");
    auto b = docs.find("b.sql");
    auto c = docs.find("c.sql");
    auto d = docs.find("d.sql");

    ASSERT_TRUE(a);
    ASSERT_TRUE(b);
    ASSERT_TRUE(c);
    EXPECT_FALSE(d);

    EXPECT_EQ(a->contents(0, 1), "A");
    EXPECT_EQ(b->contents(0, 1), "B");
    EXPECT_EQ(c->contents(0, 1), "C");
}

TEST_F(document_map_test, erase) {
    document_map docs;
    docs.add(std::make_shared<basic_document>("a.sql", "A"));
    docs.add(std::make_shared<basic_document>("b.sql", "B"));
    docs.add(std::make_shared<basic_document>("c.sql", "C"));

    EXPECT_TRUE(docs.erase("b.sql"));
    EXPECT_FALSE(docs.erase("d.sql"));

    EXPECT_TRUE(docs.find("a.sql"));
    EXPECT_FALSE(docs.find("b.sql"));
    EXPECT_TRUE(docs.find("c.sql"));
}

TEST_F(document_map_test, clear) {
    document_map docs;
    docs.add(std::make_shared<basic_document>("a.sql", "A"));
    docs.add(std::make_shared<basic_document>("b.sql", "B"));
    docs.add(std::make_shared<basic_document>("c.sql", "C"));

    docs.clear();

    EXPECT_FALSE(docs.find("a.sql"));
    EXPECT_FALSE(docs.find("b.sql"));
    EXPECT_FALSE(docs.find("c.sql"));
}

} // namespace takatori::document
