//
// @author qingzhi
// Created on 2/2/19.
//

#ifndef WORD2VEC_MATRIX_H
#define WORD2VEC_MATRIX_H

#include <vector>
#include <iostream>
#include "Vector.h"
#include "BetterAssert.h"

class Matrix {
public:
    // Prevent heap allocation
    void* operator new  (size_t)   = delete;

    void* operator new[](size_t)   = delete;

    void  operator delete(void*)   = delete;

    void  operator delete[](void*) = delete;

    Matrix() {}

    Matrix(size_t rows, size_t cols) {
        data.resize(rows, Vector(cols));
    }

    const float& operator()(size_t i, size_t j) const {
        return data[i][j];
    }

    float& operator()(size_t i, size_t j) {
        return data[i][j];
    }

    Matrix& operator += (const Matrix& rhs) {
        BETTER_ASSERT_FMT(this->rows() == rhs.rows() and this->cols() == rhs.cols()
        , "lhs' size(%zd, %zd) doesn't match rhs' size(%zd, %zd)", this->rows(), this->cols(), rhs.rows(), rhs.cols());
        for (size_t i = 0; i < this->rows(); ++i) {
            data[i] += rhs.data[i];
        }
        return *this;
    }

    Matrix operator + (const Matrix& rhs) const {
        Matrix result(rhs.rows(), rhs.cols());
        result += rhs;
        return result;
    }

    Matrix& operator -= (const Matrix& rhs) {
        BETTER_ASSERT_FMT(this->rows() == rhs.rows() and this->cols() == rhs.cols()
                , "lhs' size(%zd, %zd) doesn't match rhs' size(%zd, %zd)", this->rows(), this->cols(), rhs.rows(), rhs.cols());
        for (size_t i = 0; i < this->rows(); ++i) {
            data[i] -= rhs.data[i];
        }
        return *this;
    }

    Matrix operator - (const Matrix& rhs) const {
        Matrix result(rhs.rows(), rhs.cols());
        result -= rhs;
        return result;
    }

    Matrix& operator /= (float value) {
        for (size_t i = 0; i < data.size(); ++i) {
            data[i] /= value;
        }
    }

    Matrix& operator *= (float value) {
        for (size_t i = 0; i < data.size(); ++i) {
            data[i] *= value;
        }
    }

    Matrix operator * (float value) {
        Matrix result(*this);
        result *= value;
        return result;
    }

    Matrix operator * (const Matrix& rhs) const {
        BETTER_ASSERT_FMT(this->cols() == rhs.rows(), "lhs' cols(%zd) should be equal to rhs' rows(%zd)", this->cols(), rhs.rows());
        Matrix result(this->rows(), rhs.cols());
        for (size_t i = 0; i < result.rows(); ++i) {
            for (size_t j = 0; j < result.cols(); ++j) {
                for (size_t k = 0; k < this->cols(); ++k) {
                    result(i, j) += data[i][k] * rhs.data[k][j];
                }
            }
        }

        return result;
    }

    Matrix T() const {
        Matrix t(this->cols(), this->rows());
        for (size_t i = 0; i < t.rows(); ++i) {
            for (size_t j = 0; j < t.cols(); ++j) {
                t(i, j) = data[j][i];
            }
        }
        return t;
    }

    const Vector& row(size_t i) const {
        return data[i];
    }

    Vector& row(size_t i) {
        return data[i];
    }

    size_t rows() const {
        return data.size();
    }

    size_t cols() const {
        return data.front().length();
    }

    void resize(size_t rows, size_t cols) {
        data.resize(rows);
        for (auto& row : data) {
            row.resize(cols);
        }
    }

    friend inline std::ostream& operator << (std::ostream& os, const Matrix& m) {
        os << "[";
        for(size_t i = 0; i < m.rows(); ++i) {
            if (i) os << " ";
            os << "[";
            for (size_t j = 0; j < m.cols(); ++j) {
                if (j) os << ",";
                os << m.data[i][j];
            }
            os << "]";
            if (i + 1 < m.rows()) os << "\n";
        }
        os << "]\n";
    }

private:
    std::vector<Vector> data;
};

#endif //WORD2VEC_MATRIX_H
