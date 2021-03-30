#pragma once

#include <ostream>
#include <memory>
#include <type_traits>
#include <utility>

#include <cstdlib>

#include "memory_resource.h"
#include "standard_memory_resource.h"

namespace takatori::util {

class object_creator;
class object_deleter;

#ifdef ENABLE_OBJECT_CREATOR_PMR
/// @brief whether or not object_creator supports Polymorphic Memory Resource.
static constexpr bool object_creator_pmr_enabled = true;

/**
 * @brief allocator type that compatible with object_creator.
 */
template<class T>
using object_allocator = pmr::polymorphic_allocator<T>;

/**
 * @brief unique_ptr that created with object_creator.
 * @tparam T the value type
 * @see object_creator::create_unique()
 * @see object_creator::wrap_unique()
 */
template<class T>
using unique_object_ptr = std::unique_ptr<T, object_deleter>;
#else
/// @brief whether or not object_creator supports Polymorphic Memory Resource.
static constexpr bool object_creator_pmr_enabled = false;

/**
 * @brief allocator type that compatible with object_creator.
 */
template<class T>
using object_allocator = std::allocator<T>;

/**
 * @brief unique_ptr that created with object_creator.
 * @tparam T the value type
 * @see object_creator::create_unique()
 * @see object_creator::wrap_unique()
 */
template<class T>
using unique_object_ptr = std::unique_ptr<T, std::default_delete<T>>;
#endif // ENABLE_OBJECT_CREATOR_PMR


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
 * @brief creates and deletes objects.
 */
class object_creator {
public:
    /**
     * @brief the corresponded allocator type
     * @tparam T value type
     */
    template<class T>
    using allocator_type = object_allocator<T>;

    /**
     * @brief the corresponded deleter type.
     * @tparam T value type
     */
    template<class T>
    using deleter_type = typename unique_object_ptr<T>::deleter_type;

#ifdef ENABLE_OBJECT_CREATOR_PMR
    /**
     * @brief creates a new instance.
     * @param resource the underlying memory resource
     */
    constexpr object_creator(pmr::memory_resource* resource = get_standard_memory_resource()) noexcept : // NOLINT
        resource_ { resource }
    {}

    /**
     * @brief creates a new instance.
     * @tparam T the allocator value type (ignored)
     * @param allocator the polymorphic allocator
     */
    template<class T>
    object_creator(allocator_type<T> const& allocator) noexcept : // NOLINT
        object_creator {
                allocator.resource(),
        }
    {}
#else
    /**
     * @brief creates a new instance.
     */
    constexpr object_creator() noexcept = default;

    /**
     * @brief creates a new instance.
     * @attention in the current configuration, this will ignore the passed memory resource
     */
    constexpr object_creator(pmr::memory_resource*) noexcept {} // NOLINT

    /**
     * @brief creates a new instance.
     * @tparam T the allocator value type (ignored)
     */
    template<class T>
    constexpr object_creator(allocator_type<T> const&) noexcept {} // NOLINT
#endif // ENABLE_OBJECT_CREATOR_PMR

    /**
     * @brief wraps an object crated by this into std::unique_ptr to delete it.
     * @tparam T the target object type
     * @param object the target object
     * @return the wrapped object
     */
    template<class T>
    [[nodiscard]] unique_object_ptr<T> wrap_unique(T* object);

    /**
     * @brief creates a new object and wrap it into std::unique_ptr.
     * @tparam T the target object type
     * @tparam Args the constructor parameter type
     * @param args the constructor arguments
     * @return the created object
     */
    template<class T, class... Args>
    [[nodiscard]] unique_object_ptr<T> create_unique(Args&&... args) {
        return wrap_unique(create_object<T>(std::forward<Args>(args)...));
    }

    /**
     * @brief wraps an object crated by this into std::shared_ptr to share it.
     * @tparam T the target object type
     * @param object the target object
     * @return the wrapped object
     */
    template<class T>
    [[nodiscard]] std::shared_ptr<T> wrap_shared(T* object);

    /**
     * @brief creates a new object and wrap it into std::shared_ptr.
     * @tparam T the target object type
     * @tparam Args the constructor parameter type
     * @param args the constructor arguments
     * @return the created object
     */
    template<class T, class... Args>
    [[nodiscard]] std::shared_ptr<T> create_shared(Args&&... args) {
        return std::allocate_shared<T, object_allocator<T>>(allocator<T>(), std::forward<Args>(args)...);
    }

    /**
     * @brief returns whether or not the given allocator is compatible with this or an equivalent creator.
     * @tparam Allocator the allocator type
     * @param allocator the allocator
     * @return true if the allocator is compatible with this
     * @return false otherwise
     */
    template<class Allocator>
    [[nodiscard]] bool is_compatible(Allocator const& allocator) const noexcept;

    /**
     * @brief returns whether or not the given std::unique_ptr is created by this or an equivalent creator.
     * @tparam E the target object type
     * @tparam D the deleter type
     * @param ptr the target object
     * @return true if this can safely delete the object
     * @return false otherwise
     */
    template<class E, class D>
    [[nodiscard]] bool is_instance(std::unique_ptr<E, D> const& ptr) const noexcept {
#ifdef ENABLE_OBJECT_CREATOR_PMR
        if constexpr (std::is_same_v<D, std::default_delete<E>> && !std::is_array_v<E>) { // NOLINT
            return resource_->is_equal(*get_standard_memory_resource());
        }
        if constexpr (std::is_same_v<D, object_deleter>) { // NOLINT
            return ptr.get_deleter().creator() == *this;
        }
        return false;
#else
        (void) ptr;
        return std::is_same_v<D, std::default_delete<E>> && !std::is_array_v<E>;
#endif
    }

    /**
     * @brief creates a new object.
     * @tparam T the target object type
     * @tparam Args the constructor parameter type
     * @param args the constructor arguments
     * @return the created object
     */
    template<class T, class... Args>
    [[nodiscard]] T* create_object(Args&&... args) {
        void* pointer = allocate<T>();
        try {
            auto* result = construct<T>(pointer, std::forward<Args>(args)...);
            return result;
        } catch (...) {
            deallocate<T>(pointer);
            std::rethrow_exception(std::current_exception());
        }
    }

    /**
     * @brief deletes the given object which is previously created by this allocator.
     * @tparam T the target object type
     * @param object the object to delete
     */
    template<class T>
    void delete_object(T* object) {
        destroy<T>(object);
        deallocate<T>(object);
    }

    /**
     * @brief construct a new object on the given pointer.
     * @tparam T the target object type
     * @tparam Args the constructor parameter type
     * @param pointer to the object space
     * @param args the constructor arguments
     */
    template<class T, class... Args>
    T* construct(void* pointer, Args&&... args) {
        return new(pointer) T(std::forward<Args>(args)...); // NOLINT(cppcoreguidelines-owning-memory)
    }

    /**
     * @brief destructs the given object.
     * @tparam T the target object type
     * @param object the object to destroy
     */
    template<class T>
    void destroy(T* object) {
        if constexpr (!std::is_trivially_destructible_v<T>) {
            std::destroy_at(object);
        }
        (void) object;
    }

    /**
     * @brief allocates memory space.
     * @param bytes the object size in bytes
     * @param alignment the object alignment
     * @return pointer to the allocated memory
     * @throws std::bad_alloc if failed to reserve memory space
     */
    template<class T>
    [[nodiscard]] void* allocate(std::size_t bytes = sizeof(T), std::size_t alignment = alignof(max_align_t)) {
#ifdef ENABLE_OBJECT_CREATOR_PMR
        return resource_->allocate(bytes, alignment);
#else
        return ::operator new(bytes, static_cast<std::align_val_t>(alignment));
#endif // ENABLE_OBJECT_CREATOR_PMR
    }

    /**
     * @brief releases the memory space previously allocated by this.
     * @param pointer the pointer to memory space
     * @param bytes the object size in bytes
     * @param alignment the object alignment
     */
    template<class T>
    void deallocate(void* pointer, std::size_t bytes = sizeof(T), std::size_t alignment = alignof(max_align_t)) {
#ifdef ENABLE_OBJECT_CREATOR_PMR
        resource_->deallocate(pointer, bytes, alignment);
#else
        (void) bytes;
        (void) alignment;
        ::operator delete(pointer);
#endif // ENABLE_OBJECT_CREATOR_PMR
    }

    /**
     * @brief returns a standard allocator object for this.
     * @tparam T the value type
     * @return the standard allocator
     */
    template<class T = void>
    [[nodiscard]] allocator_type<T> allocator(std::in_place_type_t<T> = std::in_place_type<T>) const noexcept {
#ifdef ENABLE_OBJECT_CREATOR_PMR
        return allocator_type<T>(resource_);
#else
        return {};
#endif // ENABLE_OBJECT_CREATOR_PMR
    }

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
    friend bool operator!=(object_creator a, object_creator b) noexcept;

private:
#ifdef ENABLE_OBJECT_CREATOR_PMR
    pmr::memory_resource* resource_;
#endif // ENABLE_OBJECT_CREATOR_PMR
};

/**
 * @brief deletes objects created by object_allocator.
 */
class object_deleter {
public:
#ifdef ENABLE_OBJECT_CREATOR_PMR
    /**
     * @brief creates a new instance.
     * @param creator the object creator
     */
    constexpr object_deleter(object_creator creator = {}) noexcept // NOLINT
        : creator_(creator)
    {}
#else
    /**
     * @brief creates a new instance.
     */
    constexpr object_deleter(object_creator = {}) {} // NOLINT
#endif // ENABLE_OBJECT_CREATOR_PMR

    /**
     * @brief destroys object and releases the underlying resource of it.
     * @tparam T the object type
     * @param object pointer to the target object
     */
    template<class T>
    void operator()(T* object) {
        // NOTE: shared_ptr may pass nullptr
        if (object != nullptr) {
            creator().delete_object(object);
        }
    }

    /**
     * @brief returns the corresponded creator.
     * @return the corresponded creator
     */
    [[nodiscard]] constexpr object_creator creator() const noexcept { // NOLINT(readability-convert-member-functions-to-static)
#ifdef ENABLE_OBJECT_CREATOR_PMR
        return creator_;
#else
        return {};
#endif // ENABLE_OBJECT_CREATOR_PMR
    }

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
    friend bool operator!=(object_deleter a, object_deleter b) noexcept;

private:
#ifdef ENABLE_OBJECT_CREATOR_PMR
    object_creator creator_;
#endif // ENABLE_OBJECT_CREATOR_PMR
};

/**
 * @brief returns object_creator which use standard new/delete operation.
 * @return the standard object_creator.
 */
object_creator standard_object_creator() noexcept;

template<class T>
inline unique_object_ptr<T> object_creator::wrap_unique(T* object) {
#ifdef ENABLE_OBJECT_CREATOR_PMR
    return unique_object_ptr<T>(object, object_deleter { *this });
#else
    return unique_object_ptr<T>(object);
#endif // ENABLE_OBJECT_CREATOR_PMR
}

template<class T>
inline std::shared_ptr<T> object_creator::wrap_shared(T* object) {
#ifdef ENABLE_OBJECT_CREATOR_PMR
    return std::shared_ptr<T> {
            object,
            object_deleter { *this },
            allocator<T>(),
    };
#else
    return std::shared_ptr<T> { object };
#endif // ENABLE_OBJECT_CREATOR_PMR
}

/**
 * @brief returns object_creator corresponded with the given deleter.
 * @tparam T the deleter element type
 * @param deleter the target deleter
 * @return the related object creator
 */
template<class T = void>
inline constexpr object_creator get_object_creator_from_deleter(object_creator::deleter_type<T> const& deleter) noexcept {
#ifdef ENABLE_OBJECT_CREATOR_PMR
    return deleter.creator();
#else
    (void) deleter;
    return {};
#endif // ENABLE_OBJECT_CREATOR_PMR
}

/// @cond IMPL_DEFS
namespace impl {

template<class Allocator>
struct allocator_compatibility_tester {
    constexpr bool operator()(object_creator, Allocator const&) const noexcept {
        return false;
    }
};

#ifdef ENABLE_OBJECT_CREATOR_PMR
template<class T>
struct allocator_compatibility_tester<std::allocator<T>> {
    bool operator()(object_creator creator, std::allocator<T> const&) const noexcept {
        if constexpr (std::is_array_v<T>) { // NOLINT
            return false;
        }
        return creator == get_standard_memory_resource();
    }
};

template<class T>
struct allocator_compatibility_tester<object_allocator<T>> {
    bool operator()(object_creator creator, object_allocator<T> const& allocator) const noexcept {
        if constexpr (std::is_array_v<T>) { // NOLINT
            return false;
        }
        return creator == object_creator(allocator);
    }
};
#else
template<class T>
struct allocator_compatibility_tester<std::allocator<T>> {
    constexpr bool operator()(object_creator, std::allocator<T> const&) const noexcept {
        return !std::is_array_v<T>;
    }
};

#endif // ENABLE_OBJECT_CREATOR_PMR
} // namespace impl
/// @endcond

template<class Allocator>
inline bool object_creator::is_compatible(Allocator const& allocator) const noexcept {
    return impl::allocator_compatibility_tester<Allocator> {}(*this, allocator);
}

} // namespace takatori::util
