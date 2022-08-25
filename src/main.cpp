#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>

#include "string.h"

using DictMap = std::unordered_map<std::string, size_t>;
using DictVector = std::vector<std::pair<std::string, size_t>>;

void parseAndFill(const char *data, DictMap &map)
{
    size_t dataLen = strlen(data);
    std::string currentWord{};
    bool currentAlpha = false;

    for (int i = 0; i < dataLen; i++) {
        currentAlpha = isalpha(data[i]);
        if (currentAlpha) {
            currentWord += std::tolower(data[i]);
        } 
        if((!currentAlpha && !currentWord.empty()) ||
           ( currentAlpha && i == (dataLen - 1))) {
            if(map.find(currentWord) != map.end()) {
                map[currentWord]++;
            } else {
                map[currentWord] = 1u;
            }

            currentWord.clear();
        }
    }
}

bool readFile(const char *filename, DictVector &words)
{
    FILE *fp = fopen(filename, "r");

    if (!fp) {
        return false;
    }

    DictMap dictMap{};
    char *lineData{nullptr};
    size_t len{0};

    while ((getline(&lineData, &len, fp)) != -1) {
        parseAndFill(lineData, dictMap);
    }

    fclose(fp);
    if (lineData) {
        free(lineData);
    }

    if (!dictMap.empty()) {
        words.reserve(dictMap.size());
        for (const auto &entry : dictMap) {
            words.emplace_back(entry.first, entry.second);
        }
    }

    return true;
}

bool writeToFile(const char * file, DictVector & vector) {
    std::ofstream stream(file);

    if(!stream) {
        return false;
    }

    for(auto entry : vector) {
        stream << entry.second << " " << entry.first << "\n";
    }

    return true;
}

int main(int argc, const char **argv)
{
    if (argc != 3) {
        std::cout << "Wrong usage. Give file names of input and output files" << std::endl;
        return EXIT_FAILURE;
    }

    DictVector words;
    if (!readFile(argv[1], words)) {
        std::cout << "'" << argv[1] << "' can't be read" << std::endl;
        return EXIT_FAILURE;
    }

    if (words.size() == 0) {
        std::cout << "'" << argv[1] << "' contains no words" << std::endl;
        return EXIT_SUCCESS;
    }

    std::sort(words.begin(), words.end(), [](auto a, auto b) {
        if(a.second > b.second) {
            return true;
        } else if(a.second == b.second && a.first < b.first) {
            return true;
        } else {
            return false;
        } 
    });

    if(!writeToFile(argv[2], words)) {
        std::cout << "Can't write to '" << argv[2] << "'" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}