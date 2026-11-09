# Minimax Lab

A public, static website for learning the Minimax algorithm and alpha-beta pruning.

## Features
- Minimax concepts and a step-by-step example tree
- Advanced editable game-tree lab (depth 2–4)
- Editable terminal utility scores and animated value propagation
- Alpha-beta pruning visualization, cutoff explanations, and node comparison
- Minimax pseudocode and complexity notes
- Playable Tic-Tac-Toe against a Minimax AI
- Connect Four game arena with depth-limited heuristic search
- Iterative deepening, configurable alpha-beta pruning, center-first move ordering, and transposition-table caching
- Live search metrics: nodes, cutoffs, cache hits, completed depth, evaluation score, and suggested move
- Hint and undo controls for interactive experimentation
- Knowledge-check quiz

## Patch 2.0 — Advanced game-playing lab
The Connect Four arena uses a negamax formulation of Minimax with a four-in-a-row heuristic evaluator. Configure search depth, switch between plain Minimax and alpha-beta pruning, toggle center-first move ordering and transposition caching, request a suggested move, and undo the last turn. Iterative deepening runs from depth 1 to the selected maximum depth.

## Publish with GitHub Pages
1. Create a **public** repository named `minimax-lab` on GitHub.
2. Upload `index.html` and `README.md` to the repository root.
3. Open **Settings → Pages**.
4. Under **Build and deployment**, choose **Deploy from a branch**.
5. Select branch `main` and folder `/ (root)`, then save.
6. After deployment, visit `https://YOUR-USERNAME.github.io/minimax-lab/`.

No build step or server is required. The page is plain HTML, CSS and JavaScript.


## Patch 3.0 — Search Visualisation Studio

Patch 3 adds an interactive, configurable game-tree search studio:
- Configure tree depth and branching factor.
- Compare Minimax with Alpha-Beta pruning.
- Switch between promising-first, promising-last and natural move ordering.
- Step backward/forward, autoplay, or run the search to completion.
- Inspect visited and pruned nodes, current node, alpha/beta bounds, and root utility.
- Compare the selected search with the full Minimax baseline.

The studio uses an illustrative game tree with terminal utility scores; the Connect Four game remains a separate playable lab.
