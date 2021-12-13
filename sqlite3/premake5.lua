-- A library which contains useful functions for serialization of save files
project "sqlite3"
    kind "SharedLib"
    language "C"
    systemversion "latest"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.c",
        "src/**.h"
    }

    -- Filter for Debug configuration
    filter "configurations:Debug"
        symbols "On" -- turn debug symbols ON

    -- Filter for Release configuration
    filter "configurations:Release"
        optimize "Full" -- turn on Full optimizations
