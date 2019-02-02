//
// @author qingzhi
// Created on 2/2/19.
//

#ifndef WORD2VEC_SOFTMAX_H
#define WORD2VEC_SOFTMAX_H

#include <cmath>
#include <vector>
#include "MathUtils.h"
#include "Matrix.h"


static Matrix softmax(const Matrix& input) {
    assert(input.rows() == 1 or input.cols() == 1);
    Matrix output(input.rows(), input.cols());
    float sum = 0.0;
    for (size_t i = 0; i < input.rows(); ++i) {
        for (size_t j = 0; j < input.cols(); ++j) {
            output(i, j) = std::exp(input(i, j));
            sum += output(i, j);
        }
    }

    if (not isAlmostEqual(sum, 0.0f)) {
        for (size_t i = 0; i < input.rows(); ++i) {
            output.row(i) /= sum;
        }
    }

    return output;
}


static Matrix onehot(size_t hot, size_t dimension) {
    Matrix result(dimension, 1);
    result(hot, 0) = 1.0f;
    return result;
}

#endif //WORD2VEC_SOFTMAX_H
