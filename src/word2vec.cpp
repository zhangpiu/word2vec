//
// Created by vodka on 2/2/19.
//

#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "thirdparty/cxxopts.hpp"
#include "StringUtils.h"
#include "Word2Vec.h"


int main(int argc, char* argv[]) {
    std::iostream::sync_with_stdio(false);
    // Arguments parser.
    cxxopts::Options options("word2vec", " - a toy of word2vec.");
    auto args = [&]() {
        options.add_options()
                ("c,corpus", "Input corpus file.", cxxopts::value<std::string>()->default_value("../resources/corpus.txt"))
                ("W,stopwords", "Input stopwords file.", cxxopts::value<std::string>()->default_value("../resources/chinese-stopwords.txt"))
                ("v,vocabulary", "Output vocabulary file", cxxopts::value<std::string>()->default_value("../resources/vocabulary.txt"))
                ("C,context-words", "Output context words file.", cxxopts::value<std::string>()->default_value("../resources/context-words.txt"))
                ("E,embedding", "Embedded vectors file.", cxxopts::value<std::string>()->default_value("../resources/embedding.txt"))
                ("S,similarity", "Result of similarity.", cxxopts::value<std::string>()->default_value("../resources/similarity.txt"))
                ("D,delimiter", "Delimiter among words in corpus file.", cxxopts::value<std::string>()->default_value(" "))
                ("d,dimension", "Dimension of the embedded vector.", cxxopts::value<unsigned>()->default_value("5"))
                ("s,size-context-words", "Size of context words.", cxxopts::value<int>()->default_value("1"))
                ("e,epochs", "Epochs", cxxopts::value<unsigned>()->default_value("10000"))
                ("t,topK", "Top K to recall.", cxxopts::value<unsigned>()->default_value("3"))
                ("h,help", "Print help");
        try {
            return options.parse(argc, argv);
        } catch (const cxxopts::OptionException& e) {
            std::cout << "error parsing options: " << e.what() << std::endl;
            exit(1);
        }
    } ();

    const auto& corpusFile = args["corpus"].as<std::string>();
    const auto& stopwordsFile = args["stopwords"].as<std::string>();
    const auto& vocabularyFile = args["vocabulary"].as<std::string>();
    const auto& contextWordsFile = args["context-words"].as<std::string>();
    const auto& embeddingFile = args["embedding"].as<std::string>();
    const auto& similarityFile = args["similarity"].as<std::string>();
    const auto& delimiter = args["delimiter"].as<std::string>();
    const auto& dimension = args["dimension"].as<unsigned>();
    const auto& contextSize = args["size-context-words"].as<int>();
    const auto& epochs = args["epochs"].as<unsigned>();
    const auto& topK = args["topK"].as<unsigned>();

    std::ifstream fsCorpus(corpusFile);
    if (not fsCorpus.good()) {
        fprintf(stdout, "No such file: %s\n", corpusFile.c_str());
        return -1;
    }
    std::ifstream fsStopwords(stopwordsFile);
    if (not fsStopwords.good()) {
        fprintf(stdout, "No such file: %s\n", stopwordsFile.c_str());
        return -1;
    }
    std::string word;
    std::unordered_set<std::string> stopwords;
    while(std::getline(fsStopwords, word)) {
        stopwords.insert(word);
    }
    fsStopwords.close();

    std::ofstream fsVocabulary(vocabularyFile);
    if (not fsVocabulary.good()) {
        fprintf(stdout, "No such file or directory: %s\n", vocabularyFile.c_str());
        return -1;
    }

    // Parse vocabulary and context words from the input corpus.
    std::vector<std::pair<int, std::vector<std::string>>> data;
    std::unordered_map<std::string, int> vocabulary;
    std::unordered_map<int, std::string> id2vocabulary;
    std::string line;
    int count = 0;
    while(std::getline(fsCorpus, line)) {
        std::vector<std::string> rawResult = splits(line, delimiter);
        std::vector<std::string> result;
        for (const auto& s : rawResult) {
            if (stopwords.count(s)) continue;
            result.emplace_back(s);
        }

        for (int i = 0; i < result.size(); ++i) {
            std::vector<std::string> contextWords;
            for (int di = -contextSize; di <= contextSize; ++di) {
                if (di == 0) continue; // Skip the word itself.
                int idx = i + di;
                if (idx >= 0 and idx < result.size()) {
                    contextWords.emplace_back(result[idx]);
                }
            }

            if (not vocabulary.count(result[i])) {
                fsVocabulary << count << ": " << result[i] << "\n";
                id2vocabulary[count] = result[i];
                vocabulary[result[i]] = count++;
            }

            data.emplace_back(vocabulary[result[i]], contextWords);
        }
    }
    fsCorpus.close();
    fsVocabulary.close();

    std::ofstream fsContextWords(contextWordsFile);
    for (auto& p : data) {
        fsContextWords << p.first << ": ";
        for (size_t i = 0; i < p.second.size(); ++i) {
            if (i) fsContextWords << " ";
            fsContextWords << p.second[i];
        }
        fsContextWords << "\n";
    }
    fsContextWords.close();

    // Train a CBOW model.
    Word2Vec word2Vec(vocabulary.size(), dimension);
    std::unordered_map<int, std::vector<int>> input;
    for (const auto& p : data) {
        std::vector<int> indices;
        for (const auto& s : p.second) {
            indices.push_back(vocabulary[s]);
        }
        input[p.first] = indices;
    }

    word2Vec.train(input, epochs);
    const Matrix& w = word2Vec.getW1();
    std::cout << w;

    std::ofstream fsSimilarity(similarityFile);
    if (not fsSimilarity.good()) {
        fprintf(stdout, "No such file or directory: %s\n", similarityFile.c_str());
        return -1;
    }

    for (size_t i = 0; i < w.rows(); ++i) {
        std::vector<std::pair<int, float>> dist;
        for (size_t j = 0; j < w.cols(); ++j) {
            if (i == j) continue;
            dist.emplace_back(j, (w.row(i) - w.row(j)).squaredNorm());
        }

        std::partial_sort(dist.begin(), dist.begin() + (topK < dist.size() ? topK : dist.size()), dist.end(),
                [](const std::pair<int, float>& x, const std::pair<int, float>& y) {
            return x.second < y.second;
        });
        if (dist.size() > topK) {
            dist.resize(topK);
        }

        fsSimilarity << id2vocabulary[i] << "(" << i << ")\t";
        for (size_t k = 0; k < dist.size(); ++k) {
            if (k) fsSimilarity << " ";
            fsSimilarity << id2vocabulary[dist[k].first] << "(" << dist[k].first << "):" << dist[k].second;
        }
        fsSimilarity << "\n";
    }


    return 0;
}