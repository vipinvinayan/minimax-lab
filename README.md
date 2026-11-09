# Minimax Lab (Rebuilt)

A minimal GitHub Pages learning site with working games, an interactive quiz, and a real C++ search engine compiled to WebAssembly.

## Files

- `index.html` — website UI and game/quiz interactions.
- `minimax.cpp` — C++ source for Tic-Tac-Toe and Connect Four Minimax / Alpha-Beta search.
- `minimax_core.wasm` — compiled WebAssembly module used by both browser games.
- `tests/engine-smoke-test.js` — small Node.js regression test for legal winning moves and Minimax/Alpha-Beta score equivalence.

## Run locally

Because browsers generally block `fetch()` of WebAssembly from a `file://` URL, serve the folder through a local web server:

```bash
python -m http.server 8000
```

Then open `http://localhost:8000`.

## Deploy to GitHub Pages

Upload `index.html`, `README.md`, `minimax.cpp`, and `minimax_core.wasm` to the root of the existing repository, then commit. The `tests/` folder is optional for the live site but useful if you want to run the regression tests. Keep GitHub Pages configured to deploy from the `main` branch and `/ (root)`.

## How the C++ engine works

The browser loads `minimax_core.wasm`. The WebAssembly exports are compiled directly from `minimax.cpp`:

- `ttt_best_move()` runs complete Minimax or Alpha-Beta search for Tic-Tac-Toe.
- `c4_best_move()` runs depth-limited Minimax or Alpha-Beta search for Connect Four.
- Node and cutoff counters are returned by the C++ module and displayed in the UI.

Tic-Tac-Toe uses terminal utilities and a full game-tree search. Connect Four uses a depth limit, center-first move ordering, and a heuristic that scores groups of two, three, and four discs. It is an educational depth-limited AI, not a perfect Connect Four solver.

## Recompile WebAssembly

The checked-in `.wasm` is already compiled and is what GitHub Pages runs. To rebuild it with a compatible Clang/LLVM toolchain:

```bash
clang++ --target=wasm32 -O3 -fno-builtin -nostdlib minimax.cpp \
  -Wl,--no-entry -Wl,--export-memory \
  -Wl,--export=ttt_clear -Wl,--export=ttt_set_cell -Wl,--export=ttt_get_cell \
  -Wl,--export=ttt_winner_now -Wl,--export=ttt_is_full -Wl,--export=ttt_best_move \
  -Wl,--export=ttt_nodes -Wl,--export=ttt_cutoffs -Wl,--export=ttt_score \
  -Wl,--export=c4_clear -Wl,--export=c4_set_cell -Wl,--export=c4_get_cell \
  -Wl,--export=c4_winner_now -Wl,--export=c4_is_full -Wl,--export=c4_best_move \
  -Wl,--export=c4_nodes -Wl,--export=c4_cutoffs -Wl,--export=c4_score \
  -o minimax_core.wasm
```

If your compiler toolchain does not provide a WASM target, use Emscripten and its `em++` compiler instead, adapting the export flags.

## Run engine smoke tests

With Node.js installed, run from this folder:

```bash
node tests/engine-smoke-test.js
```
