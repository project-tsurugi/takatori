#include <takatori/serializer/json_printer.h>

#include <sstream>
#include <string>

#include <gtest/gtest.h>

namespace takatori::serializer {

class json_printer_test : public ::testing::Test {
protected:
    std::ostringstream buf;
};

TEST_F(json_printer_test, integer) {
    json_printer printer { buf };
    printer.integer(1);
    EXPECT_EQ(buf.str(), "1");
}

TEST_F(json_printer_test, unsigned_integer) {
    json_printer printer { buf };
    printer.unsigned_integer(1);
    EXPECT_EQ(buf.str(), "1");
}

TEST_F(json_printer_test, binary_float) {
    json_printer printer { buf };
    printer.binary_float(1.25);
    EXPECT_EQ(buf.str(), "1.25");
}

TEST_F(json_printer_test, number) {
    json_printer printer { buf };
    printer.number(decimal::triple { "1.25" });
    EXPECT_EQ(buf.str(), "1.25");
}

TEST_F(json_printer_test, string) {
    json_printer printer { buf };
    printer.string("Hello");
    EXPECT_EQ(buf.str(), "\"Hello\"");
}

TEST_F(json_printer_test, string_escape) {
    json_printer printer { buf };
    printer.string("\r\n\t\"\\");
    EXPECT_EQ(buf.str(), "\"\\r\\n\\t\\\"\\\\\"");
}

TEST_F(json_printer_test, string_multibyte) {
    json_printer printer { buf };
    printer.string("\u{3042}");
    EXPECT_EQ(buf.str(), "\"\u{3042}\"");
}

TEST_F(json_printer_test, boolean) {
    json_printer printer { buf };
    printer.boolean(true);
    EXPECT_EQ(buf.str(), "true");
}

TEST_F(json_printer_test, pointer) {
    json_printer printer { buf };
    printer.pointer(nullptr);
    EXPECT_EQ(buf.str(), "\"@1\"");
}

TEST_F(json_printer_test, pointer_disable_index) {
    json_printer printer { buf };
    printer.enable_pointer_index(false);

    printer.pointer(&printer);
    auto s = buf.str();
    EXPECT_EQ(s.find("\"0x"), 0);
    EXPECT_EQ(s.rfind('"'), s.size() - 1);
}

TEST_F(json_printer_test, struct_empty) {
    json_printer printer { buf };
    printer.struct_begin();
    printer.struct_end();
    EXPECT_EQ(buf.str(), "{}");
}

TEST_F(json_printer_test, struct_property) {
    json_printer printer { buf };
    printer.struct_begin();

    printer.property_begin("value");
    printer.integer(1);
    printer.property_end();

    printer.struct_end();
    EXPECT_EQ(buf.str(), "{\"value\":1}");
}

TEST_F(json_printer_test, struct_property_multiple) {
    json_printer printer { buf };
    printer.struct_begin();

    printer.property_begin("a");
    printer.integer(1);
    printer.property_end();

    printer.property_begin("b");
    printer.integer(2);
    printer.property_end();

    printer.property_begin("c");
    printer.integer(3);
    printer.property_end();

    printer.struct_end();
    EXPECT_EQ(buf.str(), "{\"a\":1,\"b\":2,\"c\":3}");
}

TEST_F(json_printer_test, struct_property_absent) {
    json_printer printer { buf };
    printer.struct_begin();

    printer.property_begin("a");
    printer.integer(1);
    printer.property_end();

    printer.property_begin("b");
    printer.property_end();

    printer.property_begin("c");
    printer.integer(3);
    printer.property_end();

    printer.struct_end();
    EXPECT_EQ(buf.str(), "{\"a\":1,\"c\":3}");
}

TEST_F(json_printer_test, struct_property_null) {
    json_printer printer { buf };
    printer.enable_null_if_absent(true);

    printer.struct_begin();

    printer.property_begin("a");
    printer.integer(1);
    printer.property_end();

    printer.property_begin("b");
    printer.property_end();

    printer.property_begin("c");
    printer.integer(3);
    printer.property_end();

    printer.struct_end();
    EXPECT_EQ(buf.str(), "{\"a\":1,\"b\":null,\"c\":3}");
}

TEST_F(json_printer_test, array_empty) {
    json_printer printer { buf };
    printer.array_begin();
    printer.array_end();
    EXPECT_EQ(buf.str(), "[]");
}

TEST_F(json_printer_test, array_value) {
    json_printer printer { buf };
    printer.array_begin();
    printer.integer(1);
    printer.array_end();
    EXPECT_EQ(buf.str(), "[1]");
}

TEST_F(json_printer_test, array_value_multiple) {
    json_printer printer { buf };
    printer.array_begin();
    printer.integer(1);
    printer.integer(2);
    printer.integer(3);
    printer.array_end();
    EXPECT_EQ(buf.str(), "[1,2,3]");
}

TEST_F(json_printer_test, top_value_multiple) {
    json_printer printer { buf };
    printer.integer(1);
    printer.integer(2);
    printer.integer(3);
    EXPECT_EQ(buf.str(), "1\n2\n3");
}

} // namespace takatori::serializer
