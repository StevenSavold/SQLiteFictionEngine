-- The definition of the dumpsfe project
--    This console applicaiton is used to operate on the contents of an sfe game data file

project "authorsfe"
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
        "../sfe/src",
        "../vendor/argh"
    }

    links
    {
        "sfe",
        "pthread",
        "dl",
        "m"
    }

    filter "system:windows"
        defines
        {
            "AUTHORSFE_PLATFORM_WINDOWS"
        }

    filter "system:linux"
        defines
        {
            "AUTHORSFE_PLATFORM_LINUX"
        }

    filter "system:macosx"
        defines
        {
            "AUTHORSFE_PLATFORM_MACOS"
        }

    -- Filter for Debug configuration
    filter "configurations:Debug"
        defines 
        {
            "AUTHORSFE_DEBUG"
        }
        symbols "On" -- turn debug symbols ON

    -- Filter for Release configuration
    filter "configurations:Release"
        defines 
        {
            "AUTHORSFE_RELEASE"
        }
        optimize "Full" -- turn on Full optimizations
