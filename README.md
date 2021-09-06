# SimpleBLE

The ultimate cr

Each major version of the library will have a stable API that will be fully
forwards compatible, only increasing the minor version number when new
features are added.

## Installation

### Windows
- Windows SDK Build 19041


## How to use




## Architecture

There is one master object that owns all adapters.
Adapters own peripherals.

### Layers and their responsibilities
- External layer
    - `SimpleBLE::Adapter` and `SimpleBLE::Peripheral` classes.
    - These objects hold a shared pointer to `SimpleBLE::AdapterBase` 
      and `SimpleBLE::PeripheralBase` respectively.
- API layer (OS-dependent)
    - `SimpleBLE::AdapterBase` and `SimpleBLE::PeripheralBase` classes.
    - These classes specify the API of the library on top of which
      the external layer is actually wrapping.
    - Each OS target has to implement the full public API specified in
      the external layer, using private methods and properties for 
      the specific requirements of each environment.
    - In some cases, whenever possible, these objects wrap their
      underlying OS representation in a weak pointer to allow these 
      managed objects to be reclaimed by the operating system without
      causing memory leaks. In case this happens, an exception of
      type `SimpleBLE::Exception::InvalidReference` will be thrown.

### Security

One key security feature of the library is it allows the user to specify
the URLs and tags of all internal dependencies, thus allowing compilation
from internal or secure sources without the risk of those getting compromised.

The following CMake parameters allow for this customization:
- `SIMPLEDBUS_GIT_REPOSITORY`
- `SIMPLEDBUS_GIT_TAG`

## Collaborating

### Coding & Naming Conventions
- This project follows the coding style guidelines specified in the `.clang-format` file.
  They are roughly derived from Google's coding standard with a few differences, outlined
  here:
    - Class function names are expressed in `snake_case`.
    - Class protected and private property names must end with an underscore (`_`).
- 

## API per OS

The following tables describe the state of each available API per operating system.
Any field that is not specified as supported will throw a compilation error if used
or just be ignored.

| API                                                  | Linux | Windows | MacOS |
| ---------------------------------------------------- | ----- | ------- | ----- |
| `SimpleBLE::Adapter::identifier`                     | Yes   | Yes     | Yes   |
| `SimpleBLE::Adapter::address`                        | Yes   | Yes     | Yes   |
| `SimpleBLE::Adapter::scan_start`                     | Yes   | Yes     | Yes   |
| `SimpleBLE::Adapter::scan_stop`                      | Yes   | Yes     | Yes   |
| `SimpleBLE::Adapter::scan_for`                       | Yes   | Yes     | Yes   |
| `SimpleBLE::Adapter::scan_is_active`                 | Yes   | Yes     | Yes   |
| `SimpleBLE::Adapter::scan_get_results`               | No    | No      | No    |
| `SimpleBLE::Adapter::set_callback_on_scan_start`     | Yes   | Yes     | Yes   |
| `SimpleBLE::Adapter::set_callback_on_scan_stop`      | Yes   | Yes     | Yes   |
| `SimpleBLE::Adapter::set_callback_on_scan_updated`   | Yes   | Yes     | Yes   |
| `SimpleBLE::Adapter::set_callback_on_scan_found`     | Yes   | Yes     | Yes   |
| `SimpleBLE::Adapter::get_adapters`                   | Yes   | Yes     | Yes   |
| ---------------------------------------------------- | ----- | ------- | ----- |
| `SimpleBLE::Peripheral::identifier`                  | Yes   | Yes     | Yes   |
| `SimpleBLE::Peripheral::address`                     | Yes   | Yes     | Yes   |
| `SimpleBLE::Peripheral::connect`                     | Yes   | Yes     | Yes   |
| `SimpleBLE::Peripheral::disconnect`                  | Yes   | Yes     | Yes   |
| `SimpleBLE::Peripheral::is_connected`                | Yes   | Yes     | Yes   |
| `SimpleBLE::Peripheral::is_connectable`              | No    | No      | No    |
| `SimpleBLE::Peripheral::services`                    | Yes   | Yes     | Yes   |
| `SimpleBLE::Peripheral::manufacturer_data`           | No    | No      | No    |
| `SimpleBLE::Peripheral::read`                        | No    | Yes     | Yes   |
| `SimpleBLE::Peripheral::write_request`               | Yes   | Yes     | Yes   |
| `SimpleBLE::Peripheral::write_command`               | Yes   | Yes     | Yes   |
| `SimpleBLE::Peripheral::notify`                      | No    | Yes     | Yes   |
| `SimpleBLE::Peripheral::indicate`                    | Yes   | Yes     | Yes   |
| `SimpleBLE::Peripheral::unsubscribe`                 | Yes   | Yes     | Yes   |
| `SimpleBLE::Peripheral::set_callback_on_connected`   | Yes   | Yes     | No    |
| `SimpleBLE::Peripheral::set_callback_on_disconnected`| Yes   | Yes     | No    |

## Examples per OS

The following table describes the available functionality of each example for each
operating system.

| Example                 | Linux | Windows | MacOS    |
| ----------------------- | ---   | ------- | -------- |
| `list_adapters` example | Yes   | Yes     | Emulated |
| `scan` example          | Yes   | Yes     | Yes      |
| `connect` example       | Yes   | Yes     | Yes      |
| `read` example          | Yes   | Yes     | Yes      |
| `write` example         | Yes   | Yes     | Yes      |
| `notify` example        | Yes   | Yes     | Yes      |

## Known Issues / To-Do's
- [Linux] Fork safety is not guaranteed.
- [Linux] `SimpleBLE::Peripheral::read` does not work.
- [Linux] `SimpleBLE::Peripheral::notify` runs slowly..
- [MacOS] Only the main system adapter can be detected.
- [MacOS] Implementation has incomplete error handling and might crash if invalid parameters are passed.
- [MacOS] Timeout logic can be DRYed up.
- [MacOS] Raise exceptions upon error.
- [MacOS] Service and characteristic UUIDs need to be normalized.
- [Windows] Unclear if multiple adapters can be detected.
- [All] Add a signal handler to ensure all objects are disconnected when the program exits.
- [All] Add safe version of the library that won't trigger any exceptions.
- [All] Replace C-style casts with C++ style casts.
- [All] Add C wrappers for the library.
- [All] Add Python bindings for the library.
- [All] Add Javascript bindings for the library.
- [All] Add Kotlin bindings for the library.

## Ideas
- Explore if callbacks can be wrapped in shared pointers to prevent them from being prematurely deleted.
- Explore if callbacks can be handled in a separate thread to prevent blocking the main thread.
