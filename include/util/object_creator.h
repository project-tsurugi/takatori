#pragma once

#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>

#include <cstdlib>

#include "util/memory_resource.h"
#include "util/standard_memory_resource.h"

namespace util {

class object_creator;
class object_deleter;

/**
 * @brief tests whether or not the target type supports copy construction with object_creator - T(T const&, object_creator).
 * @tparam T the target type
 */
template<class T>
struct is_copy_constructible_with_object_creator : std::is_constructible<T, T const&, object_creator> {};

/**
 * @brief tests whether or not the target type supports copy construction with object_creator - T(T const&, object_creator).
 * @tparam T the target type
 */
template<class T>
struct is_move_constructible_with_object_creator : std::is_constructible<T, T&&, object_creator> {};

/// @copydoc is_copy_constructible_with_object_creator
template<class T>
constexpr inline bool is_copy_constructible_with_object_creator_v = is_copy_constructible_with_object_creator<T>::value;

/// @copydoc is_move_constructible_with_object_creator
template<class T>
constexpr inline bool is_move_constructible_with_object_creator_v = is_move_constructible_with_object_creator<T>::value;

/**
 * @brief unique_ptr that created with object_creator.
 * @tparam T the value type
 * @see object_creator::create_unique()
 * @see object_creator::wrap_unique()
 */
template<class T>
using unique_object_ptr = std::unique_ptr<T, object_deleter>;

/**
 * @brief creates and deletes objects.
 */
class object_creator {
public:
    /**
     * @brief creates a new instance.
     * @param resource the underlying memory resource
     */
    object_creator(pmr::memory_resource* resource = get_standard_memory_resource()) noexcept : resource_(resource) {} // NOLINT

    /**
     * @brief creates a new instance.
     * @tparam T the allocator value type (ignored)
     * @param allocator the polymorphic allocator
     */
    template<class T>
    object_creator(pmr::polymorphic_allocator<T> const& allocator) noexcept : object_creator(allocator.resource()) {} // NOLINT

    /**
     * @brief creates a new object and wrap it into std::unique_ptr.
     * @tparam T the target object type
     * @tparam Args the constructor parameter type
     * @param args the constructor arguments
     * @return the created object
     */
    template<class T, class... Args>
    unique_object_ptr<T> create_unique(Args&&... args);

    /**
     * @brief wraps an object crated by this into std::unique_ptr to delete it.
     * @tparam T the target object type
     * @param object the target object
     * @return the wrapped object
     */
    template<class T>
    unique_object_ptr<T> wrap_unique(T* object);

    /**
     * @brief creates a new object and wrap it into std::shared_ptr.
     * @tparam T the target object type
     * @tparam Args the constructor parameter type
     * @param args the constructor arguments
     * @return the created object
     */
    template<class T, class... Args>
    std::shared_ptr<T> create_shared(Args&&... args);

    /**
     * @brief wraps an object crated by this into std::shared_ptr to share it.
     * @tparam T the target object type
     * @param object the target object
     * @return the wrapped object
     */
    template<class T>
    std::shared_ptr<T> wrap_shared(T* object);

    /**
     * @brief returns whether or not the given allocator is compatible with this or an equivalent creator.
     * @tparam Allocator the allocator type
     * @param allocator the allocator
     * @return true if the allocator is compatible with this
     * @return false otherwise
     */
    template<class Allocator>
    bool is_compatible(Allocator const& allocator) const noexcept;

    /**
     * @brief returns whether or not the given std::unique_ptr is created by this or an equivalent creator.
     * @tparam E the target object type
     * @tparam D the deleter type
     * @param ptr the target object
     * @return true if this can safely delete the object
     * @return false otherwise
     */
    template<class E, class D>
    bool is_instance(std::unique_ptr<E, D> const& ptr) const noexcept;

    /**
     * @brief creates a new object.
     * @tparam T the target object type
     * @tparam Args the constructor parameter type
     * @param args the constructor arguments
     * @return the created object
     */
    template<class T, class... Args>
    T* create_object(Args&&... args);

    /**
     * @brief deletes the given object which is previously created by this allocator.
     * @tparam T the target object type
     * @param object the object to delete
     */
    template<class T>
    void delete_object(T* object);

    /**
     * @brief construct a new object on the given pointer.
     * @tparam T the target object type
     * @tparam Args the constructor parameter type
     * @param pointer to the object space
     * @param args the constructor arguments
     */
    template<class T, class... Args>
    T* construct(void* pointer, Args&&... args);

    /**
     * @brief destructs the given object.
     * @tparam T the target object type
     * @param object the object to destroy
     */
    template<class T>
    void destroy(T* object);

    /**
     * @brief allocates memory space.
     * @param bytes the object size in bytes
     * @param alignment the object alignment
     * @return pointer to the allocated memory
     * @throws std::bad_alloc if failed to reserve memory space
     */
    template<class T>
    void* allocate(std::size_t bytes = sizeof(T), std::size_t alignment = alignof(max_align_t));

    /**
     * @brief releases the memory space previously allocated by this.
     * @param pointer the pointer to memory space
     * @param bytes the object size in bytes
     * @param alignment the object alignment
     */
    template<class T>
    void deallocate(void* pointer, std::size_t bytes = sizeof(T), std::size_t alignment = alignof(max_align_t));

    /**
     * @brief the corresponded allocator type
     * @tparam T value type
     */
    template<class T>
    using allocator_type = pmr::polymorphic_allocator<T>;

    /**
     * @brief returns a standard allocator object for this.
     * @tparam T the value type
     * @return the standard allocator
     */
    template<class T>
    allocator_type<T> allocator(std::in_place_type_t<T> = std::in_place_type<T>) const noexcept { return allocator_type<T>(resource_); }

    /**
     * @brief returns whether or not the two allocators are equivalent.
     * @param a the first allocator
     * @param b the second allocator
     * @return true if the both are equivalent
     * @return false otherwise
     */
    friend bool operator==(object_creator a, object_creator b) noexcept;

    /**
     * @brief returns whether or not the two allocators are different.
     * @param a the first allocator
     * @param b the second allocator
     * @return true if the both are different
     * @return false otherwise
     */
    friend bool operator!=(object_creator a, object_creator b) noexcept { return !(a == b); }

private:
    pmr::memory_resource* resource_;
};

/**
 * @brief deletes objects created by object_allocator.
 */
class object_deleter {
public:
    /**
     * @brief creates a new instance.
     * @param creator the object creator
     */
    object_deleter(object_creator creator = {}) noexcept : creator_(creator) {} // NOLINT

    /**
     * @brief destroys object and releases the underlying resource of it.
     * @tparam T the object type
     * @param object pointer to the target object
     */
    template<class T>
    void operator()(T* object);

    /**
     * @brief returns the corresponded creator.
     * @return the corresponded creator
     */
    object_creator creator() const noexcept { return creator_; }

    /**
     * @brief returns whether or not the two deleter are equivalent.
     * @param a the first deleter
     * @param b the second deleter
     * @return true if the both are equivalent
     * @return false otherwise
     */
    friend bool operator==(object_deleter a, object_deleter b) noexcept;

    /**
     * @brief returns whether or not the two deleter are different.
     * @param a the first deleter
     * @param b the second deleter
     * @return true if the both are different
     * @return false otherwise
     */
    friend bool operator!=(object_deleter a, object_deleter b) noexcept { return !(a == b); }

private:
    object_creator creator_;
};

/**
 * @brief returns object_creator which use standard new/delete operation.
 * @return the standard object_creator.
 */
object_creator standard_object_creator() noexcept;

template<class T, class... Args>
inline T* object_creator::create_object(Args&& ... args) {
    void* pointer = allocate<T>();
    try {
        auto* result = construct<T>(pointer, std::forward<Args>(args)...);
        return result;
    } catch (...) {
        deallocate<T>(pointer);
        std::rethrow_exception(std::current_exception());
    }
}

template<class T>
inline void object_creator::delete_object(T* object) {
    destroy<T>(object);
    deallocate<T>(object);
}

template<class T, class... Args>
inline T* object_creator::construct(void* pointer, Args&&... args) {
    return new(pointer) T(std::forward<Args>(args)...); // NOLINT
}

template<class T>
inline void object_creator::destroy(T* object) {
    if constexpr (!std::is_trivially_destructible_v<T>) {
        object->~T();
    }
    (void) object;
}

template<class T>
inline void* object_creator::allocate(std::size_t bytes, std::size_t alignment) {
    return resource_->allocate(bytes, alignment);
}

template<class T>
inline void object_creator::deallocate(void* pointer, std::size_t bytes, std::size_t alignment) {
    resource_->deallocate(pointer, bytes, alignment);
}

template<class T>
inline void object_deleter::operator()(T* object) {
    creator_.delete_object(object);
}

template<class T>
inline unique_object_ptr<T> object_creator::wrap_unique(T* object) {
    return unique_object_ptr<T>(object, object_deleter { *this });
}

template<class T, class... Args>
inline unique_object_ptr<T> object_creator::create_unique(Args&& ... args) {
    return wrap_unique(create_object<T>(std::forward<Args>(args)...));
}

template<class T, class... Args>
inline std::shared_ptr<T> object_creator::create_shared(Args&& ... args) {
    return wrap_shared(create_object<T>(std::forward<Args>(args)...));
}

template<class T>
inline std::shared_ptr<T> object_creator::wrap_shared(T* object) {
    return std::shared_ptr<T>(object, object_deleter { *this }, allocator(std::in_place_type<T>));
}

/// @private
namespace impl {

/// @private
template<class Allocator>
struct allocator_compatibility_tester {
    bool operator()(object_creator, Allocator const&) const noexcept {
        return false;
    }
};

/// @private
template<class T>
struct allocator_compatibility_tester<std::allocator<T>> {
    bool operator()(object_creator creator, std::allocator<T> const&) const noexcept {
        if constexpr (std::is_array_v<T>) { // NOLINT
            return false;
        }
        return creator == get_standard_memory_resource();
    }
};

/// @private
template<class T>
struct allocator_compatibility_tester<pmr::polymorphic_allocator<T>> {
    bool operator()(object_creator creator, pmr::polymorphic_allocator<T> const& allocator) const noexcept {
        if constexpr (std::is_array_v<T>) { // NOLINT
            return false;
        }
        return creator == object_creator(allocator);
    }
};
} // namespace impl

template<class Allocator>
inline bool object_creator::is_compatible(Allocator const& allocator) const noexcept {
    return impl::allocator_compatibility_tester<Allocator> {}(*this, allocator);
}

template<class E, class D>
inline bool object_creator::is_instance(std::unique_ptr<E, D> const& ptr) const noexcept {
    if constexpr (std::is_same_v<D, std::default_delete<E>> && !std::is_array_v<E>) { // NOLINT
        return resource_->is_equal(*get_standard_memory_resource());
    }
    if constexpr (std::is_same_v<D, object_deleter>) { // NOLINT
        return ptr.get_deleter().creator() == *this;
    }
    return false;
}

} // namespace util
