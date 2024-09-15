#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "SDL2/SDL.h"

#include "global.h"
#include "render.h"
#include "control.h"
#include "generate_moves.h"
#include "get_ai_move.h"
#include "perft.h"

char* address_to_coords(int a);
enum {WHITE,BLACK};


int main(int argc, char* argv[]){
    piece_values[0] = 1.00; 
    piece_values[1] = 3.20;
    piece_values[2] = 3.00;
    piece_values[3] = 5.00;
    piece_values[4] = 9.00;
    debug_flag = 0;
    srand(time(NULL));


    //initialize board array & metadata (initiali pos: rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1)
    init_board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");

    //initialize rendering
    render_init();
    render_board(-1);
    int quit = 0;
    if(AICOLOR==WHITE){
        execute_move(get_ai_move(AICOLOR));
        render_board(-1);
    }
    while(!quit){
        execute_move(get_player_move(1-AICOLOR));
        render_board(-1);
        

        execute_move(get_ai_move(AICOLOR));
        render_board(-1);

    }
    render_quit();

    return 0;
}



/*
bitfield: piece = color + type
    p = 1
    b = 2
    n = 3
    r = 4
    q = 5
    k = 6

    white = 8
    black = 16

eg. 01 101 = white queen
*/
void init_board(char* board_init){
    for(int i = 0;i<64; i++) board[i] = 0;
    int i = 0;
    int pos = 0;
    while(board_init[i]!=' '){
        // piece
        if(board_init[i] <=122 && board_init[i] >=65){
            char p = board_init[i];
            switch (p){
                case 'K': board[pos] = 14; board_md.wk_pos = pos; break;
                case 'Q': board[pos] = 13; break;
                case 'R': board[pos] = 12; break;
                case 'N': board[pos] = 11; break;
                case 'B': board[pos] = 10; break;
                case 'P': board[pos] = 9; break;
                case 'k': board[pos] = 22; board_md.bk_pos = pos; break;
                case 'q': board[pos] = 21; break;
                case 'r': board[pos] = 20; break;
                case 'n': board[pos] = 19; break;
                case 'b': board[pos] = 18; break;
                case 'p': board[pos] = 17; break;
            }
            pos++;
        }   
        //number
        else if(board_init[i] <= 57 && board_init[i] >= 48){
            pos += board_init[i] - '0';
        }

        i++;
    }

    i++;
    //player to move
    board_md.to_move = board_init[i]=='b';

    i+=2;
    //castle rights
    board_md.castle_flags = 0;
    while(board_init[i]!=' '){
        if(board_init[i]=='K'){
            board_md.castle_flags += 8;
        } else if(board_init[i]=='Q'){
            board_md.castle_flags += 4;
        } else if(board_init[i]=='k'){
            board_md.castle_flags += 2;
        } else if(board_init[i]=='q'){
            board_md.castle_flags += 1;
        }
        i++;
    }
    i++;
    //en passant target square
    if(board_init[i]!='-'){
        board_md.ep_right += board_init[i]-'a';
        i++;
        board_md.ep_right += (8-board_init[i])*8;
    } else {
        board_md.ep_right = 0;
    }
    //fmr counter (ignore for now)
    //total move count (ignore for now)
    
    game_over = 0;
}

void quit_game(int winning_color){
    if(is_in_check(1-winning_color)){
        printf("Game over! %s wins.\n", winning_color ? "black" : "white");
    } else {
        printf("Stalemate!\n");
    }
    
    char c; scanf("%c", &c);
    render_quit();
    exit(0);
}

char* address_to_coords(int a){
    static char rv[] = "  ";
    rv[0] = 'a' + a%8;
    rv[1] = '8'- a/8;
    return rv;
}

void print_board(){
    for(int i = 0; i<64; i++){
        char c;
        switch(board[i]){
            case 9: c = 'P'; break;
            case 10: c = 'B'; break;
            case 11: c = 'N'; break;
            case 12: c = 'R'; break;
            case 13: c = 'Q'; break;
            case 14: c = 'K'; break;
            case 17: c = 'p'; break;
            case 18: c = 'b'; break;
            case 19: c = 'n'; break;
            case 20: c = 'r'; break;
            case 21: c = 'q'; break;
            case 22: c = 'k'; break;
            case 0: c = '-'; break;
            default: c = '!';
        }
        printf("%c ",c);
        if(i%8==7) printf("\n");
    }
}