#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "global.h"
#include "control.h"
#include "eval.h"
#include "generate_moves.h"

double piece_values[] = {1.00, 3.20, 3.00, 5.00, 9.00};


//white wants minimum score, black wants maximum score
double eval(int to_move, int depth, int max_depth, double alpha, double beta){
    Metadata backup_metadata;
    if(depth==max_depth){
        return end_eval(to_move);
    }

    move* moves = (move*)malloc(220*sizeof(move)); int num_moves;
    generate_moves(moves, &num_moves,to_move);

    //the ai should prioritize checkmating earlier
    if(num_moves == 0){
        if(is_in_check(to_move)){
            free(moves);
            double depth_mod = (max_depth-depth)*0.00001;
            return (1-2*to_move)*(1000000+depth_mod);
        } else{
            free(moves);
            return 0;
        }
    }
    double optimal_eval = (1-(2*to_move)) * INFINITY;    
    double new_eval;

    if(to_move==0){//white: minimizing player
        for(int i = 0; i<num_moves; i++){
            backup_metadata = board_md;
            execute_move(moves[i]);

            new_eval = eval(1-to_move,depth+1,max_depth,alpha, beta);
            
            if(new_eval < optimal_eval){
                optimal_eval = new_eval;
            }

            unexecute_move(moves[i]);
            board_md = backup_metadata;

            if(alpha >= new_eval){//black will never go down this path, so break
                break;
            }
            if(new_eval < beta) beta = new_eval;
        }
    
    } else {//black: maximizing player
        for(int i = 0; i<num_moves; i++){
            backup_metadata = board_md;
            execute_move(moves[i]);

            new_eval = eval(1-to_move,depth+1,max_depth,alpha, beta);

            if(new_eval > optimal_eval){
                optimal_eval = new_eval;
            }

            unexecute_move(moves[i]);
            board_md = backup_metadata;

            if(beta <= new_eval){//white will never go down this path, so break
                break;
            }         

            if(new_eval > alpha) alpha = new_eval;   
        }
    }

    

    free(moves);
    return optimal_eval;
        
    
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

/*Quiescence(alpha, beta, to_move){
    generate_moves()
    int eval = + or - inf;
    for(move in moves){
        if(not is-check-or-capture){
            continue;
        } else {
            normal minimax eval w/ ab pruning
        }
    }
    if (eval = + or - inf){
        eval = static_eval();
    }
    return eval
}
*/

//search all checks and captures
double quiescence_search(int to_move, double alpha, double beta){
    move* all_moves = (move*)malloc(220*sizeof(move)); int num_moves;
    generate_moves(all_moves, &num_moves,to_move);

    double optimal_eval = (1-(2*to_move)) * INFINITY;
    double new_eval;
}