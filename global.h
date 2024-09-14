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
#define MAXDEPTH 6


unsigned int board[64];
typedef struct{
    char castle_flags;//1010 - white ks, white qs, black ks, black qs (1 indicates valid)
    char ep_right;
    char fmr_count;//counts half moves
    char to_move;
    char wk_pos;
    char bk_pos;
} Metadata;

Metadata board_md;

typedef struct{
    char src;
    char dest;
    char pawn_promotion;
    char reset_fmc_flag;
    char ep_right_square;//square to initiate en passant right for. 0 if none
    char castle_update;//this is not literally correct. If a piece is moving to a rook position, execute_move will update the castling flags, even if this value is zero.
    char captured_piece;//piece id of piece captured, 0 if no capture
    char ep_flag;//flag if this move is an en passant, used for unexecute_move
} move;

int game_over;
int print_time;

int debug_flag;

void quit_game(int winning_color);
void init_board(char* board_init);

#endif