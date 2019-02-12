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
    void train(const std::unordered_map<int, std::vector<int>>& data, unsigned epochs, float lr) {
        initWeights();

        for (int i = 0; i < epochs; ++i) {
            int count = 0;
            for(const auto& kv : data) {
                float loss = forwardAndBackward(kv.second, kv.first, lr);
                fprintf(stdout, "Epoch: [%d][%d/%zd]\tLoss: %.6f\n", i, count++, data.size(), loss);
            }
        }
    }

    float forwardAndBackward(const std::vector<int>& input, size_t target, float lr) {
        // 1. Forward
        Matrix hidden(embeddingSize, 1);
        for (int idx : input) {
            for (size_t i = 0; i < hidden.rows(); ++i) {
                hidden(i, 0) += W1.row(idx)[i];
            }
        }
        hidden /= input.size();

        Matrix u = W2.T() * hidden; // (N by V)^T * (N by 1) -> (V by 1)
        Matrix y = softmax(u);
        float loss = -std::log(y(target, 0));

        // 2. Backward
        Matrix delta2 = y; // V by 1
        delta2(target, 0) -= 1.0f; // delta2 = o_t - y_t

        Matrix w2Grad = hidden * delta2.T(); // N by V
        Matrix delta1 = (W2 * delta2); // N by 1;
        Matrix w1Grad(W1.rows(), W1.cols());
        Matrix x(vocabularySize, 1);
        for (int hot : input) {
            x += onehot(vocabularySize, hot);
        }
        x *= 1. / input.size();
        w1Grad += (x * delta1.T());

        w1Grad *= lr; W1 -= w1Grad;
        w2Grad *= lr; W2 -= w2Grad;

        return loss;
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

private:
    size_t vocabularySize;

    size_t embeddingSize;

    Matrix W1;

    Matrix W2;
};

#endif //WORD2VEC_WORD2VEC_H
