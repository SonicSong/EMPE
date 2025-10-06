# EMPE


## Building

### Ubuntu Linux
- `sudo apt-get install build-essential cmake pkg-config libabsl-dev libgtkmm-4.0-dev libgtk-4-dev libglib2.0-dev`
- For the love of god make sure you have latest version of abseil installed. Ubuntu repos are often out of date.
- `cmake -B build -DCMAKE_BUILD_TYPE=Release`
- `cmake --build build --config Release`

### macOS
- Install Homebrew if you haven't already: https://brew.sh/
- Install the following packages:
  - `brew install cmake pkg-config gtkmm4 gtk4 abseil git`
- `cmake -B build -DCMAKE_BUILD_TYPE=Release`
- `cmake --build build --config Release`

### Windows
Don't. It's not worth it.
- Install MSYS2
- Install the following packages:
  - `mingw-w64-ucrt-x86_64-gcc`
  -  `mingw-w64-ucrt-x86_64-cmake`
  - `mingw-w64-ucrt-x86_64-ninja`
  - `mingw-w64-ucrt-x86_64-pkg-config`
  - `mingw-w64-ucrt-x86_64-gtk4`
  - `mingw-w64-ucrt-x86_64-gtkmm-4.0`
  - `mingw-w64-ucrt-x86_64-abseil-cpp`
