import qbs

CppApplication {
    type: "application"
    consoleApplication: true
    files: ["main.cpp", "Game.h", "Game.cpp", "Tile.h",
        "Tile.cpp", "Graphics.h", "Graphics.cpp", "defines.h", "vector2.h",
        "TileUpdater.h", "TileUpdater.cpp", "Benchmark.h", "Benchmark.cpp"]

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: true
    }

    cpp.dynamicLibraries: ["SDL2", "SDL2_ttf", "pthread", "boost_program_options"]
    cpp.cppFlags: ["-std=c++1y"]
}

