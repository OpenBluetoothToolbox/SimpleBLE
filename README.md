# SimpleBLE



## Installation

**TODO:** Pull all external dependencies from repository/commits/tags, allowing the user to override the origin of these dependencies.

`git submodule init`
`git submodule update`

`git clone --recurse-submodules ${path}`

`git submodule update --init --recursive`
`git submodule update --remote`
`git submodule update --remote --merge`


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

## Collaborating

### Coding & Naming Conventions
- This project follows the coding style guidelines specified in the `.clang-format` file.
  They are roughly derived from Google's coding standard with a few differences, outlined
  here:
    - Class function names are expressed in `snake_case`.
    - Class protected and private property names must end with an underscore (`_`).
- 

## Examples per OS

The following table describes the available functionality of each example for each
operating system.

| Example                 | Linux | Windows | MacOS |
| ----------------------- | ---   | ---     | ---   |
| `list_adapters` example | X     | X       | E     |
| `scan` example          | X     | X       | X     |
| `connect` example       | O     | O       | X     |

**Nomenclature**
- `X`: Supported
- `E`: Emulated
- `O`: Unsupported


## Known Issues
- MacOS implementation has incomplete error handling and might crash if invalid parameters are passed.
