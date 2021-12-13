-- A library which contains useful functions for serialization of save files
project "sfe"
    kind "SharedLib"
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
        "%{wks.location}/sqlite3/src"
    }

    links
    {
        "sqlite3",
        "pthread",
        "dl",
        "m"
    }

    filter "system:windows"
        defines
        {
            "ENGINE_PLATFORM_WINDOWS"
        }

    filter "system:linux"
        defines
        {
            "ENGINE_PLATFORM_LINUX"
        }

    filter "system:macos"
        defines
        {
            "ENGINE_PLATFORM_MACOS"
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
