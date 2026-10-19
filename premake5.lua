workspace "damsel"
    configurations { "Debug", "Release" }
    architecture "x86_64"
    location "build"

project "damsel"
    kind "ConsoleApp"
    language "C++"
    cdialect "C++23"

    targetdir "bin/%{cfg.buildcfg}"
    objdir "obj/%{cfg.buildcfg}"

    files { "src/**.c", "include/**.h" }
    includedirs { "include" }

    warnings "Extra"

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
        optimize "Off"

    filter "configurations:Release"
        defines { "NDEBUG" }
        symbols "Off"
        optimize "Speed"
        buildoptions { "-march=native" }
