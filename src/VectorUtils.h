//
// @author qingzhi
// Created on 2/2/19.
//

#ifndef WORD2VEC_VECTORUTILS_H
#define WORD2VEC_VECTORUTILS_H

#include <vector>
#include <cassert>

template <typename T>
std::vector<T> operator + (const std::vector<T>& lhs, const std::vector<T>& rhs) {
    assert(lhs.size() == rhs.size());
    std::vector<T> result(lhs.size());
    for (size_t i = 0; i < lhs.size(); ++i) {
        result[i] = lhs[i] + rhs[i];
    }

    return result;
}

#endif //WORD2VEC_VECTORUTILS_H
