#pragma once

#include <rterm/util/defs.h>
#include <rterm/util/exception.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <type_traits>

namespace rterm {

template<typename T, size_t Dim>
class Vector {
public:
    Vector() {
        data_.fill(0);
    }

    Vector(std::initializer_list<T> initList) {
        /* initializer_list<T>::size() is not constexpr */
        if (initList.size() > Dim) {
            throw Exception("Invalid vector size");
        }
        std::copy(initList.begin(), initList.end(), data_.begin());
    }

    template<typename U>
    Vector(const Vector<U, Dim>& other) {
        operator=(other);
    }

    template<typename U>
    Vector(Vector<U, Dim>&& other) {
        operator=(std::move(other));
    }

    template<typename U>
    Vector& operator=(const Vector<U, Dim>& other) {
        std::copy(other.data_.begin(), other.data_.end(), data_.begin());
        return *this;
    }

    template<typename U>
    Vector& operator=(Vector<U, Dim>&& other) {
        data_.swap(other.data_);
        return *this;
    }

    template<typename U>
    bool operator==(const Vector<U, Dim>& other) const {
        return data_ == other.data_;
    }

    template<typename U>
    bool operator!=(const Vector<U, Dim>& other) const {
        return !operator==(other);
    }

    inline T& operator[](size_t i) {
        return data_[i];
    }

    inline const T& operator[](size_t i) const {
        return data_[i];
    }

    inline operator bool() const {
        bool res = false;
        for (auto&& t : data_) {
            res = res || t;
        }
        return res;
    }

    T norm() const {
        T result{0};
        for (auto&& t : data_) {
            result += t * t;
        }
        return result;
    }

    T abs() const {
        return std::sqrt(norm());
    }

private:
    std::array<T, Dim> data_;
};

template<typename T>
class Rect {
public:
    explicit Rect(T x = T(), T y = T(), T w = T(), T h = T())
        : Rect({x, y}, {w, h}) {
    }

    Rect(Vector<T, 2> pos, Vector<T, 2> size)
        : pos_(pos)
        , size_(size) {
    }

    template<typename U>
    bool operator==(const Rect<U>& other) const {
        return pos_ == other.pos_ && size_ == other.size_;
    }

    template<typename U>
    bool operator!=(const Rect<U>& other) const {
        return !operator==(other);
    }

    inline T& x() {
        return pos_[0];
    }

    inline T& y() {
        return pos_[1];
    }

    inline T& w() {
        return size_[0];
    }

    inline T& h() {
        return size_[1];
    }

    inline const T& x() const {
        return pos_[0];
    }

    inline const T& y() const {
        return pos_[1];
    }

    inline const T& w() const {
        return size_[0];
    }

    inline const T& h() const {
        return size_[1];
    }

private:
    Vector<T, 2> pos_;
    Vector<T, 2> size_;
};

template<size_t Dim>
using vec = Vector<f32, Dim>;
using vec2 = vec<2>;
using vec3 = vec<3>;
using vec4 = vec<4>;

template<size_t Dim>
using ivec = Vector<i32, Dim>;
using ivec2 = ivec<2>;
using ivec3 = ivec<3>;
using ivec4 = ivec<4>;

template<size_t Dim>
using uvec = Vector<u32, Dim>;
using uvec2 = uvec<2>;
using uvec3 = uvec<3>;
using uvec4 = uvec<4>;

using fSize = vec2;
using iSize = ivec2;
using uSize = uvec2;

using iRect = Rect<i32>;
using uRect = Rect<u32>;
using fRect = Rect<float>;

} // namespace rterm
