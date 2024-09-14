#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>

#include "global.h"
#include "get_ai_move.h"
#include "generate_moves.h"
#include "eval.h"
#include "control.h"



move get_ai_move(int ai_color){
    Metadata backup_metadata;
    move* moves = malloc(220*sizeof(move));
    int num_moves;
    generate_moves(moves, &num_moves, ai_color);
    if(num_moves==0){
        quit_game(1-ai_color);
    }

    move optimal_move;
    double optimal_eval = (1-(2*ai_color)) * INFINITY;

    double new_eval;
    for(int i = 0; i<num_moves; i++){
        backup_metadata = board_md;
        execute_move(moves[i]);

        new_eval = eval(1-ai_color,1,MAXDEPTH,-INFINITY,INFINITY);

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

        unexecute_move(moves[i]);
        board_md = backup_metadata;
    }

    free(moves);
    return optimal_move;   
}

move search(int ai_color, int depth){

}