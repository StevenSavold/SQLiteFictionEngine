-- Premake5 file to generate project files for all supported platforms
-- Author: Steven Savold

-- workspace (Solution) for the entire emulator 
workspace "SQLiteFictionEngine"
    configurations
    {
         "Debug",
         "Release"
    }

    architecture "x86_64"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group ""
    include "engine"
    
group "tools"
    include "authorsfe"
    include "sfe"

group "vendor"
    include "sqlite3"