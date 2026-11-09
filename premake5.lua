workspace "damsel"
    configurations { "Debug", "Release" }
    architecture "x86_64"
    location "build"

project "damsel"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++23"

    targetdir "bin/%{cfg.buildcfg}"
    objdir "obj/%{cfg.buildcfg}"

    files { "src/**.cpp", "include/**.hpp" }
    includedirs { "include" }

    warnings "Extra"
    floatingpoint "Strict"

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
        optimize "Off"

    filter "configurations:Release"
        defines { "NDEBUG" }
        symbols "Off"
        optimize "Speed"
        buildoptions { "-march=native" }
