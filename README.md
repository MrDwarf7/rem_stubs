# rem_stubs

A simple Windows command-line utility to delete empty directories within a specified folder.
Written in C, it uses Unicode-aware Windows APIs for robust path handling.

## Features

- Validates input directory paths.
- Enumerates first-level subdirectories and deletes those that are empty.
- Provides detailed error messages for invalid paths or permissions issues.

## Requirements

- Windows operating system.
- MinGW-w64 (e.g., installed via `scoop install mingw-winlibs-llvm-ucrt`).
- GNU Make.

## Building

1. Clone the Repository:

```bash
git clone https://github.com/MrDwarf7/rem_stubs.git
cd rem_stubs
 ```

2. Build the Project:

- For a debug build (default):

```bash
make
```

- For a release build (optimized):

```bash
make BUILD_TYPE=release
```

- For verbose output:

```bash
make V=1
```

- The executable will be in build/rem_stubs.exe.

3. Clean Build Artifacts:

```bash
make clean
```

4. Install (Optional):

- Copies rem_stubs.exe to ~/bin, creating the directory if it doesn't already exist
  (ensure it’s in your PATH):

```bash
make install
```

## Usage

Run the program with a directory path to delete its empty subdirectories:

```bash
.\build\rem_stubs.exe "C:\path\to\folder"
```

- Example:

 ```bash
mkdir "C:\Temp\test_dir"
.\build\rem_stubs.exe "C:\Temp"
```

Output (if test_dir is empty):

```plaintext
Path is C:\Temp
File attributes is 16
Deleted empty directory: C:\Temp\test_dir
```

## Error Handling:

- Invalid paths: "Path does not exist" or "Path is not a directory".
- Non-empty directories: Skipped without deletion.
- Usage error: Usage: rem_stubs.exe <folder> if no/invalid arguments.

## Project Structure

- main.c: Core program logic.
- Makefile: Build configuration for MinGW with debug/release modes.
- build/: Output directory containing obj/ (object files) and rem_stubs.exe.
- CMakeLists.txt: CMake configuration file (if needed for future builds).

[//]: # (- tests/: Placeholder for future test files.)

### Notes

- Requires -municode for Unicode support (wmain entry point) (Both Compiler and Linker flags!).
- Uses Windows console subsystem (-Wl,--subsystem,console).
- Tested with MinGW-w64 (GCC 14.2.0) on Windows.

## Planned Features

- Recursive deletion of empty directories.
- Support for command-line arguments to specify depth or other options.
- Integration with a logging system for better traceability.
- Unit tests for core functionality.
- Cross-platform compatibility (if feasible).
- Documentation improvements, including usage examples and error handling.
- Support for configuration files to customize behavior.
- Option to dry-run (preview deletions without actual removal).
- Support for symbolic links and junction points. (Harder & requires more testing)
- Option to skip certain directories based on patterns or names.

### Contributing

Feel free to open issues or submit pull requests on GitHub.

### License

MIT License (see LICENSE file, if added).
