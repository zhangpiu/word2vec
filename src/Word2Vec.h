//
// @author qingzhi
// Created on 2/2/19.
//

#ifndef WORD2VEC_WORD2VEC_H
#define WORD2VEC_WORD2VEC_H

#include <vector>
#include <random>
#include <unordered_map>
#include <algorithm>
#include "functions.h"
#include "Matrix.h"
#include "VectorUtils.h"


class Word2Vec {
public:
    Word2Vec(size_t vocabularySize, size_t embeddingSize)
        : vocabularySize(vocabularySize)
        , embeddingSize(embeddingSize) {}

    /**
     * Train a CBOW model.
     * @param data
     */
    void train(const std::unordered_map<int, std::vector<int>>& data) {
        initWeights();

        for(const auto& kv : data) {
            float loss = forward(kv.second, kv.first);
            std::cout << "Loss: " << loss << std::endl;
        }
    }

    float forward(const std::vector<int>& input, int target) {
        Matrix hidden(embeddingSize, 1);
        for (int idx : input) {
            for (size_t i = 0; i < hidden.rows(); ++i) {
                hidden(i, 0) += W1.row(idx)[i];
            }
        }
        hidden /= input.size();

        Matrix u = W2.T() * hidden;
        Matrix y = softmax(u);
        float loss = -log(y(target, 0));
        return loss;
    }

    void backward() {

    }

    void initWeights() {
        std::mt19937 engine{std::random_device()()};
        std::uniform_real_distribution<float> dist{0, 1};
        auto generator = [&]() { return dist(engine); };

        W1.resize(vocabularySize, embeddingSize);
        W2.resize(embeddingSize, vocabularySize);
        for (size_t i = 0; i < W1.rows(); ++i) {
            std::generate(W1.row(i).data().begin(), W1.row(i).data().end(), generator);
        }
        for (size_t i = 0; i < W2.rows(); ++i) {
            std::generate(W2.row(i).data().begin(), W2.row(i).data().end(), generator);
        }
    }

    const Matrix& getW1() const {
        return W1;
    }

    const Matrix& getW2() const {
        return W2;
    }


private:
    size_t vocabularySize;

    size_t embeddingSize;

    Matrix W1;

    Matrix W2;
};

#endif //WORD2VEC_WORD2VEC_H
