use cmake;
use cxx_build;
use std::env;
use std::path::Path;

fn main() {
    // TODO: Add all files that would trigger a rerun
    println!("cargo:rerun-if-changed=build.rs");

    let target_os = env::var("CARGO_CFG_TARGET_OS").unwrap();

    let build_debug = env::var("DEBUG").unwrap() == "true";
    if build_debug { println!("cargo:warning=Building in DEBUG mode"); }

    // The simpleble library name depends if we're building in debug more or not.
    let simpleble_library_name = if build_debug {"simpleble-debug"} else {"simpleble"};
    let simpleble_build_dest = cmake::Config::new("../simpleble").build();
    let simpleble_include_path = Path::new(&simpleble_build_dest).join("include");

    cxx_build::CFG.exported_header_dirs.push(&simpleble_include_path);

    println!("cargo:rustc-link-search=native={}/lib", simpleble_build_dest.display());
    println!("cargo:rustc-link-lib=static={}", simpleble_library_name);

    match target_os.as_str() {
        "macos" => {
            println!("cargo:rustc-link-lib=framework=Foundation");
            println!("cargo:rustc-link-lib=framework=CoreBluetooth");
        },
        "windows" => {},
        "linux" => {},
        &_ => panic!("Unexpected target OS")

    }

    // Build the bindings
    cxx_build::bridge("src/lib.rs")
        .flag_if_supported("-std=c++17")
        .compile("simpleble_bindings");
}
