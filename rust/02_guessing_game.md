# guessing game


## Cargo.lock

- `Cargo.toml` says which versions you **accept**; `Cargo.lock` records which ones cargo **picked**: the exact version of every package in the dependency tree, including the ones dependencies pull in
- cargo creates it next to `Cargo.toml` the first time it resolves the dependencies (`cargo build`, `cargo run`, `cargo generate-lockfile`) and keeps it up to date; never edit it by hand
- why it's needed: a version in `Cargo.toml` is a **range**. `rand = "0.8.5"` means `^0.8.5` = `>=0.8.5, <0.9.0`, so without a lock two builds on different days can get different code

the guessing game's `rand = "0.8.5"`, locked with cargo 1.99.1 (Nov 2026):
- cargo picked **rand 0.8.8**, the newest 0.8.x (the `available: v0.10.3` in its output is outside the range), and locked 14 packages: rand + 13 crates it depends on
- the lock covers every platform: `wasi` is listed although it's only used on WebAssembly, so one lock works on every OS

```toml
[[package]]
name = "rand"
version = "0.8.8"
source = "registry+https://github.com/rust-lang/crates.io-index"
checksum = "e058c7de0b26af77780c769414d6257830bb240f3c38477dbc2c16e5f54d6d4c"
dependencies = [
 "libc",
 "rand_chacha",
 "rand_core",
]
```

- `version` is exact; `source` is where it came from (a git dependency records the exact commit); `checksum` is the SHA-256 of the downloaded `.crate` file
- `version = 4` at the top of the file is the lock file format, not the project's version

how it keeps builds reproducible (each tried in the same scratch project):
- **the lock wins over newer releases**: if `Cargo.lock` exists and its versions still fit `Cargo.toml`, cargo uses them and doesn't look for newer ones. after `cargo update -p rand --precise 0.8.5`, the next resolve kept 0.8.5 although 0.8.8 exists
- **the code is checked, not only the version number**: cargo compares each `checksum` with the registry and with the downloaded file. with rand's checksum set to zeros: ``error: checksum for `rand v0.8.5` changed between lock files``
- **published versions never change**: crates.io never lets a released version be overwritten. *yanking* a version only stops new resolutions from choosing it; a lock that already lists it still builds
- **updates only happen when you ask** (table below). adding a dependency adds its entries and leaves the rest alone, unless it needs a newer version of something already locked
- **CI can refuse lock changes** with `--locked`. after adding a dependency to `Cargo.toml` without updating the lock: `error: cannot update the lock file .../Cargo.lock because --locked was passed to prevent this`

|command|what it does|
|-|-|
|`cargo update`|move every package to the newest version `Cargo.toml` allows (and that supports your Rust version)|
|`cargo update -p rand`|update only rand (0.8.5 → 0.8.8 in the scratch project)|
|`cargo update -p rand --precise 0.8.5`|set rand to an exact version|
|`cargo build --locked`|fail instead of changing `Cargo.lock`; use it in CI|
|`cargo build --frozen`|`--locked` + no network access|
|`cargo install --locked <crate>`|use the crate's published lock; without `--locked`, `cargo install` ignores it and picks the newest versions again|

- moving to rand 0.9 or 0.10 needs a `Cargo.toml` edit: `^0.8.5` never allows them (for `0.x` versions, cargo treats a change in the second number as incompatible)

what it doesn't pin:
- the compiler: pin it with a `rust-toolchain.toml` file (e.g. `channel = "1.99.1"`)
- anything outside cargo: system libraries that `-sys` crates link against (e.g. OpenSSL), what build scripts (`build.rs`) detect, settings such as `RUSTFLAGS`
- versions for projects that depend on a library: only the top-level project's lock is used; a project using your library goes by its own `Cargo.lock`
- byte-identical binaries: the lock fixes the dependency source code; identical output also needs the same compiler, flags and build paths

commit `Cargo.lock` to git. the old advice was programs only, not libraries; since 2023 the cargo team suggests committing it by default for both
