# wigner-basis-schrodinger

C++ code to build a rigid-rotor Hamiltonian in a Wigner-coupled (symmetric-top) basis, diagonalize it, and compute rotational partition functions and thermodynamic properties (internal energy, entropy, ZPE).

Developed in support of **UMRR** — **Uncoupled Mode (approximation) for Rigid Rotation**.

## Layout

- `armandham/hamiltonian.cpp` — main driver (basis convergence, thermodynamics)
- `armandham/cx_ham.cpp` — earlier variant
- `gaunt_coeffs.cpp` — prototype using Gaunt coefficients (`wigner-cpp`)
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
cd armandham
make
```

The root `Makefile` builds `gaunt` from `gaunt_coeffs.cpp` and expects a local `wigner-cpp` install; paths in the Makefiles may need editing for your machine.

## Run

```text
./ham <systemName> <Lmax_start> <free_rotor> <T_K>
```

- `systemName` — subdirectory name under the data path (e.g. `lebedev110`)
- `free_rotor` — `0` use `a.txt`; `1` kinetic terms only

Example:

```bash
./ham lebedev110 5 0 300
```

## Data path

Input files are read from `data/` at the repo root. Resolution order:

1. Environment variable `WIGNER_BASIS_DATA` (if set)
2. Compile-time `WIGNER_BASIS_DATA_DIR` (set by the Makefiles)
3. `data`, `../data`, or `../../data` relative to the working directory

Run `ham` from the repo root, or from `armandham/` after `make` (the Makefile embeds the absolute path to `data/`).

## License

MIT — see [LICENSE](LICENSE).
