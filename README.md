# OneMoreSol

OneMoreSol is a 2D lane-defense game built in C++ with SFML 2.x.

## Prerequisites

- CMake 3.16+
- A C++ compiler with C++11 support
	- macOS: Apple Clang (Xcode Command Line Tools)
	- Linux: GCC or Clang
	- Windows: MSVC (Visual Studio) or MinGW
- SFML 2.x (`graphics`, `window`, `system`, `audio`, `network`)

### Install dependencies

#### macOS (Homebrew)

```bash
brew install cmake sfml@2
```

#### Ubuntu/Debian

```bash
sudo apt update
sudo apt install -y cmake g++ libsfml-dev
```

#### Windows

- Install CMake.
- Install Visual Studio (Desktop development with C++) or MinGW.
- Install SFML 2.x and note the install directory.

## Build (two commands)

Run from the repository root.

1) Configure:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

If SFML is not found, re-run configure with one of these:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="<sfml-install-prefix>"
```

or

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DSFML_ROOT="<sfml-install-prefix>"
```

Examples:
- Apple Silicon + Homebrew `sfml@2` prefix is often `/opt/homebrew/opt/sfml@2`.
- Intel macOS + Homebrew `sfml@2` prefix is often `/usr/local/opt/sfml@2`.

2) Build:

```bash
cmake --build build --config Release -j
```

## Run

The game loads assets using paths like `../assets/...`, so run it from the `build` directory:

```bash
cd build
./main
```

On Windows with multi-config generators, the executable may be in:

```powershell
build\Release\main.exe
```

## Troubleshooting

- **SFML not found at configure time**
	- Pass `-DCMAKE_PREFIX_PATH` or `-DSFML_ROOT` to the configure command.
- **Black screen / missing textures / missing audio**
	- Start the executable from the `build` directory so `../assets` resolves correctly.
- **Compiler errors about `std::array`**
	- Ensure you are building the latest repository version and with a C++11-capable compiler.

## Project notes

- Source: `src/`
- Headers: `include/`
- Assets (required at runtime): `assets/`

Created by Drew Glinsman, Chris Wolinski, Jack Bowden, and Alex Walhout.
