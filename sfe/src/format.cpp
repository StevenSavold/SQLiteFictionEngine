#include "format.h"

namespace sfe {

    std::vector<unsigned char> Serialize(const GameFile& gameData)
    {
        std::vector<unsigned char> output{};

        output.emplace_back(gameData.Header.Magic);

        output.emplace_back(gameData.Header.MajorVersion);
        output.emplace_back(gameData.Header.MinorVersion);

        output.emplace_back(gameData.Header.LengthOfGameSection >> (64 - 8));
        output.emplace_back(gameData.Header.LengthOfGameSection >> (64 - 16));
        output.emplace_back(gameData.Header.LengthOfGameSection >> (64 - 24));
        output.emplace_back(gameData.Header.LengthOfGameSection >> (64 - 32));
        output.emplace_back(gameData.Header.LengthOfGameSection >> (64 - 40));
        output.emplace_back(gameData.Header.LengthOfGameSection >> (64 - 48));
        output.emplace_back(gameData.Header.LengthOfGameSection >> (64 - 56));
        output.emplace_back(gameData.Header.LengthOfGameSection >> (64 - 64));

        output.emplace_back(gameData.Header.LengthOfCodeSection >> (64 - 8));
        output.emplace_back(gameData.Header.LengthOfCodeSection >> (64 - 16));
        output.emplace_back(gameData.Header.LengthOfCodeSection >> (64 - 24));
        output.emplace_back(gameData.Header.LengthOfCodeSection >> (64 - 32));
        output.emplace_back(gameData.Header.LengthOfCodeSection >> (64 - 40));
        output.emplace_back(gameData.Header.LengthOfCodeSection >> (64 - 48));
        output.emplace_back(gameData.Header.LengthOfCodeSection >> (64 - 56));
        output.emplace_back(gameData.Header.LengthOfCodeSection >> (64 - 64));

        output.emplace_back(gameData.Header.LengthOfProgressSection >> (64 - 8));
        output.emplace_back(gameData.Header.LengthOfProgressSection >> (64 - 16));
        output.emplace_back(gameData.Header.LengthOfProgressSection >> (64 - 24));
        output.emplace_back(gameData.Header.LengthOfProgressSection >> (64 - 32));
        output.emplace_back(gameData.Header.LengthOfProgressSection >> (64 - 40));
        output.emplace_back(gameData.Header.LengthOfProgressSection >> (64 - 48));
        output.emplace_back(gameData.Header.LengthOfProgressSection >> (64 - 56));
        output.emplace_back(gameData.Header.LengthOfProgressSection >> (64 - 64));

        for (auto& byte : gameData.GameData)
        {
            output.emplace_back(byte);
        }

        for (auto& byte : gameData.CodeData)
        {
            output.emplace_back(byte);
        }

        for (auto& byte : gameData.ProgressData)
        {
            output.emplace_back(byte);
        }

        return output;
    }

    GameFile Deserialize(std::vector<uint8_t>::iterator iter)
    {
        GameFile gf{};

        // Read the header
        gf.Header.Magic = *iter++;

        gf.Header.MajorVersion = *iter++;
        gf.Header.MinorVersion = *iter++;

        gf.Header.LengthOfGameSection = combineBytes<uint64_t>(iter);
        iter += sizeof(uint64_t);

        gf.Header.LengthOfCodeSection = combineBytes<uint64_t>(iter);
        iter += sizeof(uint64_t);

        gf.Header.LengthOfProgressSection = combineBytes<uint64_t>(iter);
        iter += sizeof(uint64_t);

        for (int i = 0; i < gf.Header.LengthOfGameSection; ++i)
        {
            gf.GameData.push_back(*iter++);
        }

        for (int i = 0; i < gf.Header.LengthOfCodeSection; ++i)
        {
            gf.CodeData.push_back(*iter++);
        }

        for (int i = 0; i < gf.Header.LengthOfProgressSection; ++i)
        {
            gf.ProgressData.push_back(*iter++);
        }

        return gf;
    }

}