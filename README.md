# wigner-basis-schrodinger

C++ code to build a rigid-rotor Hamiltonian in a Wigner-coupled (symmetric-top) basis, diagonalize it, and compute rotational partition functions and thermodynamic properties (internal energy, entropy, ZPE).

Developed in support of **UMRR** — **Uncoupled Mode (approximation) for Rigid Rotation**.

## Layout

- `src/hamiltonian.cpp` — main driver (basis convergence, thermodynamics)
- `src/aux/gaunt_coeffs.cpp` — auxiliary/prototype using Gaunt coefficients (`wigner-cpp`)
- `src/legacy/cx_ham.cpp` — legacy earlier variant (kept for reference)
- `include/data_dir.hpp` — shared data-directory resolution helper
- `data/` — input sets per system (Lebedev quadrature grids, etc.)

## Data (per system directory under `data/`)

| File | Contents |
|------|----------|
| `I.txt` | Principal moments of inertia (amu·Å²) |
| `a.txt` | Complex Wigner-basis potential coefficients |
| `lmax.txt` | Max \(L\) in the potential expansion |
| `s.txt` | Symmetry number \(\sigma\) (where present) |
| `Eref.txt` | Reference energy (where present) |

## Build

Requires [Armadillo](http://arma.sourceforge.net/), OpenMP-capable Clang, and the `wignerSymbols` library (`armandham`).

```bash
make
```

Targets:

- `make` (or `make ham`) builds `bin/ham` from `src/hamiltonian.cpp`
- `make gaunt_aux` builds `bin/gaunt_aux` from `src/aux/gaunt_coeffs.cpp` (requires `wigner-cpp`)

## Run

```text
./bin/ham <systemName> <Lmax_start> <free_rotor> <temperature_K>
```

- `systemName` — subdirectory name under the data path (e.g. `lebedev110`)
- `Lmax_start` — initial basis cutoff; code increments until convergence
- `free_rotor` — `0` use `a.txt`; `1` kinetic terms only
- `temperature_K` — runtime CLI argument (`argv[4]`) in Kelvin

Example:

```bash
./bin/ham lebedev110 5 0 300
```

## Data path

Input files are read from `data/` at the repo root. Resolution order:

1. Environment variable `WIGNER_BASIS_DATA` (if set)
2. Compile-time `WIGNER_BASIS_DATA_DIR` (set by the Makefiles)
3. `data`, `../data`, or `../../data` relative to the working directory

Auxiliary prototype:

```bash
make gaunt_aux
./bin/gaunt_aux
```

## License

MIT — see [LICENSE](LICENSE).
