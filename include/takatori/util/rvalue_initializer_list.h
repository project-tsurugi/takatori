#pragma once

#include <initializer_list>
#include <iterator>
#include <vector>

#include "rvalue_reference_wrapper.h"
#include "reference_iterator.h"

namespace takatori::util {

/**
 * @brief a wrapper of std::initializer_list of rvalue_reference_wrapper.
 * @note This is for avoiding overload ambiguity.
 * @tparam T the element type
 */
template<class T>
class rvalue_initializer_list {
public:
    /// @brief the value type
    using value_type = T;
    /// @brief the size type
    using size_type = std::size_t;
    /// @brief the reference type
    using reference = std::add_rvalue_reference_t<value_type>;
    /// @brief the pointer type
    using pointer = std::add_pointer_t<value_type>;

    /// @brief the raw iterator type
    using raw_iterator = reference_iterator<double_pointer_extractor<value_type>>;
    /// @brief the raw const iterator type
    using raw_const_iterator = reference_iterator<double_pointer_extractor<std::add_const_t<value_type>>>;

    /// @brief the move iterator type
    using iterator = std::move_iterator<raw_iterator>;
    /// @brief the move const iterator type
    using const_iterator = std::move_iterator<raw_const_iterator>;

    /**
     * @brief constructs a new object.
     * @tparam Elements the element types
     * @param elements the elements
     */
    rvalue_initializer_list(std::initializer_list<rvalue_reference_wrapper<T>> elements) noexcept { // NOLINT
        elements_.reserve(elements.size());
        for (auto&& w : elements) {
            auto&& e = w.get();
            elements_.emplace_back(std::addressof(e));
        }
    }

    /**
     * @brief returns the number of elements in this.
     * @return the number of elements
     */
    [[nodiscard]] constexpr size_type size() const noexcept { return elements_.size(); }

    /**
     * @brief returns a forward iterator which points the beginning of this container.
     * @return the iterator of beginning (inclusive)
     */
    [[nodiscard]] iterator begin() noexcept { return std::make_move_iterator(raw_iterator(elements_.data())); }

    /// @copydoc begin()
    [[nodiscard]] const_iterator begin() const noexcept { return std::make_move_iterator(raw_const_iterator(elements_.data())); }

    /**
     * @brief returns a forward iterator which points the ending of this container.
     * @return the iterator of ending (exclusive)
     */
    [[nodiscard]] iterator end() noexcept { return std::make_move_iterator(raw_iterator(elements_.data() + elements_.size())); } // NOLINT

    /// @copydoc end()
    [[nodiscard]] const_iterator end() const noexcept { return std::make_move_iterator(raw_const_iterator(elements_.data() + elements_.size())); } // NOLINT

    /**
     * @brief returns a backward iterator which points the reversed beginning of this container.
     * @return the reversed iterator of beginning (inclusive)
     */
    [[nodiscard]] std::reverse_iterator<iterator> rbegin() noexcept { return std::make_reverse_iterator(end()); }

    /// @copydoc rbegin()
    [[nodiscard]] std::reverse_iterator<const_iterator> rbegin() const noexcept { return std::make_reverse_iterator(end()); }

    /**
     * @brief returns a backward iterator which points the reversed ending of this container.
     * @return the reversed iterator of ending (exclusive)
     */
    [[nodiscard]] std::reverse_iterator<iterator> rend() noexcept { return std::make_reverse_iterator(begin()); }

    /// @copydoc rend()
    [[nodiscard]] std::reverse_iterator<const_iterator> rend() const noexcept { return std::make_reverse_iterator(begin()); }

private:
    std::vector<pointer> elements_;

    using extractor_type = double_pointer_extractor<value_type>;
};

} // namespace takatori::util
