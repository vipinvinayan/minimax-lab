// Run with: node tests/engine-smoke-test.js
const fs = require('node:fs');
const assert = require('node:assert/strict');

(async () => {
  const wasmBytes = fs.readFileSync(new URL('../minimax_core.wasm', `file://${__filename}`));
  const { instance } = await WebAssembly.instantiate(wasmBytes, {});
  const e = instance.exports;

  // Tic-Tac-Toe: choose a legal move and take an immediate forced win.
  e.ttt_clear();
  e.ttt_set_cell(0, 1); e.ttt_set_cell(1, 1);
  e.ttt_set_cell(3, -1); e.ttt_set_cell(4, -1);
  const tttMove = e.ttt_best_move(1, 1);
  assert.equal(tttMove, 2, 'Tic-Tac-Toe engine must take the winning square');
  assert.ok(e.ttt_nodes() > 0, 'Tic-Tac-Toe node counter must increment');

  // Connect Four: take a horizontal win in the next move.
  e.c4_clear();
  e.c4_set_cell(5, 0, 1); e.c4_set_cell(5, 1, 1); e.c4_set_cell(5, 2, 1);
  e.c4_set_cell(5, 4, -1);
  const c4Move = e.c4_best_move(1, 4, 1);
  assert.equal(c4Move, 3, 'Connect Four engine must take the winning column');
  assert.ok(e.c4_nodes() > 0, 'Connect Four node counter must increment');

  // Alpha-Beta and plain Minimax must agree on the score for the same position.
  e.c4_clear();
  e.c4_set_cell(5, 3, -1); e.c4_set_cell(5, 2, 1); e.c4_set_cell(5, 4, -1);
  e.c4_best_move(1, 4, 1);
  const alphaBetaScore = e.c4_score();
  e.c4_best_move(1, 4, 0);
  const plainScore = e.c4_score();
  assert.equal(alphaBetaScore, plainScore, 'Alpha-Beta must preserve the Minimax score');

  console.log('PASS: Tic-Tac-Toe forced win');
  console.log('PASS: Connect Four forced win');
  console.log('PASS: Alpha-Beta and plain Minimax return the same score');
})();
