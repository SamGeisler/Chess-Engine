#ifndef EVAL_H
#define EVAL_H

double eval(int to_move, int depth, int max_depth, double alpha, double beta, move* best_move);
double end_eval();

#endif