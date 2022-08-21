#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <regex>
#include <string>
#include <vector>

using TokenIterator = std::sregex_token_iterator;
using DictMap = std::unordered_map<std::string, size_t>;
using DictVector = std::vector<std::pair<std::string, size_t>>;

bool readFile(const char *filename, DictVector & words)
{
    FILE *fp = fopen(filename, "r");

    if(!fp) {
        return false;
    }

    DictMap dictMap{};
    char *lineData{nullptr};
    size_t len{0};
    std::regex regexp("[a-zA-Z]+");

    while ((getline(&lineData, &len, fp)) != -1) {
        std::string line{lineData};
        std::transform(line.begin(), line.end(), line.begin(), [](unsigned char c) {
            return std::tolower(c);
        });
        TokenIterator iter(line.begin(),
                           line.end(),
                           regexp,
                           0);
        TokenIterator end;

        for (; iter != end; ++iter) {
            if(dictMap.find(*iter) != dictMap.end()) {
                dictMap[*iter]++;
            } else {
                dictMap[*iter] = 1u;
            }
        }
    }

    fclose(fp);
    if (lineData)
        free(lineData);

    if(!dictMap.empty()) {
        words.reserve(dictMap.size());
        for(const auto & entry : dictMap) {
            words.emplace_back(entry.first, entry.second);
        }
    }

    return true;
}

int main(int argc, const char** argv)
{
    if(argc != 2) {
        std::cout << "Wrong usage. Give file name as the only parameter" << std::endl;
        return EXIT_FAILURE;
    }

    DictVector words; 
    if(!readFile(argv[1], words)) {
        std::cout << "Given file can't be read" << std::endl;
        return EXIT_FAILURE;
    }

    if(words.size() == 0) {
        std::cout << "Given file contains no words" << std::endl;
        return EXIT_SUCCESS;
    }

    std::sort(words.begin(), words.end(), [](auto a, auto b){
        if(a.second > b.second) {
            return true;
        } else if(a.second == b.second && a.first < b.first) {
            return true;
        } else {
            return false;
        }
    });

    for(const auto & v : words) {
        std::cout << v.second << " " << v.first << "\n";
    }
    std::cout << std::flush;

    return EXIT_SUCCESS;
}