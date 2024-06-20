#pragma once

#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace SimpleBLE {

/**
 * @class ByteArray
 * @brief A class to handle byte arrays and their conversion from/to hex strings.
 */
class ByteArray {
  public:
    /**
     * @brief Default constructor.
     */
    ByteArray() = default;

    /**
     * @brief Constructs ByteArray from a vector of uint8_t.
     * @param vec A vector of uint8_t.
     */
    ByteArray(const std::vector<uint8_t>& vec) : data_(vec) {}

    /**
     * @brief Constructs ByteArray from a raw pointer and size.
     * @param ptr A pointer to uint8_t data.
     * @param size The size of the data.
     */
    ByteArray(const uint8_t* ptr, size_t size) : data_(ptr, ptr + size) {}

    /**
     * @brief Constructs ByteArray from a std::string.
     * @param byteArr A string containing byte data.
     */
    ByteArray(const std::string& byteArr) : data_(byteArr.begin(), byteArr.end()) {}

    /**
     * @brief Constructs ByteArray from a C-style string and size.
     * @param byteArr A C-style string.
     * @param size The size of the string.
     */
    ByteArray(const char* byteArr, size_t size) : ByteArray(std::string(byteArr, size)) {}

    /**
     * @brief Constructs ByteArray from a C-style string.
     * @param byteArr A C-style string.
     */
    ByteArray(const char* byteArr) : ByteArray(std::string(byteArr)) {}

    /**
     * @brief Creates a ByteArray from a hex string.
     * @param byteArr A C-style string containing hex data.
     * @return A ByteArray object.
     */
    static ByteArray fromHex(const char* byteArr) { return fromHex(std::string(byteArr)); }

    /**
     * @brief Creates a ByteArray from a hex string.
     * @param byteArr A C-style string containing hex data.
     * @param size The size of the string.
     * @return A ByteArray object.
     */
    static ByteArray fromHex(const char* byteArr, size_t size) { return fromHex(std::string(byteArr, size)); }

    /**
     * @brief Creates a ByteArray from a hex string.
     * @param hexStr A string containing hex data.
     * @return A ByteArray object.
     */
    static ByteArray fromHex(const std::string& hexStr) {
        size_t size = hexStr.size();
        if (size % 2 != 0) {
            throw std::length_error("Hex string length must be even.");
        }

        ByteArray byteArray;
        byteArray.data_.reserve(size / 2);

        for (size_t i = 0; i < size; i += 2) {
            uint8_t byte = hexCharToByte(hexStr[i]) * 16 + hexCharToByte(hexStr[i + 1]);
            byteArray.data_.push_back(byte);
        }

        return byteArray;
    }

    /**
     * @brief Converts the ByteArray to a C-style string.
     * @note The returned pointer is a `unique_ptr` to ensure proper memory management.
     *       The caller takes ownership of the `unique_ptr`, and the memory will be automatically
     *       deallocated when the `unique_ptr` goes out of scope.
     * @return A `unique_ptr` to a C-style string.
     */
    std::unique_ptr<char[]> c_str() const {
        std::unique_ptr<char[]> buffer(new char[data_.size() + 1]);
        std::copy(data_.begin(), data_.end(), buffer.get());
        buffer[data_.size()] = '\0';
        return buffer;
    }

    /**
     * @brief Copies the contents of the ByteArray to a provided buffer as a C-style string.
     *
     * This function converts the ByteArray to a null-terminated C-style string by copying
     * its contents into the provided buffer. The buffer should be large enough to hold
     * all the elements of the ByteArray plus the null-terminator.
     *
     * @param[out] buffer Pointer to a buffer where the C-style string will be copied.
     *                    The caller is responsible for ensuring the buffer is properly allocated.
     */
    void c_str(unsigned char* buffer) const {
        std::copy(data_.begin(), data_.end(), buffer);
        buffer[data_.size()] = '\0';
    }

    /**
     * @brief Converts the ByteArray to a hex string.
     * @param spacing Whether to include spaces between bytes.
     *
     * @return A hex string representation of the ByteArray.
     */
    std::string toHexString(bool spacing = false) const {
        std::ostringstream oss;
        for (auto byte : data_) {
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
            if (spacing) {
                oss << " ";
            }
        }
        return oss.str();
    }

    /**
     * @brief Overloaded stream insertion operator for ByteArray.
     * @param os The output stream.
     * @param byteArray The ByteArray object.
     * @return The output stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const ByteArray& byteArray) {
        os << byteArray.toHexString(true);
        return os;
    }

    /**
     * @brief Conversion operator to convert ByteArray to std::string.
     *
     * @note This is provided to support code that relies on ByteArray
     *       being representd as a string.
     * @return String containing the raw bytes of the ByteArray
     */
    operator std::string() const { return std::string(data_.begin(), data_.end()); }

    // Expose vector-like functionality
    size_t size() const { return data_.size(); }
    const uint8_t* data() const { return data_.data(); }
    bool empty() const { return data_.empty(); }
    void clear() { data_.clear(); }
    uint8_t& operator[](size_t index) { return data_[index]; }
    const uint8_t& operator[](size_t index) const { return data_[index]; }
    void push_back(uint8_t byte) { data_.push_back(byte); }
    auto begin() const { return data_.begin(); }
    auto end() const { return data_.end(); }

  private:
    std::vector<uint8_t> data_;

    /**
     * @brief Converts a hex character to a byte.
     * @param hexChar A hex character.
     * @return The byte value of the hex character.
     * @throws std::invalid_argument if the character is not a valid hex character.
     */
    static uint8_t hexCharToByte(char hexChar) {
        if (hexChar >= '0' && hexChar <= '9') {
            return hexChar - '0';
        } else if (hexChar >= 'A' && hexChar <= 'F') {
            return hexChar - 'A' + 10;
        } else if (hexChar >= 'a' && hexChar <= 'f') {
            return hexChar - 'a' + 10;
        } else {
            throw std::invalid_argument(std::string("Invalid hex character: ") + hexChar);
        }
    }
};

}  // namespace SimpleBLE
