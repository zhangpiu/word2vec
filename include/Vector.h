//
// @author qingzhi
// Created on 2/2/19.
//

#ifndef WORD2VEC_VECTOR_H
#define WORD2VEC_VECTOR_H

#include <vector>
#include <cstdio>
#include <cassert>


class Vector {
public:
    // Prevent heap allocation
    void* operator new  (size_t)   = delete;

    void* operator new[](size_t)   = delete;

    void  operator delete(void*)   = delete;

    void  operator delete[](void*) = delete;

    Vector() {}

    Vector(size_t length, float value = 0.0f) {
        _data.resize(length, value);
    }

    const float& operator[] (size_t i) const {
        return _data[i];
    }

    float& operator[] (size_t i) {
        return _data[i];
    }

    Vector& operator += (const Vector& rhs) {
        assert(this->length() == rhs.length());
        for (size_t i = 0; i < _data.size(); ++i) {
            _data[i] += rhs._data[i];
        }
    }

    Vector operator + (const Vector& rhs) {
        Vector result(rhs.length());
        result += rhs;
        return result;
    }

    Vector& operator -= (const Vector& rhs) {
        assert(this->length() == rhs.length());
        for (size_t i = 0; i < _data.size(); ++i) {
            _data[i] -= rhs._data[i];
        }
    }

    Vector operator - (const Vector& rhs) const {
        Vector result(*this);
        result -= rhs;
        return result;
    }

    Vector& operator /= (float value) {
        for (size_t i = 0; i < _data.size(); ++i) {
            _data[i] /= value;
        }

        return *this;
    }

    Vector& operator *= (float value) {
        for (size_t i = 0; i < _data.size(); ++i) {
            _data[i] *= value;
        }

        return *this;
    }

    float squaredNorm() const {
        float value = 0.0f;
        for (size_t i = 0; i < _data.size(); ++i) {
            value += _data[i] * _data[i];
        }
        return value;
    }

    size_t length() const {
        return _data.size();
    }

    void resize(size_t length) {
        _data.resize(length);
    }

    const std::vector<float>& data() const {
        return _data;
    }

    std::vector<float>& data() {
        return _data;
    }

private:
    std::vector<float> _data;
};

#endif //WORD2VEC_VECTOR_H
