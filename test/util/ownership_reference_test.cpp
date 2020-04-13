#include <takatori/util/ownership_reference.h>

#include <gtest/gtest.h>

namespace takatori::util {

class ownership_reference_test : public ::testing::Test {
public:
    object_creator creator { get_standard_memory_resource() };
};

TEST_F(ownership_reference_test, simple) {
    unique_object_ptr<int> v = creator.create_unique<int>(100);
    ownership_reference r { v };

    EXPECT_EQ(*r, 100);

    r = creator.create_unique<int>(200);
    EXPECT_EQ(*v, 200);
}

TEST_F(ownership_reference_test, getter_setter) {
    int value = 0;
    ownership_reference<int> r {
        [&]() {
            return std::addressof(value);
        },
        [&](unique_object_ptr<int> p) -> void {
            value = *p;
        }
    };

    EXPECT_EQ(r.get(), 0);

    r.set(creator.create_unique<int>(100));
    EXPECT_EQ(value, 100);
}

TEST_F(ownership_reference_test, emptyness) {
    unique_object_ptr<int> v;
    ownership_reference r { v };

    EXPECT_FALSE(r);
    EXPECT_FALSE(r.find());
    EXPECT_THROW(r.get(), std::bad_optional_access);

    v = creator.create_unique<int>(100);
    EXPECT_TRUE(r);
    EXPECT_TRUE(r.find());
    EXPECT_NO_THROW(r.get());
}

TEST_F(ownership_reference_test, output) {
    unique_object_ptr<int> v;
    ownership_reference r { v };
    std::cout << r << std::endl;

    v = creator.create_unique<int>(100);
    std::cout << r << std::endl;
}

} // namespace takatori::util
