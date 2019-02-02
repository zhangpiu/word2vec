//
// Created by vodka on 2/2/19.
//

#ifndef WORD2VEC_STRINGUTILS_H
#define WORD2VEC_STRINGUTILS_H

#include <vector>
#include <cstring>

/**
 * split string using C-string utils. (thread safe)
 * @param str
 * @param separator
 * @return
 */
static std::vector<std::string> splits(const char* str, const char* separator) {
    std::vector<std::string> output;
    if (str == nullptr || separator == nullptr) {
        return output;
    }

    char* dup = strdup(str);
    char* save;
    char* pch = strtok_r(dup, separator, &save);
    while (pch != nullptr) {
        output.emplace_back(std::string(pch));
        pch = strtok_r(nullptr, separator, &save);
    }

    if (dup) {
        free(dup);
    }

    return output;
}

static std::vector<std::string> splits(const std::string& str, const std::string& delimiters = ",") {
    return splits(str.c_str(), delimiters.c_str());
}

#endif //WORD2VEC_STRINGUTILS_H
