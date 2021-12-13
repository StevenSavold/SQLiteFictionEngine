#pragma once 
#include <vector>
#include <iostream>

namespace sfe {

    struct GameFile
    {
        struct {
            uint8_t  Magic = 0;                    // The magic number (0x42)
            uint8_t  MajorVersion = 0;             // The major version number of the file format
            uint8_t  MinorVersion = 0;             // The minor version number of the file format
            uint64_t LengthOfGameSection = 0;      // The offset in bytes into the body that the sqlite db file can be found
            uint64_t LengthOfCodeSection = 0;      // The offset in bytes into the body that the lua file can be found
            uint64_t LengthOfProgressSection = 0;  // The offset in bytes into the body that the sql diff file can be found 
        } Header;

        // Textual SQL code that will construct a db file with the games contents
        std::string GameData; 

        // Textual data of the lua file
        std::string CodeData;

        // Textual SQL code that will alter the database created by GameData to represent the progress the player made
        std::string ProgressData; 

        inline bool isValid() { return Header.Magic == 0x42; }
    };

    template <typename T>
    T combineBytes(std::vector<unsigned char>::iterator iter)
    {
        std::size_t num_bytes = sizeof(T);
        std::size_t shiftBy = (num_bytes - 1) * 8;
        T accumulator = 0;

        for (std::size_t i = 0; i < num_bytes; ++i)
        {
            accumulator |= (*iter++) << shiftBy;
            shiftBy -= 8;
        }

        return accumulator;
    }


    std::vector<unsigned char> Serialize(const GameFile& gameData);
    GameFile Deserialize(std::vector<unsigned char>::iterator iter);

}