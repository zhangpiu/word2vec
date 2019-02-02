//
// @author qingzhi
// Created on 2/2/19.
//

#ifndef WORD2VEC_MATHUTILS_H
#define WORD2VEC_MATHUTILS_H

#include <numeric>

template<class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type isAlmostEqual(T x, T y, int ulp = 2) {
    // the machine epsilon has to be scaled to the magnitude of the values used
    // and multiplied by the desired precision in ULPs (units in the last place)
    return std::abs(x-y) <= std::numeric_limits<T>::epsilon() * std::abs(x+y) * ulp
           // unless the result is subnormal
           || std::abs(x-y) < std::numeric_limits<T>::min();
}

#endif //WORD2VEC_MATHUTILS_H
