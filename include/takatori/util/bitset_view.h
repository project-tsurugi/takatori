#pragma once

#include <stdexcept>
#include <type_traits>

#include <cstddef>

#include <boost/dynamic_bitset.hpp>

namespace takatori::util {

/**
 * @brief an unmodifiable bit-set view.
 * @details Individual bit values are packed every 8 units.
 *      Individual bits are arranged from LSB to MSB, from the first octet.
 * @details This does not holds ownership of the memory area.
 */
class bitset_view {
public:
    /// @brief the value type.
    using value_type = bool;

    /// @brief the block type.
    using block_type = char;

    /// @brief the block pointer type.
    using block_pointer = std::add_pointer_t<std::add_const_t<block_type>>;

    /// @brief the size and position type.
    using size_type = std::size_t;

    /**
     * @brief creates a new empty instance.
     */
    constexpr bitset_view() noexcept = default;

    /**
     * @brief creates a new instance.
     * @param block_data the head octet of this view
     * @param bit_count the available number of bits
     */
    constexpr bitset_view(block_pointer block_data, size_type bit_count) noexcept :
            block_data_ { block_data },
            bit_size_ { bit_count }
    {}

    /**
     * @brief returns the available number of bits.
     * @return the available number of bits
     */
    [[nodiscard]] constexpr size_type size() const noexcept {
        return bit_size_;
    }

    /**
     * @brief returns whether or not this bitset capacity is empty.
     * @return true if `size() == 0`
     * @return false if `size() > 0`
     */
    [[nodiscard]] constexpr bool empty() const noexcept {
        return bit_size_ == 0;
    }

    /**
     * @brief returns content at the position on this buffer.
     * @param position the content position
     * @return the buffer content
     * @pre `position < size()`
     * @attention undefined behavior if the `position >= size()`
     */
    [[nodiscard]] constexpr value_type operator[](size_type position) const noexcept {
        auto block = static_cast<unsigned char>(*(block_data_ + to_block_offset(position)));
        return (block & (1U << to_bit_offset(position))) != 0;
    }

    /**
     * @brief returns content at the position on this buffer.
     * @param position the content position
     * @return the buffer content
     * @pre `position < size()`
     * @throw std::out_of_range if `position >= size()`
     */
    [[nodiscard]] constexpr value_type at(size_type position) const {
        if (position >= bit_size_) {
            throw std::out_of_range("buffer position is out of range");
        }
        return operator[](position);
    }

    /**
     * @brief returns the head of underlying buffer.
     * @return the head of underlying buffer
     */
    [[nodiscard]] constexpr block_pointer block_data() const noexcept {
        return block_data_;
    }

    /**
     * @brief returns the number of bytes in this view.
     * @return the number of bytes in this view
     */
    [[nodiscard]] constexpr size_type block_size() const noexcept {
        return to_block_size(bit_size_);
    }

    /**
     * @brief provides boost::dynamic_bitset with equivalent elements.
     * @tparam BlockType the block type
     * @tparam Allocator the block allocator type
     * @return the boost::dynamic_bitset with equivalent elements
     */
    template<class BlockType = std::uint64_t, class Allocator = std::allocator<BlockType>>
    [[nodiscard]] explicit operator ::boost::dynamic_bitset<BlockType, Allocator>() {
        ::boost::dynamic_bitset<BlockType, Allocator> results {};
        results.resize(size());
        for (std::size_t i = 0, n = size(); i < n; ++i) {
            results[i] = operator[](i);
        }
        return results;
    }

private:
    block_pointer block_data_ {};
    size_type bit_size_ {};

    constexpr static size_type to_block_size(size_type bit_size) noexcept {
        return (bit_size + 7) / 8;
    }

    constexpr static size_type to_block_offset(size_type bit_position) noexcept {
        return bit_position / 8;
    }

    constexpr static size_type to_bit_offset(size_type bit_position) noexcept {
        return bit_position % 8;
    }
};

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
std::ostream& operator<<(std::ostream& out, bitset_view value);

} // namespace takatori::util
