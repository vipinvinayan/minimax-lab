# Minimax Lab

An interactive, static website for learning adversarial search.

**Features:** Minimax and Alpha-Beta explainer with pseudocode; step-by-step tree visualizer (editable leaves, move-ordering experiments, Minimax vs Alpha-Beta statistics); Algorithm Arena comparing Minimax, Alpha-Beta, Negamax, Expectimax and MCTS; Tic-Tac-Toe vs an unbeatable AI; Connect Four vs a depth-limited negamax AI (Alpha-Beta, iterative deepening, transposition table, hints, undo); quiz.

## Deploy on GitHub Pages
1. Create a public repository named `minimax-lab`.
2. Upload `index.html` and `README.md` (Add file → Upload files) and commit.
3. Go to Settings → Pages → Deploy from a branch → `main` → `/ (root)` → Save.
4. After a minute the site is live at `https://YOUR-USERNAME.github.io/minimax-lab/`.

To update later, upload the new `index.html` with the same name and commit.

## Notes
- Pure HTML/CSS/JS, no build step, no backend.
- The tree visualizer uses illustrative random trees, not the Connect Four tree.
- Connect Four search is depth-limited with a heuristic; it does not solve the game.
- MCTS results are random estimates and vary between runs.
