# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

CLU PDF Rotate is a command-line utility that rotates a single page of a PDF
using the Adobe PDF Library (APDFL 18.0.5+). Rotation is additive — the
supplied value is added to the page's existing `/Rotate` value, then
normalized into [0, 360).

## Build Commands

Requires Python 3.8+ and Git 2.3+. Always activate the virtualenv before running invoke tasks.

```bash
# Setup (first time or after dependency changes)
./mkenv.py
. ./python-env-$(hostname)/bin/activate

# Bootstrap Conan profiles and dependencies
invoke -e bootstrap --update --config=Release

# Build
invoke -e build --config=Release

# Run tests (CTest)
invoke -e test --config=Release

# Clean everything
invoke -e distclean

# Generate Linux installer
invoke -e make-installers --config=Release
```

When running binaries directly, source the Conan environment first:
```bash
source build/Release/generators/conanrun.sh
build/Release/bin/pdfrotate --help
```

## Architecture

### Source Layout
- `src/clu_pdf_rotate/` - CLI entry point (`main_func.cpp`, `rotatorlib.h`)
- `src/RotatorLib/` - Core rotation implementation:
  - `clu_pdf_rotate.cpp` - `Rotator_Main()` + `RotatePage()` (APDFL calls)
  - `clu_pdf_rotate_utility.cpp` - `ProcessCmdArgs()`, `NormalizeRotation()`, `DeriveOutputPath()`
  - `ErrorCodes.h` - Return codes (0 success, 101-108 errors)

### Build Outputs
- `build/<CONFIG>/bin/pdfrotate` - Main executable
- `build/<CONFIG>/bin/UnitTest` - Catch2 test harness
- `build/<CONFIG>/lib/rotatorlib` - Static library containing core logic

### Key Data Structures
- `RotateRec` - Input doc path, output doc path, page number (1-based), rotation in degrees
- `CLUError` - Exception class with `ReturnCode` for error propagation

### CLI
```
pdfrotate <input.pdf> <page> <rotation> [-o <output.pdf>]
```
Output defaults to `<input>-rotated.<ext>` next to the input.

## Dependencies

Managed via Conan 2 with `dl_val_toolkit` invoke tasks:
- Adobe PDF Library (APDFL) - PDF manipulation
- Boost (program_options, filesystem)
- Catch2 - Unit testing
- common-utils - Datalogics internal utilities

## Testing

Unit tests live in `unit_tests/UnitTests.cpp` (Catch2). They cover the pure
helpers (`NormalizeRotation`, `DeriveOutputPath`); integration tests that
exercise APDFL belong in `unit_tests/test_input/` with sample PDFs.

```bash
# Run all tests
invoke -e test --config=Release

# Run tests directly (after sourcing conanrun.sh)
build/Release/bin/UnitTest
```

## Code Quality

Pre-commit hooks are configured. Run before committing:
```bash
pre-commit install        # One-time setup
pre-commit run -a         # Run all hooks
```

## Platform Support

- macOS (Intel x86_64, ARM64)
- Linux (Rocky 8 x86_64, ARM64)
- Windows (x64 MSVC)

Platform-specific Conan profiles are defined in `dlproject.yaml`.
