# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.0.3] - XXXX-XX-XX

### Fixed
- Scan will never stop sleeping on Linux.

## [0.0.2] - 2021-10-09

### Added
- Safe implementation of `SimpleBLE::Adapter` and `SimpleBLE::Peripheral` classes.
- CppCheck and ClangFormat CI checks. _(Thanks Andrey1994!)_
- C-style API with examples.
- Access to manufacturer data in the `SimpleBLE::Peripheral` class, for Windows and MacOS.

### Fixed
- Compilation errors that came up during development. _(Thanks fidoriel!)_
- WinRT buffer allocation would fail. _(Thanks PatrykSajdok!)_
- `SimpleBLE::Adapter` would fail to stop scanning. _(Thanks PatrykSajdok!)_
- Switched WinRT initialization to single-threaded.

### Changed
- SimpleBluez dependency migrated to OpenBluetoothToolbox.

## [0.0.1] - 2021-09-06

### Added
- Initial definition of the full API.
- Usage examples of the library.
