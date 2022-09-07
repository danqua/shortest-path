workspace "Pathfinding"
    architecture "x86_64"
    flags {
        "NoImportLib",
		"MultiProcessorCompile",
		"NoImplicitLink"
    }
    configurations {
        "Debug",
        "Release"
    }
    debugdir "data"
    targetdir "build/%{cfg.buildcfg}"
    objdir "build/obj/%{cfg.buildcfg}"

project "Dijkstra"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++11"
    files {
        "code/**.h",
        "code/**.cpp"
    }
    includedirs {
        "code",
        "extern/SDL2-2.24.0/include"
    }
    libdirs {
        "extern/SDL2-2.24.0/lib/x64"
    }
    links {
        "SDL2",
        "SDL2main"
    }
    disablewarnings {
		"4996"
	}
    postbuildcommands {
		"{COPY} extern/SDL2-2.24.0/lib/x64/SDL2.dll data",
	}
    filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"