// Minimax Lab — C++ game-search core
// The same source is compiled to WebAssembly for the GitHub Pages games.
// Values: -1 = human / Red / X, +1 = AI / Yellow / O, 0 = empty.

#if defined(__wasm32__)
// Tiny freestanding runtime helper: keeps the WebAssembly module self-contained.
extern "C" void* memset(void* ptr, int value, unsigned int count) {
    unsigned char* out = static_cast<unsigned char*>(ptr);
    for (unsigned int i = 0; i < count; ++i) out[i] = static_cast<unsigned char>(value);
    return ptr;
}
#endif

static int ttt_board[9];
static int ttt_node_count = 0;
static int ttt_cutoff_count = 0;
static int ttt_last_score = 0;

static int c4_board[42];
static int c4_node_count = 0;
static int c4_cutoff_count = 0;
static int c4_last_score = 0;

static int ttt_winner(const int* b) {
    const int lines[8][3] = {
        {0,1,2},{3,4,5},{6,7,8}, {0,3,6},{1,4,7},{2,5,8},
        {0,4,8},{2,4,6}
    };
    for (int i=0;i<8;i++) {
        int s=b[lines[i][0]]+b[lines[i][1]]+b[lines[i][2]];
        if (s==3) return 1;
        if (s==-3) return -1;
    }
    return 0;
}
static bool ttt_full(const int* b) {
    for(int i=0;i<9;i++) if(b[i]==0) return false;
    return true;
}
static int ttt_search(int* b, int player, int depth, int alpha, int beta,
                      int ai, int use_ab) {
    ++ttt_node_count;
    int w=ttt_winner(b);
    if(w==ai) return 10-depth;
    if(w==-ai) return depth-10;
    if(ttt_full(b)) return 0;

    if(player==ai) {
        int best=-100000;
        for(int i=0;i<9;i++) if(b[i]==0) {
            b[i]=player;
            int value=ttt_search(b,-player,depth+1,alpha,beta,ai,use_ab);
            b[i]=0;
            if(value>best) best=value;
            if(use_ab) {
                if(best>alpha) alpha=best;
                if(alpha>=beta) { ++ttt_cutoff_count; break; }
            }
        }
        return best;
    } else {
        int best=100000;
        for(int i=0;i<9;i++) if(b[i]==0) {
            b[i]=player;
            int value=ttt_search(b,-player,depth+1,alpha,beta,ai,use_ab);
            b[i]=0;
            if(value<best) best=value;
            if(use_ab) {
                if(best<beta) beta=best;
                if(alpha>=beta) { ++ttt_cutoff_count; break; }
            }
        }
        return best;
    }
}

extern "C" {
__attribute__((visibility("default"))) void ttt_clear() {
    for(int i=0;i<9;i++) ttt_board[i]=0;
    ttt_node_count=ttt_cutoff_count=ttt_last_score=0;
}
__attribute__((visibility("default"))) void ttt_set_cell(int i,int v) {
    if(i>=0 && i<9) ttt_board[i]=(v>0?1:(v<0?-1:0));
}
__attribute__((visibility("default"))) int ttt_get_cell(int i) {
    return (i>=0 && i<9)?ttt_board[i]:0;
}
__attribute__((visibility("default"))) int ttt_winner_now() { return ttt_winner(ttt_board); }
__attribute__((visibility("default"))) int ttt_is_full() { return ttt_full(ttt_board)?1:0; }
__attribute__((visibility("default"))) int ttt_best_move(int ai,int use_ab) {
    if(ai!=1 && ai!=-1) return -1;
    ttt_node_count=ttt_cutoff_count=0;
    int best=-100000, move=-1;
    for(int i=0;i<9;i++) if(ttt_board[i]==0) {
        ttt_board[i]=ai;
        int score=ttt_search(ttt_board,-ai,1,-100000,100000,ai,use_ab);
        ttt_board[i]=0;
        if(score>best) { best=score; move=i; }
    }
    ttt_last_score=best;
    return move;
}
__attribute__((visibility("default"))) int ttt_nodes() { return ttt_node_count; }
__attribute__((visibility("default"))) int ttt_cutoffs() { return ttt_cutoff_count; }
__attribute__((visibility("default"))) int ttt_score() { return ttt_last_score; }

static int c4_at(const int* b,int r,int c) { return b[r*7+c]; }
static bool c4_win(const int* b,int p) {
    for(int r=0;r<6;r++) for(int c=0;c<7;c++) if(c4_at(b,r,c)==p) {
        if(c<=3 && c4_at(b,r,c+1)==p && c4_at(b,r,c+2)==p && c4_at(b,r,c+3)==p) return true;
        if(r<=2 && c4_at(b,r+1,c)==p && c4_at(b,r+2,c)==p && c4_at(b,r+3,c)==p) return true;
        if(r<=2 && c<=3 && c4_at(b,r+1,c+1)==p && c4_at(b,r+2,c+2)==p && c4_at(b,r+3,c+3)==p) return true;
        if(r<=2 && c>=3 && c4_at(b,r+1,c-1)==p && c4_at(b,r+2,c-2)==p && c4_at(b,r+3,c-3)==p) return true;
    }
    return false;
}
static bool c4_full(const int* b) {
    for(int c=0;c<7;c++) if(b[c]==0) return false;
    return true;
}
static int c4_drop(int* b,int col,int p) {
    if(col<0 || col>=7) return -1;
    for(int r=5;r>=0;r--) if(b[r*7+col]==0) { b[r*7+col]=p; return r; }
    return -1;
}
static int c4_window_score(int a,int b,int c,int d,int ai) {
    int vals[4]={a,b,c,d}, own=0, opp=0, empty=0;
    for(int i=0;i<4;i++) { if(vals[i]==ai) own++; else if(vals[i]==-ai) opp++; else empty++; }
    if(own==4) return 100000;
    if(opp==4) return -100000;
    if(own==3 && empty==1) return 120;
    if(own==2 && empty==2) return 12;
    if(opp==3 && empty==1) return -140;
    if(opp==2 && empty==2) return -14;
    return 0;
}
static int c4_evaluate(const int* b,int ai) {
    int score=0;
    for(int r=0;r<6;r++) if(c4_at(b,r,3)==ai) score+=6;
    for(int r=0;r<6;r++) for(int c=0;c<7;c++) {
        if(c<=3) score+=c4_window_score(c4_at(b,r,c),c4_at(b,r,c+1),c4_at(b,r,c+2),c4_at(b,r,c+3),ai);
        if(r<=2) score+=c4_window_score(c4_at(b,r,c),c4_at(b,r+1,c),c4_at(b,r+2,c),c4_at(b,r+3,c),ai);
        if(r<=2 && c<=3) score+=c4_window_score(c4_at(b,r,c),c4_at(b,r+1,c+1),c4_at(b,r+2,c+2),c4_at(b,r+3,c+3),ai);
        if(r<=2 && c>=3) score+=c4_window_score(c4_at(b,r,c),c4_at(b,r+1,c-1),c4_at(b,r+2,c-2),c4_at(b,r+3,c-3),ai);
    }
    return score;
}
static int c4_search(int* b,int depth,int alpha,int beta,int player,int ai,int use_ab) {
    ++c4_node_count;
    if(c4_win(b,ai)) return 100000+depth;
    if(c4_win(b,-ai)) return -100000-depth;
    if(depth==0 || c4_full(b)) return c4_evaluate(b,ai);
    const int order[7]={3,2,4,1,5,0,6};
    if(player==ai) {
        int best=-1000000;
        for(int k=0;k<7;k++) { int col=order[k], row=c4_drop(b,col,player); if(row<0) continue;
            int value=c4_search(b,depth-1,alpha,beta,-player,ai,use_ab); b[row*7+col]=0;
            if(value>best) best=value;
            if(use_ab) { if(best>alpha) alpha=best; if(alpha>=beta) { ++c4_cutoff_count; break; } }
        }
        return best;
    } else {
        int best=1000000;
        for(int k=0;k<7;k++) { int col=order[k], row=c4_drop(b,col,player); if(row<0) continue;
            int value=c4_search(b,depth-1,alpha,beta,-player,ai,use_ab); b[row*7+col]=0;
            if(value<best) best=value;
            if(use_ab) { if(best<beta) beta=best; if(alpha>=beta) { ++c4_cutoff_count; break; } }
        }
        return best;
    }
}
__attribute__((visibility("default"))) void c4_clear() {
    for(int i=0;i<42;i++) c4_board[i]=0;
    c4_node_count=c4_cutoff_count=c4_last_score=0;
}
__attribute__((visibility("default"))) void c4_set_cell(int r,int c,int v) {
    if(r>=0 && r<6 && c>=0 && c<7) c4_board[r*7+c]=(v>0?1:(v<0?-1:0));
}
__attribute__((visibility("default"))) int c4_get_cell(int r,int c) {
    return (r>=0 && r<6 && c>=0 && c<7)?c4_board[r*7+c]:0;
}
__attribute__((visibility("default"))) int c4_winner_now() {
    if(c4_win(c4_board,1)) return 1;
    if(c4_win(c4_board,-1)) return -1;
    return 0;
}
__attribute__((visibility("default"))) int c4_is_full() { return c4_full(c4_board)?1:0; }
__attribute__((visibility("default"))) int c4_best_move(int ai,int depth,int use_ab) {
    if(ai!=1 && ai!=-1) return -1;
    if (depth < 1) depth = 1;
    if (depth > 8) depth = 8;
    c4_node_count=c4_cutoff_count=0;
    const int order[7]={3,2,4,1,5,0,6};
    int best=-1000000, move=-1;
    for(int k=0;k<7;k++) { int col=order[k], row=c4_drop(c4_board,col,ai); if(row<0) continue;
        int score=c4_search(c4_board,depth-1,-1000000,1000000,-ai,ai,use_ab);
        c4_board[row*7+col]=0;
        if(score>best) { best=score; move=col; }
    }
    c4_last_score=best;
    return move;
}
__attribute__((visibility("default"))) int c4_nodes() { return c4_node_count; }
__attribute__((visibility("default"))) int c4_cutoffs() { return c4_cutoff_count; }
__attribute__((visibility("default"))) int c4_score() { return c4_last_score; }
}
