#pragma once

#include <type_traits>
#include <array>

namespace rterm {

template<typename T, size_t Dim>
class Vector {
public:
    explicit Vector(const std::initializer_list<T>& initList)
        : data_(initList) {
    }

    template<typename ...Args>
    Vector(Args... args);

    inline T& operator[](size_t i) {
        return data_[i];
    }

    inline const T& operator[](size_t i) const {
        return data_[i];
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
    
    explicit Rect(Vector<T, 2> pos, Vector<T, 2> size)
        : pos_(pos)
        , size_(size) {
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

using iRect = Rect<int>;
using fRect = Rect<float>;
using dRect = Rect<double>;

} // namespace rterm
