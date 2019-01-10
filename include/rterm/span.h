#pragma once

#include <algorithm>
#include <array>
#include <iterator>

#if __cplusplus > 201703L
#include <span>
#endif

namespace rterm {

#if __cplusplus > 201703L
using span = std::span;
#else

namespace detail {
#if __cplusplus >= 201703L
using std::data;
using std::size;
#else
/* based on http://eel.is/c++draft/iterator.container */
template<class C>
constexpr auto data(C& c) -> decltype(c.data()) {
    return c.data();
}

template<class C>
constexpr auto data(const C& c) -> decltype(c.data()) {
    return c.data();
}

template<class T, std::size_t N>
constexpr T* data(T (&array)[N]) noexcept {
    return array;
}

template<class E>
constexpr const E* data(std::initializer_list<E> il) noexcept {
    return il.begin();
}

template <class C>
constexpr auto size(const C& c) -> decltype(c.size()) {
    return c.size();
}

template <class T, std::size_t N>
constexpr std::size_t size(const T (&)[N]) noexcept {
    return N;
}
#endif
}

/* based on http://eel.is/c++draft/views.span */
template<typename T>
class span {
public:
    using element_type = T;
    using value_type = std::remove_cv_t<T>;
    using index_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;
    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = typename std::reverse_iterator<iterator>;
    using const_reverse_iterator = typename std::reverse_iterator<const_iterator>;

public:
    /* ctors */
    constexpr span() noexcept
        : begin_(nullptr)
        , end_(nullptr)
    {}

    constexpr span(pointer ptr, index_type count) noexcept
        : span(ptr, ptr + count)
    {}

    constexpr span(pointer first, pointer last) noexcept
        : begin_(first)
        , end_(last)
    {}

    template<std::size_t N>
    constexpr span(element_type (&arr)[N]) noexcept
        : span(arr, N)
    {}

    template<std::size_t N>
    constexpr span(std::array<value_type, N>& arr) noexcept
        : span(arr.data(), N)
    {}

    template<std::size_t N>
    constexpr span(const std::array<value_type, N>& arr) noexcept
        : span(arr.data(), N)
    {}

    template<typename Containter>
    constexpr span(Containter& cont)
        : span(detail::data(cont), detail::size(cont))
    {}

    template<typename Containter>
    constexpr span(const Containter& cont)
        : span(detail::data(cont), detail::size(cont))
    {}

    template<class U>
    constexpr span(span<U> other) noexcept
        : span(other.data(), other.size())
    {}

    constexpr span(const span<T>& other) = default;

    /* operator= */
    constexpr span& operator=(const span<T>& other) {
        begin_ = other.begin_;
        end_ = other.end_;
        return *this;
    }

    /* iterators */
    constexpr iterator begin() noexcept {
        return begin_;
    }

    constexpr const_iterator begin() const noexcept {
        return begin_;
    }

    constexpr const_iterator cbegin() const noexcept {
        return begin_;
    }

    constexpr iterator end() noexcept {
        return end_;
    }

    constexpr const_iterator end() const noexcept {
        return end_;
    }

    constexpr const_iterator cend() const noexcept {
        return end_;
    }

    constexpr reverse_iterator rbegin() noexcept {
        return begin_;
    }

    constexpr const_reverse_iterator rbegin() const noexcept {
        return begin_;
    }

    constexpr const_reverse_iterator crbegin() const noexcept {
        return begin_;
    }

    constexpr reverse_iterator rend() noexcept {
        return end_;
    }

    constexpr const_reverse_iterator rend() const noexcept {
        return end_;
    }

    constexpr const_reverse_iterator crend() const noexcept {
        return end_;
    }

    /* element access */
    constexpr reference operator[](index_type idx) const {
        return begin_[idx];
    }

    constexpr reference operator()(index_type idx) const {
        return begin_[idx];
    }

    constexpr pointer data() const noexcept {
        return begin_;
    }

    /* observers */
    constexpr index_type size() const noexcept {
        return end_ - begin_;
    }

    constexpr index_type size_bytes() const noexcept {
        return size() * sizeof(T);
    }

    constexpr bool empty() const noexcept {
        return size() == 0;
    }

    /* subviews */
    constexpr span<element_type> first(std::ptrdiff_t count) const {
        return span(begin_, count);
    }

    constexpr span<element_type> last(std::ptrdiff_t count) const {
        return span(end_ - count, count);
    }

    constexpr span<element_type> subspan(
        std::ptrdiff_t offset,
        std::ptrdiff_t count
    ) const {
        return span(begin_ + offset, count);
    }

private:
    pointer begin_;
    pointer end_;
};

template<class T, class U>
constexpr bool operator==(const span<T> lhs, const span<U> rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<class T, class U>
constexpr bool operator!=(const span<T> lhs, const span<U> rhs) {
    return !(lhs == rhs);
}

#endif

} // namespace rterm
