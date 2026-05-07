# CLU PDF Rotate

A command-line utility that rotates a single page of a PDF using the Adobe
PDF Library (APDFL 18.0.5+). The rotation is **additive** — the value passed
on the command line is added to the page's existing `/Rotate` entry.

## Usage

```bash
pdfrotate <input.pdf> <page> <rotation> [-o <output.pdf>]
```

- `input.pdf`  — the PDF file to operate on; never modified.
- `page`       — 1-based page number to rotate.
- `rotation`   — degrees to rotate by; must be a multiple of 90 (positive or negative).
- `-o, --output` (optional) — output path. If omitted, the output is written
  next to the input as `<basename>-rotated.<ext>`.

Examples:

```bash
# Rotate page 3 by 90 degrees clockwise; output is foo-rotated.pdf
pdfrotate foo.pdf 3 90

# Rotate page 1 by 180 degrees and write to a custom location
pdfrotate sample.pdf 1 180 -o /tmp/flipped.pdf

# Counter-clockwise rotation (negative values are supported)
pdfrotate -i report.pdf -p 5 -r -90
```

## Setup

- Requires Python 3.8+ and Git 2.3+.
- Create/update the virtual environment:
  ```bash
  ./mkenv.py
  ```
- Activate it:
  - Windows: `python-env-[name]\Scripts\activate`
  - macOS/Linux: `. ./python-env-[name]/bin/activate`
- Optional but recommended: install git hooks locally with `pre-commit install`.

## Build and Test

- Bootstrap Conan profiles/presets and third-party dependencies:
  ```bash
  invoke -e bootstrap --update --config=Release
  ```
- Build:
  ```bash
  invoke -e build --config=Release
  ```
- Run tests via CTest:
  ```bash
  invoke -e test --config=Release
  ```
- When running the binary directly, source the Conan run environment first:
  ```bash
  source build/Release/generators/conanrun.sh
  build/Release/bin/pdfrotate --help
  ```

## Installers

After a Release build on Linux, generate the installer tarball:
```bash
invoke -e make-installers --config=Release
```

## Exit Codes

| Code | Meaning |
|------|---------|
| 0    | Success |
| 101  | Unknown fatal error |
| 102  | Internal error (library init, etc.) |
| 103  | Input file not found |
| 104  | Input file could not be opened (corrupt, password-protected, not a PDF) |
| 105  | Page number out of range |
| 106  | Output file could not be saved |
| 107  | Rotation value is not a multiple of 90 |
| 108  | Invalid command-line arguments |
