# Get started


## link difference between rust and C/C++

- linking connects calls to library code: **static** copies the code into the executable; **dynamic** only records `NEEDED libfoo.so`, which the loader (`ld-linux`) maps at startup (`ldd` lists them)
- **C/C++**: links its libraries dynamically by default (`libstdc++.so`, `libgcc_s.so`, `libc.so`). This works because C, and C++ on Linux, have a stable **ABI** (how calls, struct layouts and symbol names look in machine code), so a `.so` from one compiler version works with programs from another
- **rust**: every crate, `std` included, is compiled from source into an `.rlib` and linked **statically**; only the C runtime (`libc.so.6`, `libgcc_s.so.1`) is dynamic
  - why: rust has **no stable ABI** (layouts and symbol names may change between compiler versions; generics are instantiated in the calling crate), so crates ship as source

same hello world (`std::map` / `HashMap`), built in sonic-slave-trixie:

|build|dynamic deps|size (stripped)|
|-|-|-|
|`g++`|libstdc++, libgcc_s, libc|15 KB|
|`g++ -static`|none|1.8 MB|
|`cargo build --release`|libgcc_s, libc|366 KB|
|`RUSTFLAGS="-C target-feature=+crt-static"`|none|1.2 MB|
|`RUSTFLAGS="-C prefer-dynamic"`|`libstd-<hash>.so`|11 KB, runs only with that exact rustc|

consequences:
- **portability**: built on trixie and run on bookworm, the rust binary ran; the C++ one failed (``version `GLIBCXX_3.4.32' not found``). glibc still sets the minimum, so build on the oldest target release, or use `crt-static`/musl
- **interop**: rust and C/C++ only meet through the C ABI (`extern "C"`, `crate-type = ["cdylib"]`); e.g. the `swss-common` crate → `libswsscommon.so` C API, PyO3 → `libpython.so`
- **costs**: bigger binaries; processes don't share library code pages (compare memory with PSS, not RSS); a fix in a crate means rebuilding every binary that uses it
- **benefit**: link-time optimization across crates (`lto = true`)


## cargo

cargo = rust's build system and package manager: builds your code, and downloads and builds its dependencies (packages of rust code are called *crates*)

|command|what it does|
|-|-|
|`cargo --version`|check cargo is installed|
|`cargo new <name>`|create project `<name>/`: `Cargo.toml`, `src/main.rs`, a git repo and `.gitignore` (no repo inside an existing one unless `--vcs=git`; `--vcs` picks another VCS or none)|
|`cargo init`|make an existing directory a cargo project (creates `Cargo.toml`)|
|`cargo build`|debug build → `target/debug/<name>`; the first build creates `Cargo.lock`|
|`cargo run`|build if the source changed, then run|
|`cargo check`|check it compiles without producing an executable; much faster than `build`, so run it often while coding|
|`cargo build --release`|optimized build → `target/release/<name>`; slower to compile, use it to ship and to benchmark|

- `Cargo.toml` (TOML): `[package]` sets `name`, `version`, `edition`; `[dependencies]` lists the crates the project uses
- `Cargo.lock`: the exact versions of dependencies; cargo manages it, never edit it by hand
- `src/` holds the code; the top level is for README, license and config files
- two profiles: dev (quick to rebuild, `target/debug`) and release (optimized, `target/release`)
- same commands on every OS; to build an existing project: `git clone <url> && cd <project> && cargo build`

