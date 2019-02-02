//
// Created by vodka on 2/2/19.
//

#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "src/StringUtils.h"
#include "src/Word2Vec.h"


int main(int argc, char* argv[]) {
    std::ifstream file("../resources/corpus.txt");
    if (not file.good()) {
        return -1;
    }

    int skipgram = 1;
    std::unordered_map<std::string, int> corpus;
    std::string line;
    int count = 0;
    std::unordered_map<int, std::vector<std::string>> data;
    while(std::getline(file, line)) {
        std::vector<std::string> result = splits(line, " ");
        for (int i = 0; i < result.size(); ++i) {
            std::vector<std::string> sample;
            for (int di = -skipgram; di <= skipgram; ++di) {
                if (di == 0) continue; // Skip the word itself.

                int idx = i + di;
                if (idx >= 0 and idx < result.size()) {
                    sample.emplace_back(result[idx]);
                }
            }
            data[count] = sample;
            if (not corpus.count(result[i])) {
                corpus[result[i]] = count++;
            }
        }
    }

    for (auto& kv : corpus) {
        std::cout << kv.first << " " << kv.second << std::endl;
    }

    for (auto& kv : data) {
        std::cout << kv.first << ": ";
        for (auto& item : kv.second) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }

    Word2Vec word2Vec(corpus.size(), 3);
    word2Vec.initWeights();

    Matrix w1 = word2Vec.getW1();
    for (size_t i = 0; i < w1.rows(); ++i) {
        for (size_t j = 0; j < w1.cols(); ++j) {
            std::cout << w1(i, j) << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
    Matrix w2 = word2Vec.getW2();
    for (size_t i = 0; i < w2.rows(); ++i) {
        for (size_t j = 0; j < w2.cols(); ++j) {
            std::cout << w2(i, j) << " ";
        }
        std::cout << std::endl;
    }

    std::unordered_map<int, std::vector<int>> input;
    for (const auto& kv : data) {
        std::vector<int> indices;
        for (const auto& s : kv.second) {
            indices.push_back(corpus[s]);
        }
        input[kv.first] = indices;
    }

    word2Vec.train(input);

    std::cout << word2Vec.getW1();

    return 0;
}