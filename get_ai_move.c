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
    move best_move;
    eval(ai_color, 1, MAXDEPTH,-INFINITY, INFINITY, &best_move);
    return best_move;   
}

move search(int ai_color, int depth){

}