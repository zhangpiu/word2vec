//
// Created by vodka on 2/2/19.
//

#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include "src/StringUtils.h"

int main(int argc, char* argv[]) {
    std::ifstream file("../resources/corpus.txt");
    if (not file.good()) {
        return -1;
    }

    std::string line;
    while(std::getline(file, line)) {
        std::vector<std::string> result = splits(line, " ");

        for (auto& s : result) {
            std::cout << s << " ";
        }
        std::cout << "\n";
    }



    return 0;
}