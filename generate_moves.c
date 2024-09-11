#include <stdio.h>
#include <string.h>
#include "SDL2/SDL.h"

#include "global.h"
#include "control.h"
#include "generate_moves.h"


enum {WHITE,BLACK};

void gen_pawn_moves(int src);
void gen_bishop_moves(int src);
void gen_knight_moves(int src);
void gen_rook_moves(int src);
void gen_queen_moves(int src);
void gen_king_moves(int src);
void append_move(move m);
void try_append_move(move m);
int is_in_check(int color_in_check);

char color;
move* move_list_pos;

//public, piece-specific functions store and increment move_list_pos pointer
void generate_moves(move* mempos, int* number_moves, int to_move){
    color = to_move ? 'b' : 'w'; // get color
    move_list_pos = mempos;
    for(int src_pos = 0; src_pos<64; src_pos++){
        
        if(!board[src_pos]) continue;//No piece at intended source position
        if(board[src_pos] > 16 && color == 'w' || board[src_pos] < 16 && color == 'b') continue;//piece is of wrong color
        
        switch(board[src_pos]%8){
            case 1: gen_pawn_moves(src_pos); break;
            case 2: gen_bishop_moves(src_pos); break;
            case 3: gen_knight_moves(src_pos); break;
            case 4: gen_rook_moves(src_pos); break;
            case 5: gen_queen_moves(src_pos); break;
            case 6: gen_king_moves(src_pos); break;
        }
    }
    *number_moves = move_list_pos - mempos;
}

void gen_queen_moves(int src){
    gen_bishop_moves(src);
    gen_rook_moves(src);
}

void gen_king_moves(int src){
    int update_castle_flags = color=='w' ? 12 : 3;
    if(src>=9 && src%8!=0) try_append_move((move){src, src-9,0,0,0,update_castle_flags,0,0});
    if(src>=8) try_append_move((move){src, src-8,0,0,0,update_castle_flags,0,0});
    if(src>=8 && src%8 != 7) try_append_move((move){src, src-7,0,0,0,update_castle_flags,0,0});
    if(src%8 != 0) try_append_move((move){src, src-1,0,0,0,update_castle_flags,0,0});
    if(src%8 != 7) try_append_move((move){src, src+1,0,0,0,update_castle_flags,0,0});
    if(src<56 && src%8 != 0) try_append_move((move){src, src+7,0,0,0,update_castle_flags,0,0});
    if(src < 56) try_append_move((move){src, src+8,0,0,0,update_castle_flags,0,0});
    if(src < 56 && src%8 != 7) try_append_move((move){src, src+9,0,0,0,update_castle_flags,0,0});

    //white KS castle
    if(src==60 && board_md.castle_flags & 8 && !is_in_check(0) && !board[61] && !board[62]){
        board[60] = 0; board[61] = 14; board_md.wk_pos = 61;
        int no_c_through_check = !is_in_check(0);
        board[60] = 14; board[61] = 0; board_md.wk_pos = 60;
        if(no_c_through_check) append_move((move){60,62,0,0,0,12,0,0});
        
    }

    //white QS castle
    if(src == 60 && board_md.castle_flags & 4 && !is_in_check(0)&& !board[57] && !board[58] && !board[59]){
        board[60] = 0; board[59] = 14; board_md.wk_pos = 59;
        int no_c_through_check = !is_in_check(0);
        board[59] = 0; board[60] = 14;  board_md.wk_pos = 60;
        if(no_c_through_check) append_move((move){60,58,0,0,0,12,0,0});
        
    }

    //black KS castle
    if(src==4 && board_md.castle_flags & 2 && !is_in_check(1) && !board[5] && !board[6]){
        board[5] = 22; board[4] = 0;  board_md.bk_pos = 5;
        int no_c_through_check = !is_in_check(1);
        board[4] = 22; board[5] = 0; board_md.bk_pos = 4;
        if(no_c_through_check) append_move((move){4,6,0,0,0,3,0,0});
        
    }

    //black QS castle
    if(src==4 && board_md.castle_flags & 1 && !is_in_check(1) && !board[1] && !board[2] && !board[3]){
        board[3]  =22; board[4] = 0; board_md.bk_pos = 3;
        int no_c_through_check = !is_in_check(1);
        board[4] = 22; board[3] = 0; board_md.bk_pos = 4;
        if(no_c_through_check) append_move((move){4,2,0,0,0,3,0,0});

        
    }
}

void gen_pawn_moves(int src){
    if(color=='w'){
        //correct position for DPP
        if(src >=48 && src < 56){
            //If room for DPP, DPP
            if(!board[src-8] && !board[src-16]) append_move((move){src, src-16,0,1,src-8,0,0,0});
        }

        //En passant
        if(board_md.ep_right<24 && src>16 && (src == board_md.ep_right+7 || src==board_md.ep_right+9)){
            //avoid edge wrap-around
            if(board_md.ep_right/8 == src/8 - 1) append_move((move){src, board_md.ep_right, 0,1,0,0,0,1});
        }

        //promotion
        if(src<16){
            //push
            if(!board[src-8]){
                for(int promo_piece = 2; promo_piece <=5; promo_piece++) append_move((move){src,src-8, promo_piece, 1, 0, 0,0,0});
            }

            //capture
            if(src%8 != 7 && board[src-7] && board[src-7] > 16){
                for(int promo_piece = 2; promo_piece <= 5; promo_piece++) append_move((move){src,src-7, promo_piece, 1, 0, 0,0,0});
            }
            if(src %8 != 0 && board[src-9] && board[src-9] > 16){
                for(int promo_piece = 2; promo_piece <= 5; promo_piece++) append_move((move){src,src-9, promo_piece, 1, 0, 0,0,0});
            }
        //non promotion
        } else {
            //push
            if(!board[src-8]) append_move((move){src, src-8, 0, 1, 0, 0,0,0});

            //capture
            if( src%8!=7 && board[src-7] && board[src-7] >16) append_move((move){src,src-7, 0, 1, 0, 0,0,0});
            if( src%8 != 0 && board[src-9] && board[src-9] > 16) append_move((move){src,src-9, 0, 1, 0, 0,0,0});
        }
    } else {
        //correct position for DPP
        if(src >=8 && src < 16){
            //If room for DPP, DPP
            if(!board[src+8] && !board[src+16]){
                append_move((move){src, src+16,0,1,src+8,0,0,0});
            }
        }

        //En passant
        if(board_md.ep_right>=40 && (src == board_md.ep_right-7 || src==board_md.ep_right-9)){
            //avoid board wrap-around
            if(board_md.ep_right/8 == src/8+1) append_move((move){src, board_md.ep_right, 0,1,0,0,0,1});
        }

        //promotion
        if(src>=48){
            //push
            if(!board[src+8]){
                for(int promo_piece = 2; promo_piece <=5; promo_piece++) append_move((move){src,src+8, promo_piece, 1, 0, 0,0,0});
            }
            //capture
            if(src%8 != 0 && board[src+7] && board[src+7]<16){
                for(int promo_piece = 2; promo_piece <= 5; promo_piece++) append_move((move){src,src+7, promo_piece, 1, 0, 0,0,0});
            }
            if(src%8 != 7 && board[src+9] && board[src+9]<16){
                for(int promo_piece = 2; promo_piece <= 5; promo_piece++) append_move((move){src,src+9, promo_piece, 1, 0, 0,0,0});
            }
        //non-promotion
        } else {
            //push
            if(!board[src+8]) append_move((move){src, src+8, 0, 1, 0, 0,0,0});

            //capture
            if(src%8 != 0 && board[src+7] && board[src+7]<16) append_move((move){src,src+7, 0, 1, 0, 0,0,0});
            if(src%8 != 7 && board[src+9] && board[src+9]<16) append_move((move){src,src+9, 0, 1, 0, 0,0,0});
        }
    }
}

void gen_bishop_moves(int src){
    if(src%8!=0){
        for(int i = src-9; ; i-=9){
            if(i<0) break;
            if(board[i]){
                if(board[i] > 16 && color == 'w') append_move((move){src, i, 0, 1, 0, 0,0,0});
                else if(board[i] < 16 && color == 'b') append_move((move){src, i, 0, 1, 0, 0,0,0});
                break;
            }
            append_move((move){src,i,0,0,0,0,0,0});
            if(i%8==0) break;
        }
    }
    if(src%8!=7){
        for(int i = src+9; ; i+=9){
            if(i>63) break;
            if(board[i]){
                if(board[i] > 16 && color == 'w') append_move((move){src, i, 0, 1, 0, 0,0,0});
                else if(board[i] < 16 && color == 'b') append_move((move){src, i, 0, 1, 0, 0,0,0});
                break;
            }
            append_move((move){src,i,0,0,0,0,0,0});
            if(i%8==7) break;
        }
    }
    if(src%8!=7){
        for(int i = src-7; ; i-=7){
            if(i<0) break;
            if(board[i]){
                if(board[i] > 16 && color == 'w') append_move((move){src, i, 0, 1, 0, 0,0,0});
                else if(board[i] < 16 && color == 'b') append_move((move){src, i, 0, 1, 0, 0,0,0});
                break;
            }
            append_move((move){src,i,0,0,0,0,0,0});
            if(i%8==7) break;
        }
    }
    if(src%8!=0){
        for(int i = src+7; ; i+=7){
            if(i>63) break;
            if(board[i]){
                if(board[i] > 16 && color == 'w') append_move((move){src, i, 0, 1, 0, 0,0,0});
                else if(board[i] < 16 && color == 'b') append_move((move){src, i, 0, 1, 0, 0,0,0});
                break;
            }
            append_move((move){src,i,0,0,0,0,0,0});
            if(i%8==0) break;
        }
    }
}

void gen_knight_moves(int src){
    if(src>=16 && src%8>0) try_append_move((move){src, src-17,0,0,0,0,0,0});
    if(src>=16 && src%8<7) try_append_move((move){src, src-15,0,0,0,0,0,0});
    if(src>=8 && src%8>1) try_append_move((move){src, src-10,0,0,0,0,0,0});
    if(src>=8 && src%8<6) try_append_move((move){src, src-6,0,0,0,0,0,0});
    if(src<56 && src%8>1) try_append_move((move){src, src+6,0,0,0,0,0,0});
    if(src<56 && src%8<6) try_append_move((move){src, src+10,0,0,0,0,0,0});
    if(src<48 && src%8>0) try_append_move((move){src, src+15,0,0,0,0,0,0});
    if(src<48 && src%8<7) try_append_move((move){src, src+17,0,0,0,0,0,0});
}

void gen_rook_moves(int src){
    for(int i = src-8; i>-1; i-=8){
        if(board[i]){
            try_append_move((move){src, i, 0, 0, 0, 0,0,0});
            break;
        }
        append_move((move){src, i, 0, 0, 0, 0,0,0});
    }
    for(int i = src+8; i<64; i+=8){
        if(board[i]){
            try_append_move((move){src, i, 0, 0, 0, 0,0,0});
            break;
        }
        append_move((move){src, i, 0, 0, 0, 0,0,0});
    }
    for(int i = src-1; i>=0 && i/8==src/8; i--){
        if(board[i]){
            try_append_move((move){src, i, 0, 0, 0, 0,0,0});
            break;
        }
        append_move((move){src, i, 0, 0, 0, 0,0,0});
    }
    for(int i = src+1; i/8==src/8; i++){
        if(board[i]){
            try_append_move((move){src, i, 0, 0, 0, 0,0,0});
            break;
        }
        append_move((move){src, i, 0, 0, 0, 0,0,0});
    }
}

//append_move if the destination square is not occupied by the same color
void try_append_move(move m){
    if(board[m.dest] > 16 && color == 'b') return;
    if(board[m.dest] && board[m.dest] < 16 && color == 'w') return;
    append_move(m);
}

void append_move(move m){
    m.captured_piece = board[m.dest];

    *move_list_pos = m;
    move_list_pos++;

    Metadata bup_md = board_md;
    execute_move(m);



    //printf("checking %d to %d, c==b: %d\n",m.src,m.dest,color=='b');
    if(is_in_check(color=='b')){
        //printf("Move %d to %d puts %c in check!\n",m.src, m.dest,color);
        move_list_pos--;
    }

    unexecute_move(m);

    board_md = bup_md;
}

int is_in_check(int color_in_check){
    int king_pos = color_in_check ? board_md.bk_pos : board_md.wk_pos;

    //check surroundings (adjacent king)
    int oki = (1-color_in_check)*8+14; //opposite king ID
    if(king_pos>=9 && king_pos%8!=0){
        if(board[king_pos-9]==oki) return 1;
    }
    if(king_pos>=8){
        if(board[king_pos-8]==oki) return 1;
    }
    if(king_pos>=8 && king_pos%8 != 7){
        if(board[king_pos-7]==oki) return 1;
    }
    if(king_pos%8 != 0){
        if(board[king_pos-1]==oki) return 1;
    }
    if(king_pos%8 != 7){
        if(board[king_pos+1]==oki) return 1;
    }
    if(king_pos<56 && king_pos%8 != 0){
        if(board[king_pos+7]==oki) return 1;
    }
    if(king_pos < 56){
        if(board[king_pos+8]==oki) return 1;
    }
    if(king_pos < 56 && king_pos%8 != 7){
        if(board[king_pos+9] == oki) return 1;
    }

    //check knight positions
    int oni = (1-color_in_check)*8 + 11; //opposite knight ID
    if(king_pos>=16 && king_pos%8>0){
        if(board[king_pos-17]==oni) return 1;
    }
    if(king_pos>=16 && king_pos%8<7){
        if(board[king_pos-15]==oni) return 1;
    }
    if(king_pos>=8 && king_pos%8>1){
        if(board[king_pos-10]==oni) return 1;
    }
    if(king_pos>=8 && king_pos%8<6){
        if(board[king_pos-6]==oni) return 1;
    }
    if(king_pos<56 && king_pos%8>1){
        if(board[king_pos+6]==oni) return 1;
    }
    if(king_pos<56 && king_pos%8<6){
        if(board[king_pos+10]==oni) return 1;
    }
    if(king_pos<48 && king_pos%8>0){
        if(board[king_pos+15]==oni) return 1;
    }
    if(king_pos<48 && king_pos%8<7){
        if(board[king_pos+17]==oni) return 1;
    }

    //check diagonals
    if(king_pos%8!=0){
        for(int i = king_pos-9; i>=0; i-=9){
            if(board[i]==(1-color_in_check)*8 + 10 || board[i]==(1-color_in_check)*8 + 13){return 1;}
            if(board[i]) break;
            if(i%8==0) break;
        }
    }
    if(king_pos%8!=7){
        for(int i = king_pos+9; i<64; i+=9){
            if(board[i]==(1-color_in_check)*8 + 10 || board[i]==(1-color_in_check)*8 + 13){return 1;}
            if(board[i]) break;
            if(i%8==7) break;
        }
    }
    if(king_pos%8!=7){
        for(int i = king_pos-7; i>=0; i-=7){
            if(board[i]==(1-color_in_check)*8 + 10 || board[i]==(1-color_in_check)*8 + 13){return 1;}
            if(board[i]) break;
            if(i%8==7) break;
        }
    }
    if(king_pos%8!=0)
    for(int i = king_pos+7; i<64; i+=7){
        if(board[i]==(1-color_in_check)*8 + 10 || board[i]==(1-color_in_check)*8 + 13){return 1;}
        if(board[i]) break;
        if(i%8==0) break;
    }

    //check rank & file
    for(int i = king_pos-8; i>=0; i-=8){
        if(board[i]==(1-color_in_check)*8 + 12 || board[i]==(1-color_in_check)*8 + 13) return 1;
        if(board[i]) break;
    }
    for(int i = king_pos+8; i<64; i+=8){
        if(board[i]==(1-color_in_check)*8 + 12 || board[i]==(1-color_in_check)*8 + 13) return 1;
        if(board[i]) break;
    }
    for(int i = king_pos-1; i/8==king_pos/8; i--){
        if(board[i]==(1-color_in_check)*8 + 12 || board[i]==(1-color_in_check)*8 + 13) return 1;
        if(board[i]) break;
    }
    for(int i = king_pos+1; i/8==king_pos/8; i++){
        if(board[i]==(1-color_in_check)*8 + 12 || board[i]==(1-color_in_check)*8 + 13) return 1;
        if(board[i]) break;
    }

    //pawn checks TODO: FIX!
    if(!color_in_check){ //white king
        if(king_pos>8 && king_pos%8!=0){
            if(board[king_pos-9]==17) return 1;
        }
        if(king_pos %8!=7 && king_pos>=8){
            if(board[king_pos-7]==17) return 1;
        }
    } else { //black king
        if(king_pos < 56 && king_pos%8!=0){
            if(board[king_pos+7]==9) return 1;
        }
        if(king_pos<56 && king_pos %8!=7){
            if(board[king_pos+9]==9){
                return 1;
            }
        }
    }

    return 0;
}