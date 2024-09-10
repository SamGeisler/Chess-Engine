#include <stdio.h>
#include "SDL2/SDL.h"

#include "global.h"
#include "control.h"
#include "render.h"
#include "generate_moves.h"

//private
void print_moves(move* loc, int num);

//public 
move get_player_move(int color){
    move* legal_moves = malloc(220*sizeof(move)); int num_legal_moves;
    generate_moves(legal_moves, &num_legal_moves, color);
    if(num_legal_moves==0){
        quit_game(AICOLOR);
    }
    //print_moves(legal_moves,num_legal_moves);
    int src_pos, dest_pos;
    int holding = -1;
    move null_move = (move){0,0,0,0,0,0};
    move move_found = (move){-1,0,0,0,0,0};
    
    while(move_found.src == -1){
        render_board(holding);
        SDL_Event e;
        while( SDL_PollEvent(&e) ){
            int mx, my; SDL_GetMouseState(&mx, &my);

            //user quits window
            if(e.type == SDL_QUIT|| e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE){
                render_quit();
                exit(1);
            //click
            } else if(e.type == SDL_MOUSEBUTTONDOWN){
                if(holding != -1) continue;
                holding = 8 * ((my - YMARG)/PS)  +  ( (mx - XMARG)/PS );
                if(AICOLOR == 0) holding = 63-holding;

            //release mouse
            } else if (e.type==SDL_MOUSEBUTTONUP){
                if(holding == -1) continue;
                
                //mouse in range
                if(mx > XMARG && mx < XMARG + 8*PS && my > YMARG && my < YMARG + 8*PS){
                    int newpos = 8 * ((my - YMARG)/PS)  +  ( (mx - XMARG)/PS );
                    if(AICOLOR == 0) newpos = 63-newpos;

                    //user didn't just drop the piece down in the same square
                    if(holding != newpos){
                        src_pos = holding;
                        dest_pos = newpos;
                        char promo_char = '\n'; int promo_num = 0;
                        if(board[src_pos]%8==1 && (dest_pos < 8 || dest_pos >= 56)){
                            printf("Enter piece to promote to (b, k, r, q): ");
                            while(promo_char == '\n') scanf("%c", &promo_char);
                            switch(promo_char){
                                case 'b': promo_num = 2; break;
                                case 'k': promo_num = 3; break;
                                case 'r': promo_num = 4; break;
                                case 'q': promo_num = 5; break;
                            }
                        }
                        
                        for(int n= 0; n<num_legal_moves; n++){
                            if(legal_moves[n].src==src_pos && legal_moves[n].dest == dest_pos && legal_moves[n].pawn_promotion==promo_num){
                                move_found = legal_moves[n];
                                break;
                            }
                        }
                        holding = -1;
                        if(move_found.src == -1){
                            printf("%d to %d, promo %d: Illegal move!\n",src_pos, dest_pos, promo_num);
                        }
                    } else {
                        holding = -1;
                    }
                } else {
                    holding = -1;
                }
            }
        }
        
    }
    
    free(legal_moves);
    return move_found;
}

void print_moves(move* loc, int num){
    for(int i = 0; i<num; i++){
        printf("move from %d to %d.\n", loc[i].src, loc[i].dest);
    }
}



void execute_move(move m){
    if(debug_flag) printf("1");
    //update king position metadata
    if(board[m.src]==14){
        board_md.wk_pos = m.dest;
    } else if(board[m.src]==22){
        board_md.bk_pos = m.dest;
    }



    //check if promoting
    if(!m.pawn_promotion){
        board[m.dest] = board[m.src];
    } else {
        board[m.dest] = (board[m.src]/8)*8 + m.pawn_promotion;
    }


    //enpassant
    if(board[m.src]%8==1 && board_md.ep_right!= 0 && m.dest==board_md.ep_right){
        //white taking
        if(board[m.src]<16){
            board[m.dest+8]=0;
        } else {
            board[m.dest-8]=0;
        }
    } 
    
    //update castle flags according to move data
    board_md.castle_flags = board_md.castle_flags & ~m.castle_update;

    //update castle flags for non-king move
    if(m.src==0 || m.dest==0) board_md.castle_flags &= ~1;
    if(m.src==7 || m.dest==7) board_md.castle_flags &= ~2;
    if(m.src==56 || m.dest==56) board_md.castle_flags &= ~4;
    if(m.src==63 || m.dest==63) board_md.castle_flags &= ~8;


    //update en passant right according to move data
    board_md.ep_right = m.ep_right_square;
    
    //update of revert fifty move rule counter
    if(m.reset_fmc_flag) board_md.fmr_count = 0;
    else board_md.fmr_count++;
    //clear source position
    board[m.src] = 0;

    

    //move rook for castle_moves
    if(board[m.dest]%8==6){
        if(m.src==60 && m.dest==62){ 
            board[61] = 12;
            board[63] = 0;
        } else if(m.src==60 && m.dest==58){
            board[59] = 12;
            board[56] = 0;
        } else if(m.src==4 && m.dest==6){
            board[5] = 20;
            board[7] = 0;
        }else if(m.src==4 && m.dest ==2){
            board[3] = 20;
            board[0] = 0;
        }
    }
    
    //board_md.fmr_count >= 100, game_over (check for checkmate)
    board_md.to_move = 1-board_md.to_move;
}