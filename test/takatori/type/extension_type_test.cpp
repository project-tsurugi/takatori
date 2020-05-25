#include <takatori/type/extension.h>

#include <gtest/gtest.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>
#include <takatori/util/hash.h>

#include "dummy_extension.h"

namespace takatori::type {

class extension_type_test : public ::testing::Test {
public:
    extension& upcast(extension& object) const noexcept { return object; }
    extension& upcast(extension&& object) const noexcept { return object; }
};

static_assert(extension::tag == type_kind::extension);
static_assert(std::is_same_v<type_of_t<extension::tag>, extension>);

TEST_F(extension_type_test, simple) {
    dummy_extension v { "Hello, world!" };

    extension& t = upcast(v);
    EXPECT_EQ(t.kind(), extension::tag);
    EXPECT_EQ(t.extension_id(), dummy_extension::extension_tag);
    EXPECT_EQ(t, dummy_extension("Hello, world!"));
    EXPECT_NE(t, dummy_extension("OTHER"));
}

TEST_F(extension_type_test, hash) {
    using util::hash;
    EXPECT_EQ(hash(upcast(dummy_extension("A"))), hash(upcast(dummy_extension("A"))));
    EXPECT_NE(hash(upcast(dummy_extension("A"))), hash(upcast(dummy_extension("B"))));
}

TEST_F(extension_type_test, clone) {
    dummy_extension v { "Hello, world!" };
    extension& t = upcast(v);

    auto copy = util::clone_unique(t);
    EXPECT_EQ(t, *copy);
    EXPECT_NE(std::addressof(t), copy.get());
}

TEST_F(extension_type_test, clone_move) {
    dummy_extension v { "Hello, world!" };
    extension& t = upcast(v);

    auto copy = util::clone_unique(t);
    EXPECT_EQ(t, *copy);
    EXPECT_NE(std::addressof(t), copy.get());

    auto move = util::clone_unique(std::move(t));
    EXPECT_NE(std::addressof(t), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(extension_type_test, output) {
    dummy_extension v { "Hello, world!" };
    extension& t = upcast(v);

    std::cout << t << std::endl;
}

} // namespace takatori::type
