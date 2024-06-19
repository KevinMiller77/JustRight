-- Output directory: bin/{Configuration}/{OS}/{Arch}/{ProjectName}
-- Intermediate dir: obj/{Configuration}/{OS}/{Arch}/{ProjectName}
-- Workspace Name: JustRight
-- Project Name: justrightc
-- Project Kind: ConsoleApp
-- Project Language: C
-- Project Architecture: Arm64
-- Project Configuration: Debug, Release (define DEBUG for Debug)

workspace "JustRight"
    architecture "arm64"
    configurations { "Debug", "Release" }
    platforms { "x64", "x86", "Arm64" }
    
    location "build"
    targetdir "bin/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/%{prj.name}"
    objdir "bin-int/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/%{prj.name}"

    startproject "justrightc"

project "justrightc"
    kind "ConsoleApp"
    language "C"
    targetname "justrightc"

    cdialect "C11"
    
    targetdir "bin/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/%{prj.name}"
    objdir "bin-int/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/%{prj.name}"

    files { "src/**.c" }
    
    includedirs { "include" }
    externalincludedirs { "include" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"