#pragma once

#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace Common {

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
            uint8_t byte = static_cast<uint8_t>(std::stoi(hexStr.substr(i, 2), nullptr, 16));
            byteArray.data_.push_back(byte);
        }

        return byteArray;
    }

    /**
     * @brief Converts the ByteArray to a hex string.
     * @param spacing Whether to include spaces between bytes.
     *
     * @return A hex string representation of the ByteArray.
     */
    std::string toHex(bool spacing = false) const {
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
     * @brief Slices the ByteArray from a specified start index to an end index.
     *
     * This method creates a new ByteArray containing bytes from the specified range.
     * The start index is inclusive, while the end index is exclusive.
     *
     * @param start The starting index from which to begin slicing.
     * @param end The ending index up to which to slice (exclusive).
     * @return ByteArray A new ByteArray containing the sliced segment.
     * @throws std::out_of_range If the start index is greater than the end index or if the end index is out of bounds.
     */
    ByteArray slice(size_t start, size_t end) const {
        if (start > end || end > data_.size()) {
            throw std::out_of_range("Invalid slice range");
        }
        return ByteArray(std::vector<uint8_t>(data_.begin() + start, data_.begin() + end));
    }

    /**
     * @brief Slices the ByteArray from a specified start index to the end of the array.
     *
     * This method creates a new ByteArray containing all bytes from the specified start index to the end of the
     * ByteArray.
     *
     * @param start The starting index from which to begin slicing.
     * @return ByteArray A new ByteArray containing the sliced segment from the start index to the end.
     * @throws std::out_of_range If the start index is out of the bounds of the ByteArray.
     */
    ByteArray slice_from(size_t start) const { return slice(start, data_.size()); }

    /**
     * @brief Slices the ByteArray from the beginning to a specified end index.
     *
     * This method creates a new ByteArray containing all bytes from the beginning of the ByteArray to the specified end
     * index.
     *
     * @param end The ending index up to which to slice (exclusive).
     * @return ByteArray A new ByteArray containing the sliced segment from the beginning to the end index.
     * @throws std::out_of_range If the end index is out of the bounds of the ByteArray.
     */
    ByteArray slice_to(size_t end) const { return slice(0, end); }

    /**
     * @brief Overloaded stream insertion operator for ByteArray.
     * @param os The output stream.
     * @param byteArray The ByteArray object.
     * @return The output stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const ByteArray& byteArray) {
        os << byteArray.toHex(true);
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
};

}  // namespace Common
