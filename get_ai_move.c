#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "global.h"
#include "get_ai_move.h"
#include "generate_moves.h"
#include "eval.h"
#include "control.h"

move get_ai_move(int ai_color){
    move* moves = malloc(220*sizeof(move));
    int num_moves;
    generate_moves(moves, &num_moves, ai_color);
    if(num_moves==0){
        quit_game(1-ai_color);
    } else {
        move optimal_move;
        double optimal_eval = 1-(2*ai_color) * 1000000.0;

        int bup_board[64];
        Metadata bup_md;
        double new_eval;
        for(int i = 0; i<num_moves; i++){
            if(moves[i].src==9 && moves[i].dest == 36){
                print_time = 0;
            }
            memcpy(bup_board,board,64*sizeof(int));
            bup_md = board_md;

            execute_move(moves[i]);
            new_eval = eval(1-ai_color,1,MAXDEPTH);

            if(ai_color==0){//white
                if(new_eval < optimal_eval){
                    optimal_move = moves[i];
                    optimal_eval = new_eval;
                }
            } else {//black
                if(new_eval > optimal_eval){
                    optimal_move = moves[i];
                    optimal_eval = new_eval;
                }
            }
            memcpy(board,bup_board,64*sizeof(int));
            board_md = bup_md;
            print_time = 0;
        }
        free(moves);
        return optimal_move;   
    }
}