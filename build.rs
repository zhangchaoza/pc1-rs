extern crate bindgen;
extern crate cc;

use std::env;
use std::path::PathBuf;

fn main() {
    println!("cargo:rerun-if-changed=pc1_128/pc1_128.c");
    println!("cargo:rerun-if-changed=pc1_256/pc1_256.c");

    cc::Build::new()
        .file("pc1_128/pc1_128.c")
        .file("pc1_256/pc1_256.c")
        .compile("pc1");

    let lib_path = PathBuf::from(env::current_dir().unwrap());

    println!("cargo:rustc-link-search={}", lib_path.display());
    println!("cargo:rustc-link-lib=static=pc1");

    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());

    _ = bindgen::Builder::default()
        .header("pc1_128/pc1_128.c")
        .allowlist_function("pc1_128_encrypt_block")
        .allowlist_function("pc1_128_decrypt_block")
        .generate()
        .expect("unable to generate pc1_128 bindings")
        .write_to_file(out_path.join("bindings_pc1_128.rs"))
        .expect("couldn't write bindings_pc1_128!");

    _ = bindgen::Builder::default()
        .header("pc1_256/pc1_256.c")
        .allowlist_function("pc1_256_encrypt_block")
        .allowlist_function("pc1_256_decrypt_block")
        .generate()
        .expect("unable to generate pc1_256 bindings")
        .write_to_file(out_path.join("bindings_pc1_256.rs"))
        .expect("couldn't write bindings_pc1_256!");
}
