//
// @author qingzhi
// Created on 1/23/19.
//

#ifndef WORD2VEC_BETTEREXCEPTION_H
#define WORD2VEC_BETTEREXCEPTION_H

#include <string>
#include <exception>

namespace camel {
/// Base class for exceptions
class BetterException : public std::exception {
public:
    explicit BetterException(const std::string& msg): msg(msg) {}

    BetterException(const std::string& msg,
                   const char* funcName,
                   const char* file,
                   int line) {
        int size = snprintf(nullptr, 0, "Error in %s at %s:%d: %s",
                            funcName, file, line, msg.c_str());
        this->msg.resize(size + 1);
        snprintf(&this->msg[0], this->msg.size(), "Error in %s at %s:%d: %s",
                 funcName, file, line, msg.c_str());
    }

    /// from std::exception
    const char* what() const noexcept override {
        return this->msg.c_str();
    }

private:
    std::string msg;
};
}

#endif //WORD2VEC_BETTEREXCEPTION_H
