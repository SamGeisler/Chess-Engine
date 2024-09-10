#ifndef GLOBAL_H
#define GLOBAL_H
#include <stdio.h>

#define SWIDTH 1200
#define SHEIGHT 800
#define FULLSCREEN 0
#define BGCOLOR 0x222222
#define LIGHTSPACECOLOR 0xEEEEEE
#define DARKSPACECOLOR 0x4444444
#define PS 90

#define XMARG ((SWIDTH - PS*8)/2)
#define YMARG ((SHEIGHT - PS*8)/2)

#define AICOLOR 1
#define MAXDEPTH 3


unsigned int board[64];
typedef struct{
    unsigned short int castle_flags;//1010 - white ks, white qs, black ks, black qs (1 indicates valid)
    short int ep_right;
    short int fmr_count;//counts half moves
    short int to_move;
    short int wk_pos;
    short int bk_pos;
} Metadata;

Metadata board_md;

typedef struct{
    short int src;
    short int dest;
    short int pawn_promotion;
    short int reset_fmc_flag;
    short int ep_right_square;
    short int castle_update;//this is not literally correct. If a piece is moving to a rook position, execute_move will update the castling flags, even if this value is zero.
} move;

int game_over;
int print_time;

int debug_flag;

void quit_game(int winning_color);
void init_board(char* board_init);

#endif