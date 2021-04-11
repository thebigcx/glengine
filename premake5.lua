workspace "engine"
    configurations { "debug", "release" }

project "engine"
    kind "StaticLib"
    cppdialect "C++17"

    targetdir "bin/%{cfg.buildcfg}/engine"
    objdir "obj/%{cfg.buildcfg}/engine"

    includedirs {
        "src", "src/engine/"
    }

    files {
        "src/engine/**.h", "src/engine/**.cpp", "src/engine/**.c"
    }

    filter "configurations:debug"
        symbols "On"

    filter "configurations:release"
        optimize "On"
        runtime "Release"

project "editor"
    kind "ConsoleApp"
    cppdialect "C++17"

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
        "assimp"
    }

    filter "configurations:debug"
        symbols "On"

    filter "configurations:release"
        optimize "On"
        runtime "Release"