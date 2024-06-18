#include <gtest/gtest.h>
#include "simpleble/ByteArray.h"

using namespace SimpleBLE;

TEST(ByteArrayTest, DefaultConstructor) {
    ByteArray byteArray;
    EXPECT_EQ(byteArray.size(), 0);
}

TEST(ByteArrayTest, VectorConstructor) {
    std::vector<uint8_t> vec = {1, 2, 3, 4};
    ByteArray byteArray(vec);
    EXPECT_EQ(byteArray.size(), 4);
    EXPECT_EQ(byteArray[0], 1);
    EXPECT_EQ(byteArray[1], 2);
    EXPECT_EQ(byteArray[2], 3);
    EXPECT_EQ(byteArray[3], 4);
}

TEST(ByteArrayTest, PointerConstructor) {
    uint8_t data[] = {1, 2, 3, 4};
    ByteArray byteArray(data, 4);
    EXPECT_EQ(byteArray.size(), 4);
    EXPECT_EQ(byteArray[0], 1);
    EXPECT_EQ(byteArray[1], 2);
    EXPECT_EQ(byteArray[2], 3);
    EXPECT_EQ(byteArray[3], 4);
}

TEST(ByteArrayTest, StringConstructor) {
    std::string str = "Hello";
    ByteArray byteArray(str);
    EXPECT_EQ(byteArray.size(), 5);
    EXPECT_EQ(byteArray[0], 'H');
    EXPECT_EQ(byteArray[1], 'e');
    EXPECT_EQ(byteArray[2], 'l');
    EXPECT_EQ(byteArray[3], 'l');
    EXPECT_EQ(byteArray[4], 'o');
}

TEST(ByteArrayTest, CharArrayConstructor) {
    const char str[] = {'H', 'e', 'l', 'l', 'o'};
    ByteArray byteArray(str, 5);
    EXPECT_EQ(byteArray.size(), 5);
    EXPECT_EQ(byteArray[0], 'H');
    EXPECT_EQ(byteArray[1], 'e');
    EXPECT_EQ(byteArray[2], 'l');
    EXPECT_EQ(byteArray[3], 'l');
    EXPECT_EQ(byteArray[4], 'o');
}

TEST(ByteArrayTest, CStringConstructor) {
    const char* str = "Hello";
    ByteArray byteArray(str);
    EXPECT_EQ(byteArray.size(), 5);
    EXPECT_EQ(byteArray[0], 'H');
    EXPECT_EQ(byteArray[1], 'e');
    EXPECT_EQ(byteArray[2], 'l');
    EXPECT_EQ(byteArray[3], 'l');
    EXPECT_EQ(byteArray[4], 'o');
}

TEST(ByteArrayTest, FromHexValid) {
    ByteArray byteArray = ByteArray::fromHex("48656c6C6f");
    EXPECT_EQ(byteArray.size(), 5);
    EXPECT_EQ(byteArray[0], 'H');
    EXPECT_EQ(byteArray[1], 'e');
    EXPECT_EQ(byteArray[2], 'l');
    EXPECT_EQ(byteArray[3], 'l');
    EXPECT_EQ(byteArray[4], 'o');
}

TEST(ByteArrayTest, FromHexInvalid) {
    EXPECT_THROW(ByteArray::fromHex("123"), std::length_error);
    EXPECT_THROW(ByteArray::fromHex("G123"), std::invalid_argument);
}

TEST(ByteArrayTest, CStrMethod) {
    struct Deleter {
        bool* flag;
        void operator()(char* ptr) const {
            delete[] ptr;
            *flag = true;  // Set the flag to true when memory is deallocated
        }
    };

    ByteArray byteArray("Hello");

    bool deallocated = false;
    {
        std::unique_ptr<char[], Deleter> cstr(byteArray.c_str().release(), Deleter{&deallocated});
        EXPECT_STREQ(cstr.get(), "Hello");
    }
    EXPECT_TRUE(deallocated);  // Verify that the memory was deallocated
}

TEST(ByteArrayTest, ToStringMethod) {
    ByteArray byteArray("Hello");
    EXPECT_EQ(byteArray.toHexString(), "48656c6c6f");
    EXPECT_EQ(byteArray.toHexString(true), "48 65 6c 6c 6f ");
}

TEST(ByteArrayTest, StingConversion) {
    ByteArray byteArray = ByteArray::fromHex("48656c6C6f");
    std::string str = static_cast<std::string>(byteArray);
    EXPECT_EQ(str, "Hello");
}

TEST(ByteArrayTest, StreamOperator) {
    ByteArray byteArray("Hello");
    std::ostringstream oss;
    oss << byteArray;
    EXPECT_EQ(oss.str(), "48 65 6c 6c 6f ");
}
