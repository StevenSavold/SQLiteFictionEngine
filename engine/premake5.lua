-- The definition of the Engine project
--    This is the command line app that runs the game

project "engine"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
	systemversion "latest"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "**.cpp",
        "**.c",
        "**.h"
    }

    includedirs
    {
        "src"
    }

    sysincludedirs
    {
        "%{wks.location}/vendor/lua/lua-5/include",
        "%{wks.location}/sqlite3/src"
    }

    filter "system:windows"
        defines
        {
            "ENGINE_PLATFORM_WINDOWS"
        }

        libdirs
        {
            "%{wks.location}/vendor/lua/lua-5"
        }

        links
        {
            "liblua53",
            "sqlite3"
        }

    filter "system:linux"
        defines
        {
            "ENGINE_PLATFORM_LINUX"
        }

        libdirs
        {
            "%{wks.location}/vendor/lua/lua-5"
        }

        links
        {
            "lua",
            "sqlite3",
            "pthread",
            "dl",
            "m"
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
            "dl",
            "sqlite3"
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
