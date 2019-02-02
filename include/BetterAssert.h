//
// @author qingzhi
// Created on 1/23/19.
//

#ifndef WORD2VEC_BETTERASSERT_H
#define WORD2VEC_BETTERASSERT_H

#include <cstdio>
#include "BetterException.h"

/**
 * Assertions
 */
#define BETTER_ASSERT(X)                                              \
    do {                                                              \
        if (! (X)) {                                                  \
            fprintf(stderr, "Assertion '%s' failed in %s at %s:%d\n", \
                    #X, __PRETTY_FUNCTION__, __FILE__, __LINE__);     \
            std::abort();                                             \
        }                                                             \
    } while (false)

#define BETTER_ASSERT_MSG(X, MSG)                                     \
    do {                                                              \
        if (! (X)) {                                                  \
            fprintf(stderr, "Assertion '%s' failed in %s "            \
                     "at %s:%d; details: " MSG "\n",                  \
                    #X, __PRETTY_FUNCTION__, __FILE__, __LINE__);     \
            std::abort();                                             \
        }                                                             \
     } while (false)

#define BETTER_ASSERT_FMT(X, FMT, ...)                                \
    do {                                                              \
        if (! (X)) {                                                  \
            fprintf(stderr, "Assertion '%s' failed in %s "            \
               "at %s:%d; details: " FMT "\n",                        \
               #X, __PRETTY_FUNCTION__, __FILE__, __LINE__, __VA_ARGS__); \
            std::abort();                                             \
        }                                                             \
    } while (false)


/**
 * Exceptions for returning user errors
 */
#define BETTER_THROW_MSG(MSG)                                                       \
    do {                                                                            \
        throw camel::BetterException(MSG, __PRETTY_FUNCTION__, __FILE__, __LINE__); \
    } while (false)

#define BETTER_THROW_FMT(FMT, ...)                                                  \
    do {                                                                            \
        std::string __s;                                                            \
        int __size = snprintf(nullptr, 0, FMT, __VA_ARGS__);                       \
        __s.resize(__size + 1);                                                     \
        snprintf(&__s[0], __s.size(), FMT, __VA_ARGS__);                            \
        throw camel::BetterException(__s, __PRETTY_FUNCTION__, __FILE__, __LINE__); \
    } while (false)

/**
 * Exceptions thrown upon a conditional failure
 */
#define BETTER_THROW_IF_NOT(X)                           \
    do {                                                 \
        if (!(X)) {                                      \
            BETTER_THROW_FMT("Error: '%s' failed", #X);  \
        }                                                \
     } while (false)

#define BETTER_THROW_IF_NOT_MSG(X, MSG)                  \
    do {                                                 \
        if (!(X)) {                                      \
            BETTER_THROW_FMT("Error: '%s' failed: " MSG, #X); \
        }                                                 \
    } while (false)

#define BETTER_THROW_IF_NOT_FMT(X, FMT, ...)                               \
    do {                                                                   \
        if (!(X)) {                                                        \
            BETTER_THROW_FMT("Error: '%s' failed: " FMT, #X, __VA_ARGS__); \
        }                                                                  \
     } while (false)

#endif //WORD2VEC_BETTERASSERT_H
