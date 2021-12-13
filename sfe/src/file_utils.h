#pragma once
#include <fstream>
#include <vector>

namespace sfe {

    std::string ReadTextFile(const char* filename);
    std::vector<unsigned char> ReadBinaryFile(const char* filename);
    bool StoreBinaryFile(const char* filename, std::vector<unsigned char> content);

}