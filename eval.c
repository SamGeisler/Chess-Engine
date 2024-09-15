#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "global.h"
#include "control.h"
#include "eval.h"
#include "generate_moves.h"
#include "generate_moves_ccp.h"


double quiescence_search(int to_move, double alpha, double beta);

//white wants minimum score, black wants maximum score
//WHITE IS BETA BLACK IS ALPHA
double eval(int to_move, int depth, int max_depth, double alpha, double beta, move* best_move){
    Metadata backup_metadata;
    if(depth==max_depth){
        //return quiescence_search(to_move, alpha, beta);
        return quiescence_search(to_move,alpha,beta);
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

            new_eval = eval(1-to_move,depth+1,max_depth,alpha, beta, best_move);
            
            if(new_eval < optimal_eval){
                optimal_eval = new_eval;
                if(depth==1) *best_move = moves[i];
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

            new_eval = eval(1-to_move,depth+1,max_depth,alpha, beta, best_move);

            if(new_eval > optimal_eval){
                optimal_eval = new_eval;
                if(depth==1) *best_move = moves[i];
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

double end_eval(){
    double piece_count = 0;
    for(int i = 0; i<64; i++){
        if(board[i] && board[i]%8 !=6){
            piece_count += (2*((int)board[i]/8)-3) * piece_values[(int)board[i]%8-1];
        }
    }
    return piece_count;
    
}


//search all checks and captures
//WHITE IS BETA BLACK IS ALPHA
double quiescence_search(int to_move, double alpha, double beta){

    double optimal_eval = end_eval();//We assume that capturing will always give a better move than not (this assumes not in Zugzwang)
    double new_eval;

    if(!to_move){//white - minimizing (beta is best alternative)
        if(optimal_eval <= alpha){//black has a better option than the SE (which we assume as a floor), so we never go down this path
            return alpha;
        }
        if(beta > optimal_eval) beta = optimal_eval;
    } else {//black - maximizing (alpha is best alternative)
        if(optimal_eval >= beta){//white has a better option than the SE, so we will never go down this path
            return beta;
        }
        if(alpha < optimal_eval) alpha = optimal_eval;
    }




    Metadata backup_metadata;
    move* moves = (move*)malloc(220*sizeof(move)); int num_moves;
    generate_moves_CCP(moves, &num_moves,to_move);

    for(int i = 0; i<num_moves; i++){
        backup_metadata = board_md;
        execute_move(moves[i]);

        new_eval = quiescence_search(1-to_move, alpha, beta);

        if(!to_move){//white - minimizing
            if(optimal_eval > new_eval) optimal_eval = new_eval;
        } else {//black - maximizing
            if(optimal_eval < new_eval) optimal_eval = new_eval;
        }

        unexecute_move(moves[i]);
        board_md = backup_metadata;

        //AB pruning
        if(!to_move){//white - minimizing
            if(new_eval <= alpha) break;
            if(new_eval < beta) beta = new_eval;
        } else {//black - maximizing
            if(new_eval >= beta) break;
            if(new_eval > alpha) alpha = new_eval;
        }
    }

    free(moves);
    return optimal_eval;
}
