-- Premake5 file to generate project files for all supported platforms
-- Author: Steven Savold

-- workspace (Solution) for the entire emulator 
workspace "SQLiteFictionEngine"
    configurations
    {
         "Debug32",
         "Release32",
         "Debug64",
         "Release64"
    }

    filter "configurations:*32"
        architecture "x86"
    
    filter "configurations:*64"
        architecture "x86_64"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Engine"
    location "Engine"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
	systemversion "latest"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/**.cpp",
        "%{prj.name}/**.c",
        "%{prj.name}/**.h"
    }

    includedirs
    {
        "%{prj.name}/src",
        "vendor/lua/lua-5/include"
    }

    sysincludedirs
    {
        "%{prj.name}/src",
        "vendor/lua/lua-5/include"
    }

    filter "system:windows"
        defines
        {
            "ENGINE_PLATFORM_WINDOWS"
        }

        libdirs
        {
            "vendor/lua/lua-5"
        }

        links
        {
            "liblua53",
        }

    filter "system:linux"
        defines
        {
            "ENGINE_PLATFORM_LINUX"
        }

        libdirs
        {
            "vendor/lua/lua-5"
        }

        links
        {
            "lua",
            "m",
            "dl"
        }

    filter "system:macosx"
        defines
        {
            "ENGINE_PLATFORM_MACOS"
        }

        links
        {
            "lua",
            "m",
            "dl"
        }

    -- Filter for Debug configuration
    filter "configurations:Debug"
        defines 
        {
            "ENGINE_DEBUG"
        }
        symbols "On" -- turn debug symbols ON

    -- Filter for Release configuration
    filter "configurations:Release"
        defines 
        {
            "ENGINE_RELEASE"
        }
        optimize "Full" -- turn on Full optimizations