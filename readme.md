# OpticalMeasurementParser

A C++20 library for parsing **optical measurement files** for glazing layers used in LBNL window calculation tools. Parses both legacy IGDB text format and JSON measurements (IGSDB-style). Built on top of [BSDFXMLParser](https://github.com/LBNL-ETA/BSDFXMLParser) and [nlohmann_json](https://github.com/nlohmann/json).

## Requirements

- C++20 compatible compiler (`g++` 11+, `clang++` 14+, MSVC 19.30+)
- CMake 3.8+ (3.21+ if you want to use the shipped CMake presets)
- [BSDFXMLParser](https://github.com/LBNL-ETA/BSDFXMLParser) (fetched automatically)
- [nlohmann_json](https://github.com/nlohmann/json) (fetched automatically)

## Consuming OpticalMeasurementParser

### Using FetchContent in CMake (recommended)

```cmake
include(FetchContent)
FetchContent_Declare(
    OpticalMeasurementParser
    GIT_REPOSITORY https://github.com/LBNL-ETA/OpticalMeasurementParser.git
    GIT_TAG v2.3.9
)
FetchContent_MakeAvailable(OpticalMeasurementParser)

target_link_libraries(MyTarget PRIVATE OpticalMeasurementParser)
```

Update `GIT_TAG` to the desired release tag.

## Building (developers)

Dependencies are downloaded automatically via CMake FetchContent on first configure. Tests build by default when OpticalMeasurementParser is the top-level project (`-DBUILD_Optical_Measurement_Parser_Tests=OFF` to disable).

### Presets

`CMakePresets.json` ships four visible configure presets, plus two hidden inheritance bases:

| Preset | When to use it |
|---|---|
| `default-debug` / `default-release` | Standard configure on any platform; CI. Fetches all dependencies from declared remotes. Picks the system default compiler (MSVC on Windows, system `cc`/`c++` on Linux/macOS). |
| `local-debug` / `local-release` | Consume sibling working copies of dependencies instead of fetching them. |

Examples:

```
cmake --preset default-release
cmake --build build/default-release --parallel
ctest --test-dir build/default-release -C Release --output-on-failure
```

`local` expects a sibling directory layout — e.g. `../BSDFXMLParser` and `../json` next to `../OpticalMeasurementParser`. Currently overridden:

| Dependency | Expected sibling path |
|------------|----------------------|
| BSDFXMLParser | `../BSDFXMLParser` |
| nlohmann_json | `../json` |

Missing siblings fall back to the declared remote automatically, so `local-*` is safe to invoke even if you don't have the siblings checked out.

#### Per-machine compiler presets (`CMakeUserPresets.json`)

To use a specific compiler (`vs2022-release`, `gcc-13-debug`, `clang-18-release`, etc.), each developer maintains their own `CMakeUserPresets.json` next to `CMakePresets.json`. It is gitignored, read automatically by CMake (and CLion, VS Code, etc.), and stays on the developer's machine.

Personal presets `inherit` from one of the shipped presets (usually `local`, which gives you sibling-repo overrides for free) and override whatever they want. A complete realistic example -- building with WSL Clang on a Windows machine, with CLion 2023.2+ routed through the WSL toolchain automatically:

```json
{
    "version": 6,
    "configurePresets": [
        {
            "name": "clang-release",
            "displayName": "clang (Release)",
            "inherits": "local",
            "generator": "Ninja",
            "binaryDir": "${sourceDir}/build/clang-release",
            "cacheVariables": {
                "CMAKE_C_COMPILER":   "clang",
                "CMAKE_CXX_COMPILER": "clang++",
                "CMAKE_BUILD_TYPE":   "Release"
            },
            "vendor": {
                "jetbrains.com/clion": {
                    "toolchain": "WSL"
                }
            }
        }
    ]
}
```

### Manual configure (without presets)

```
cmake -B build
cmake --build build --config Release --parallel
ctest --test-dir build -C Release --output-on-failure
```

### Clean rebuild

Delete the `build/` directory and re-run the configure and build commands above.

## License

See the [LICENSE](LICENSE) file.

Berkeley Lab WINDOW Calc Engine (CalcEngine) Copyright (c) 2016 - 2019, The Regents of the University of California, through Lawrence Berkeley National Laboratory (subject to receipt of any required approvals from the U.S. Dept. of Energy). All rights reserved.

If you have questions about your rights to use or distribute this software, please contact Berkeley Lab's Innovation & Partnerships Office at IPO@lbl.gov.

NOTICE.  This Software was developed under funding from the U.S. Department of Energy and the U.S. Government consequently retains certain rights.  As such, the U.S. Government has been granted for itself and others acting on its behalf a paid-up, nonexclusive, irrevocable, worldwide license in the Software to reproduce, distribute copies to the public, prepare derivative works, and perform publicly and display publicly, and to permit other to do so.
