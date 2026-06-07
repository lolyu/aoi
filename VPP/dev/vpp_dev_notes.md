# VPP Dev Notes

## Build & Run Unit Tests

All commands run from the repo root: `/home/lolv/workspace/repo/vpp_build2/vpp`

### 1. Install build dependencies (first time only)

```bash
make install-dep
```

Detects the OS (Ubuntu/Debian/RHEL/Fedora/etc.) and installs required packages via the system package manager. Requires `sudo`.

### 2. Build VPP

| Command | Result |
|---|---|
| `make build` | Debug build (default) |
| `make build-release` | Optimized release build |
| `make rebuild` | Wipe + rebuild debug |
| `make rebuild-release` | Wipe + rebuild release |
| `make wipe` / `make wipe-release` | Clean only |

Artifacts land in `build-root/build-vpp[_debug]-native/` and `build-root/install-vpp[_debug]-native/`.

### 3. Run unit tests

| Command | Purpose |
|---|---|
| `make test` | Run all tests against release build |
| `make test-debug` | Run all tests against debug build |
| `make test TEST=<module>` | Run a specific test file (omit `.py`) |
| `make test TEST=<module>.<Class>` | Run a single test class |
| `make test TEST=<module>.<Class>.<method>` | Run a single test method |
| `make test-cov` | Run tests with code coverage (gcov) |

Examples:

```bash
# Run the full IPIP test file
make test TEST=test_ipip

# Run only the TestIPIP class
make test TEST=test_ipip.TestIPIP

# Run a single test
make test TEST=test_ipip.TestIPIP.test_ipip4_mp2p

# Debug a test under gdb (foreground, single core)
make test-debug DEBUG=gdb TEST=test_ipip.TestIPIP.test_ipip4_mp2p
```

### 4. Useful test variables

| Variable | Effect |
|---|---|
| `V=1` | Verbose output |
| `DEBUG=gdb` | Run vpp under gdb (foreground) |
| `DEBUG=gdbserver` | Run vpp under gdbserver |
| `DEBUG=core` | Keep core dumps |
| `DEBUG=attach` | Attach to a running vpp |
| `STEP=1` | Step through each test |
| `RND_SEED=<n>` | Reproducible random seed |
| `EXTENDED_TESTS=1` | Include extended test suites |
| `CACHE_OUTPUT=1` | Cache test output |
| `TEST_JOBS=<n>` | Parallel test workers (default auto) |

### 5. Filtering tests with `make test-debug`

Use `TEST=` (same syntax as `make test`):

```bash
# whole module
make test-debug TEST=test_ipip

# single class
make test-debug TEST=test_ipip.TestIPIP

# single method
make test-debug TEST=test_ipip.TestIPIP.test_ipip4_mp2p

# under gdb
make test-debug DEBUG=gdb TEST=test_ipip.TestIPIP.test_ipip4_mp2p
```

### 6. Run tests in parallel

Use `TEST_JOBS=` to spawn multiple python test workers (each spawns its own VPP process):

```bash
make test TEST_JOBS=auto              # one worker per CPU
make test TEST_JOBS=8                 # explicit worker count
make test TEST_JOBS=4 TEST=test_ipip  # parallel + filter
```

Notes:
- Default is `TEST_JOBS=1` (serial).
- `TEST_JOBS=auto` picks a sensible number based on CPU count.
- Parallel mode is **incompatible** with `DEBUG=`, `STEP=1`, or `PROFILE=` — the Makefile fails the sanity check if any of those are set with `TEST_JOBS != 1/auto`. So `make test-debug DEBUG=gdb` must stay serial.
- Each worker uses its own `/tmp/vpp-unittest-*` dir, so output stays separated.

### 7. Test artifacts

- Test logs: `/tmp/vpp-unittest-<TestClass>-<rand>/`
- Failed tests: `/tmp/vpp-failed-unittests/`
- Python venv: `build-root/test/venv/`

## contribute PR upstream



## references
* https://s3-docs.fd.io/vpp/26.06/contributing/gitreview.html
