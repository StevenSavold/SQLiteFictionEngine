#pragma once
#include <fstream>
#include "format.h"

namespace sfe {
    int Save(const char* savefile_name, const char* sql_file, const char* lua_file, const char* sqldiff_file = nullptr);
    GameFile Load(const char* filename);
}
