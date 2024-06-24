workspace "justrightc"
    architecture "arm64"
    configurations { "Debug", "Release" }
    platforms { "x64", "x86", "Arm64" }
    
    location "build"
    targetdir "bin/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/%{prj.name}"
    objdir "bin-int/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/%{prj.name}"

    startproject "justrightc"

project "justrightc"
    kind "ConsoleApp"
    language "C++"
    targetname "justrightc"

    cppdialect "C++17"
    
    targetdir "bin/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/%{prj.name}"
    objdir "bin-int/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/%{prj.name}"

    files { "src/**.cpp" }
    
    includedirs { "include" }
    externalincludedirs { "include" }
    buildoptions { "-O2" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

project "justrightc-tests"
    kind "ConsoleApp"
    language "C++"
    targetname "justrightc-tests"

    cppdialect "C++17"
    
    targetdir "bin/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/%{prj.name}"
    objdir "bin-int/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/%{prj.name}"

    files { "src/**.cpp", "tests/**.cpp" }
    excludes { "src/main.cpp" }
    
    includedirs { "include" }
    externalincludedirs { "include" }

    testsDir = path.getabsolute("tests")
    buildoptions { "-O2", "-DTESTS_ROOT_DIR=" .. testsDir }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
