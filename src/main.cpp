#include <vector>
#include <iostream>
#include <regex>

using TokenIter = std::sregex_token_iterator;
std::vector<std::string> readFile(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    std::vector<std::string> toReturn{};
    std::regex regexp("[A-Za-z]+");

    if (fp)
    {
        char *lineData = nullptr;
        size_t len = 0;
        while ((getline(&lineData, &len, fp)) != -1)
        {
            std::string line{lineData};
            std::cout << line << std::endl;
            TokenIter iter(line.begin(),
                           line.end(),
                           regexp,
                           -1);
            TokenIter end;

            for (; iter != end; ++iter)
                toReturn.push_back(*iter);
        }
        fclose(fp);
        if (lineData)
            free(lineData);
    }

    return toReturn;
}

int main(int argc, const char** argv)
{
    // for(int i = 1; i < argc; i++) {
    //     std::cout << argv[i] << std::endl;
    // }

    std::vector<std::string> words = readFile(argv[1]);
    for(const auto & word : words) {
        std::cout << word << std::endl;
    }
}