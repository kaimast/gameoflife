import qbs

CppApplication {
    type: "application" // To suppress bundle generation on Mac
    consoleApplication: true
    files: ["main.cpp", "Game.h", "Game.cpp", "Tile.h",
        "Tile.cpp", "Graphics.h", "Graphics.cpp", "defines.h", "vector2.h"]

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: true
    }

    cpp.dynamicLibraries: ["SDL2", "SDL2_ttf"]
    cpp.cppFlags: ["-std=c++11"]
}

