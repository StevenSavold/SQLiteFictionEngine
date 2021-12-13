#include "file_utils.h"
#include <fstream>
#include <algorithm>

namespace sfe {

    std::string ReadTextFile(const char* filename)
    {
        std::ifstream t(filename);
        return {std::istreambuf_iterator<char>(t), std::istreambuf_iterator<char>()};
    }

    std::vector<unsigned char> ReadBinaryFile(const char* filename)
    {
        std::ifstream file{filename, std::ios::binary};
        if (!file.is_open())
        {
            return {};
        }

        return {std::istreambuf_iterator<char>(file), {}};
    }   

    bool StoreBinaryFile(const char* filename, std::vector<unsigned char> content)
    {
        std::ofstream file{filename, std::ios::out | std::ios::binary};
        if (!file.is_open())
        {
            return false;
        }

        std::copy(content.cbegin(), content.cend(), std::ostreambuf_iterator<char>(file));

        return !file.fail();
    } 

}