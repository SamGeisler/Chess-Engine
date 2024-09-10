#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "global.h"
#include "control.h"
#include "eval.h"
#include "generate_moves.h"

double piece_values[] = {1.00, 3.20, 3.00, 5.00, 9.00};

//white wants minimum score, black wants maximum score
double eval(int to_move, int depth, int max_depth){
    move* moves = (move*)malloc(220*sizeof(move)); int num_moves;
    generate_moves(moves, &num_moves,to_move);

    

    //the ai should prioritize checkmating earlier
    if(num_moves == 0){
        if(is_in_check(to_move)){
            free(moves);
            double depth_mod = (max_depth-depth)*0.00001;
            return (1-2*to_move)*(1000000.0+depth_mod);
        } else{
            free(moves);
            return 0;
        }
    } else {
        if(depth==max_depth){
            free(moves);
            return end_eval(to_move);
        } else{
            move optimal_move;
            double optimal_eval = (1-(2*to_move)) * 1000000.0;

            int bup_board[64];
            Metadata bup_md;
            double new_eval;
            for(int i = 0; i<num_moves; i++){
                memcpy(bup_board,board,64*sizeof(int));
                bup_md = board_md;

                execute_move(moves[i]);
                new_eval = eval(1-to_move,depth+1,max_depth);


                if(to_move==0){//white
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
            }
            free(moves);
            return optimal_eval;
        }
    }
}

double end_eval(int to_move){
    double piece_count = 0;
    for(int i = 0; i<64; i++){
        if(board[i] && board[i]%8 !=6){
            int dir = 2*(board[i]/8)-3;
            piece_count += dir * piece_values[board[i]%8-1];
        }
    }
    return piece_count;
    
}