#include "libsfe.h"
#include "file_utils.h"

#include <sqlite3.h>

namespace sfe {

    int Save(const char* savefile_name, const char* sql_file, const char* lua_file, const char* sqldiff_file /* = nullptr */)
    {
        GameFile gf{};
        gf.Header.Magic = 0x42;
        gf.Header.MajorVersion = 1;
        gf.Header.MinorVersion = 0;

        try
        {
            gf.GameData = ReadTextFile(sql_file);
            gf.CodeData = ReadTextFile(lua_file);
            if (sqldiff_file)
                gf.ProgressData = ReadTextFile(sqldiff_file);

            gf.Header.LengthOfGameSection = gf.GameData.length();
            gf.Header.LengthOfCodeSection = gf.CodeData.length();
            gf.Header.LengthOfProgressSection = gf.ProgressData.length();

            std::vector<unsigned char> data = Serialize(gf);

            StoreBinaryFile(savefile_name, data);
        } 
        catch(...)
        {
            return 1;
        }

        return 0;
    }


    GameFile Load(const char* filename)
    {
        std::vector<unsigned char> content = ReadBinaryFile(filename);
        if (content.empty())
        {
            return {}; // placeholder, not sure what to return yet
        }
        auto iter = content.begin();

        if (*iter != 0x42)
        {
            return {};
        }

        return Deserialize(iter);
    }


}

