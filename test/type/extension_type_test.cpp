#include <takatori/type/extension.h>

#include <gtest/gtest.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>
#include <takatori/util/hash.h>

namespace takatori::type {

class extension_type_test : public ::testing::Test {
public:
    extension& upcast(extension& object) const noexcept { return object; }
    extension& upcast(extension&& object) const noexcept { return object; }
};

static_assert(extension::tag == type_kind::extension);
static_assert(std::is_same_v<type_of_t<extension::tag>, extension>);

class test_ext : public extension {
public:
    static constexpr extension_id_type extension_tag = minimum_user_extension_id + 12'345;

    explicit test_ext(std::string value) noexcept : value_(std::move(value)) {}

    test_ext* clone(util::object_creator creator) const& override {
        return creator.create_object<test_ext>(value_);
    }
    test_ext* clone(util::object_creator creator) && override {
        return creator.create_object<test_ext>(std::move(value_));
    }
    extension_id_type extension_id() const noexcept override {
        return extension_tag;
    }
    std::string& value() noexcept {
        return value_;
    }

protected:
    bool equals(extension const& other) const noexcept override {
        return extension_tag == other.extension_id()
            && value_ == util::unsafe_downcast<test_ext>(other).value_;
    }
    std::size_t hash() const noexcept override {
        return util::hash(extension_tag, value_);
    }
    std::ostream& print_to(std::ostream& out) const override {
        return out << "testing(" << value_ << ")";
    }

private:
    std::string value_;
};

TEST_F(extension_type_test, simple) {
    test_ext v { "Hello, world!" };

    extension& t = upcast(v);
    EXPECT_EQ(t.kind(), extension::tag);
    EXPECT_EQ(t.extension_id(), test_ext::extension_tag);
    EXPECT_EQ(t, test_ext("Hello, world!"));
    EXPECT_NE(t, test_ext("OTHER"));
}

TEST_F(extension_type_test, hash) {
    using util::hash;
    EXPECT_EQ(hash(upcast(test_ext("A"))), hash(upcast(test_ext("A"))));
    EXPECT_NE(hash(upcast(test_ext("A"))), hash(upcast(test_ext("B"))));
}

TEST_F(extension_type_test, clone) {
    test_ext v { "Hello, world!" };
    extension& t = upcast(v);

    auto copy = util::clone_unique(t);
    EXPECT_EQ(t, *copy);
    EXPECT_NE(std::addressof(t), copy.get());
}

TEST_F(extension_type_test, clone_move) {
    test_ext v { "Hello, world!" };
    extension& t = upcast(v);

    auto copy = util::clone_unique(t);
    EXPECT_EQ(t, *copy);
    EXPECT_NE(std::addressof(t), copy.get());

    auto move = util::clone_unique(std::move(t));
    EXPECT_NE(std::addressof(t), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(extension_type_test, output) {
    test_ext v { "Hello, world!" };
    extension& t = upcast(v);

    std::cout << t << std::endl;
}

} // namespace takatori::type
