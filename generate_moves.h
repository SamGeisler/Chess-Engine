#ifndef GENERATE_MOVES_H
#define GENERATE_MOVES_H

void generate_moves(move* mempos, int* number_moves, int to_move);
void append_move(move m, move** mempos);
int is_in_check(int color_in_check);

#endif