workspace "engine"
    configurations { "debug", "release" }

project "engine"
    kind "StaticLib"
    cppdialect "C++17"
    buildoptions { "-Wall", "-pedantic" }

    targetdir "bin/%{cfg.buildcfg}/engine"
    objdir "obj/%{cfg.buildcfg}/engine"

    includedirs {
        "src", "src/engine/"
    }

    files {
        "src/engine/**.h", "src/engine/**.cpp", "src/engine/**.c"
    }

    optimize "On"

    filter "configurations:debug"
        symbols "On"

    filter "configurations:release"
        optimize "On"
        runtime "Release"

project "editor"
    kind "ConsoleApp"
    cppdialect "C++17"
    buildoptions { "-Wall", "-pedantic" }

    targetdir "bin/%{cfg.buildcfg}/editor"
    objdir "obj/%{cfg.buildcfg}/editor"

    includedirs {
        "src", "src/editor/", "src/engine", "src/editor/imgui"
    }

    libdirs {
        "bin/%{cfg.buildcfg}/onyx"
    }

    files {
        "src/editor/**.h", "src/editor/**.cpp"
    }

    links {
        "engine",
        "glfw",
        "dl",
        "lua",
        "openal",
        "assimp",
        "yaml-cpp"
    }

    filter "configurations:debug"
        symbols "On"

    filter "configurations:release"
        optimize "On"
        runtime "Release"