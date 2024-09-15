#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "control.h"
#include "generate_moves.h"
#include "generate_moves_CCP.h"


enum {WHITE,BLACK};

void pawn_CCP(int src);
void bishop_CCP(int src);
void knight_CCP(int src);
void rook_CCP(int src);
void queen_CCP(int src);
void king_CCP(int src);

char color;
move* move_list_pos;


void generate_moves_CCP(move* mempos, int* number_moves, int to_move){
    color = to_move ? 'b' : 'w'; // get color
    move_list_pos = mempos;
    for(int src_pos = 0; src_pos<64; src_pos++){
        
        if(!board[src_pos]) continue;//No piece at intended source position
        if(board[src_pos] > 16 && color == 'w' || board[src_pos] < 16 && color == 'b') continue;//piece is of wrong color
        
        switch(board[src_pos]%8){
            case 1: pawn_CCP(src_pos); break;
            case 2: bishop_CCP(src_pos); break;
            case 3: knight_CCP(src_pos); break;
            case 4: rook_CCP(src_pos); break;
            case 5: queen_CCP(src_pos); break;
            case 6: king_CCP(src_pos); break;
        }
    }
    
    *number_moves = move_list_pos - mempos;
    qsort(mempos,*number_moves,sizeof(move),move_order_comp);

}

void pawn_CCP(int src){
    if(color=='w'){
        //En passant
        if(board_md.ep_right<24 && src>16 && (src == board_md.ep_right+7 || src==board_md.ep_right+9)){
            //avoid edge wrap-around
            if(board_md.ep_right/8 == src/8 - 1) append_move((move){src, board_md.ep_right, 0,1,0,0,0,1},&move_list_pos);
        }

        //promotion
        if(src<16){
            //push
            if(!board[src-8]){
                for(int promo_piece = 2; promo_piece <=5; promo_piece++) append_move((move){src,src-8, promo_piece, 1, 0, 0,0,0},&move_list_pos);
            }

            //capture
            if(src%8 != 7 && board[src-7] && board[src-7] > 16){
                for(int promo_piece = 2; promo_piece <= 5; promo_piece++) append_move((move){src,src-7, promo_piece, 1, 0, 0,0,0},&move_list_pos);
            }
            if(src %8 != 0 && board[src-9] && board[src-9] > 16){
                for(int promo_piece = 2; promo_piece <= 5; promo_piece++) append_move((move){src,src-9, promo_piece, 1, 0, 0,0,0},&move_list_pos);
            }
        //non promotion
        } else {
            //capture
            if( src%8!=7 && board[src-7] && board[src-7] >16) append_move((move){src,src-7, 0, 1, 0, 0,0,0},&move_list_pos);
            if( src%8 != 0 && board[src-9] && board[src-9] > 16) append_move((move){src,src-9, 0, 1, 0, 0,0,0},&move_list_pos);
        }
    } else {
        //En passant
        if(board_md.ep_right>=40 && (src == board_md.ep_right-7 || src==board_md.ep_right-9)){
            //avoid board wrap-around
            if(board_md.ep_right/8 == src/8+1) append_move((move){src, board_md.ep_right, 0,1,0,0,0,1},&move_list_pos);
        }

        //promotion
        if(src>=48){
            //push
            if(!board[src+8]){
                for(int promo_piece = 2; promo_piece <=5; promo_piece++) append_move((move){src,src+8, promo_piece, 1, 0, 0,0,0},&move_list_pos);
            }
            //capture
            if(src%8 != 0 && board[src+7] && board[src+7]<16){
                for(int promo_piece = 2; promo_piece <= 5; promo_piece++) append_move((move){src,src+7, promo_piece, 1, 0, 0,0,0},&move_list_pos);
            }
            if(src%8 != 7 && board[src+9] && board[src+9]<16){
                for(int promo_piece = 2; promo_piece <= 5; promo_piece++) append_move((move){src,src+9, promo_piece, 1, 0, 0,0,0},&move_list_pos);
            }
        //non-promotion
        } else {
            //capture
            if(src%8 != 0 && board[src+7] && board[src+7]<16) append_move((move){src,src+7, 0, 1, 0, 0,0,0},&move_list_pos);
            if(src%8 != 7 && board[src+9] && board[src+9]<16) append_move((move){src,src+9, 0, 1, 0, 0,0,0},&move_list_pos);
        }
    }
}

void bishop_CCP(int src){
    if(src%8!=0){
        for(int i = src-9; ; i-=9){
            if(i<0) break;
            if(board[i]){
                if(board[i] > 16 && color == 'w') append_move((move){src, i, 0, 1, 0, 0,0,0},&move_list_pos);
                else if(board[i] < 16 && color == 'b') append_move((move){src, i, 0, 1, 0, 0,0,0},&move_list_pos);
                break;
            }
            if(i%8==0) break;
        }
    }
    if(src%8!=7){
        for(int i = src+9; ; i+=9){
            if(i>63) break;
            if(board[i]){
                if(board[i] > 16 && color == 'w') append_move((move){src, i, 0, 1, 0, 0,0,0},&move_list_pos);
                else if(board[i] < 16 && color == 'b') append_move((move){src, i, 0, 1, 0, 0,0,0},&move_list_pos);
                break;
            }
            if(i%8==7) break;
        }
    }
    if(src%8!=7){
        for(int i = src-7; ; i-=7){
            if(i<0) break;
            if(board[i]){
                if(board[i] > 16 && color == 'w') append_move((move){src, i, 0, 1, 0, 0,0,0},&move_list_pos);
                else if(board[i] < 16 && color == 'b') append_move((move){src, i, 0, 1, 0, 0,0,0},&move_list_pos);
                break;
            }
            if(i%8==7) break;
        }
    }
    if(src%8!=0){
        for(int i = src+7; ; i+=7){
            if(i>63) break;
            if(board[i]){
                if(board[i] > 16 && color == 'w') append_move((move){src, i, 0, 1, 0, 0,0,0},&move_list_pos);
                else if(board[i] < 16 && color == 'b') append_move((move){src, i, 0, 1, 0, 0,0,0},&move_list_pos);
                break;
            }
            if(i%8==0) break;
        }
    }
};

void knight_CCP(int src){
    if(color=='w'){
        if(src>=16 && src%8>0 && board[src-17]>16) append_move((move){src, src-17,0,0,0,0,0,0},&move_list_pos);
        if(src>=16 && src%8<7 && board[src-15]>16) append_move((move){src, src-15,0,0,0,0,0,0},&move_list_pos);
        if(src>=8 && src%8>1 && board[src-10]>16) append_move((move){src, src-10,0,0,0,0,0,0},&move_list_pos);
        if(src>=8 && src%8<6 && board[src-6]>16) append_move((move){src, src-6,0,0,0,0,0,0},&move_list_pos);
        if(src<56 && src%8>1 && board[src+6]>16) append_move((move){src, src+6,0,0,0,0,0,0},&move_list_pos);
        if(src<56 && src%8<6 && board[src+10]>16) append_move((move){src, src+10,0,0,0,0,0,0},&move_list_pos);
        if(src<48 && src%8>0 && board[src+15]>16) append_move((move){src, src+15,0,0,0,0,0,0},&move_list_pos);
        if(src<48 && src%8<7 && board[src+17]>16) append_move((move){src, src+17,0,0,0,0,0,0},&move_list_pos);
    } else {
        if(src>=16 && src%8>0 && board[src-17] && board[src-17] < 16) append_move((move){src, src-17,0,0,0,0,0,0},&move_list_pos);
        if(src>=16 && src%8<7 && board[src-15] && board[src-15] < 16) append_move((move){src, src-15,0,0,0,0,0,0},&move_list_pos);
        if(src>=8 && src%8>1 && board[src-10] && board[src-10] < 16) append_move((move){src, src-10,0,0,0,0,0,0},&move_list_pos);
        if(src>=8 && src%8<6 && board[src-6] &&  board[src-6] < 16) append_move((move){src, src-6,0,0,0,0,0,0},&move_list_pos);
        if(src<56 && src%8>1 && board[src+6] &&  board[src+6] < 16) append_move((move){src, src+6,0,0,0,0,0,0},&move_list_pos);
        if(src<56 && src%8<6 && board[src+10] && board[src+10] < 16) append_move((move){src, src+10,0,0,0,0,0,0},&move_list_pos);
        if(src<48 && src%8>0 && board[src+15] && board[src+15] < 16) append_move((move){src, src+15,0,0,0,0,0,0},&move_list_pos);
        if(src<48 && src%8<7 && board[src+17] && board[src+17] < 16) append_move((move){src, src+17,0,0,0,0,0,0},&move_list_pos);
    }
}

void rook_CCP(int src){
    for(int i = src-8; i>-1; i-=8){
        if(color == 'w' && board[i] > 16 || color == 'b' && board[i] && board[i] < 16){
            append_move((move){src, i, 0, 0, 0, 0,0,0},&move_list_pos);
            break;
        }
    }
    for(int i = src+8; i<64; i+=8){
        if(color == 'w' && board[i] > 16 || color == 'b' && board[i] && board[i] < 16){
            append_move((move){src, i, 0, 0, 0, 0,0,0},&move_list_pos);
            break;
        }
    }
    for(int i = src-1; i>=0 && i/8==src/8; i--){
        if(color == 'w' && board[i] > 16 || color == 'b' && board[i] && board[i] < 16){
            append_move((move){src, i, 0, 0, 0, 0,0,0},&move_list_pos);
            break;
        }
    }
    for(int i = src+1; i/8==src/8; i++){
        if(color == 'w' && board[i] > 16 || color == 'b' && board[i] && board[i] < 16){
            append_move((move){src, i, 0, 0, 0, 0,0,0},&move_list_pos);
            break;
        }
    }
};

void queen_CCP(int src){
    rook_CCP(src);
    bishop_CCP(src);
}

void king_CCP(int src){
    int update_castle_flags = color=='w' ? 12 : 3;
    if(color == 'w'){
        if(src>=9 && src%8!=0 && board[src-9] > 16) append_move((move){src, src-9,0,0,0,update_castle_flags,0,0},&move_list_pos);
        if(src>=8 && board[src-8] > 16) append_move((move){src, src-8,0,0,0,update_castle_flags,0,0},&move_list_pos);
        if(src>=8 && src%8 != 7 && board[src-7] > 16) append_move((move){src, src-7,0,0,0,update_castle_flags,0,0},&move_list_pos);
        if(src%8 != 0 && board[src-1] > 16) append_move((move){src, src-1,0,0,0,update_castle_flags,0,0},&move_list_pos);
        if(src%8 != 7 && board[src+1] > 16) append_move((move){src, src+1,0,0,0,update_castle_flags,0,0},&move_list_pos);
        if(src<56 && src%8 != 0 && board[src+7] > 16) append_move((move){src, src+7,0,0,0,update_castle_flags,0,0},&move_list_pos);
        if(src < 56 && board[src+8] > 16) append_move((move){src, src+8,0,0,0,update_castle_flags,0,0},&move_list_pos);
        if(src < 56 && src%8 != 7 && board[src+9] > 16) append_move((move){src, src+9,0,0,0,update_castle_flags,0,0},&move_list_pos);
    } else {
        if(src>=9 && src%8!=0 && board[src-9] && board[src-9] < 16) append_move((move){src, src-9,0,0,0,update_castle_flags,0,0},&move_list_pos);
        if(src>=8 && board[src-8] && board[src-8] < 16) append_move((move){src, src-8,0,0,0,update_castle_flags,0,0},&move_list_pos);
        if(src>=8 && src%8 != 7 && board[src-7] && board[src-7] < 16) append_move((move){src, src-7,0,0,0,update_castle_flags,0,0},&move_list_pos);
        if(src%8 != 0 && board[src-1] && board[src-1] < 16) append_move((move){src, src-1,0,0,0,update_castle_flags,0,0},&move_list_pos);
        if(src%8 != 7 && board[src+1] && board[src+1] < 16) append_move((move){src, src+1,0,0,0,update_castle_flags,0,0},&move_list_pos);
        if(src<56 && src%8 != 0 && board[src+7] && board[src+7] < 16) append_move((move){src, src+7,0,0,0,update_castle_flags,0,0},&move_list_pos);
        if(src < 56 && board[src+8] && board[src+8] < 16) append_move((move){src, src+8,0,0,0,update_castle_flags,0,0},&move_list_pos);
        if(src < 56 && src%8 != 7 && board[src+9] && board[src+9] < 16) append_move((move){src, src+9,0,0,0,update_castle_flags,0,0},&move_list_pos);
    }
};