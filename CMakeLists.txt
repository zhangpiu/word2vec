cmake_minimum_required(VERSION 3.12)
project(word2vec)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)
add_executable(word2vec src/word2vec.cpp)