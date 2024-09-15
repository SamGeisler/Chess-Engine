#ifndef CONTROL_H
#define CONTROL_H


move get_player_move(int color);
void execute_move(move m);
void unexecute_move(move m);
int move_order_comp(const void* e1, const void* e2);

#endif