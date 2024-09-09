#include "render.h"
#include "global.h"
#include <stdio.h>
#include "SDL2/SDL.h"





SDL_Window* window = NULL;
SDL_Surface* screen_surface = NULL;
SDL_Surface* current_surface = NULL;
SDL_Surface* number_surface = NULL;

SDL_Surface* piece_surfaces[32];

SDL_Surface* load_surface(char* path){
    SDL_Surface* loaded = SDL_LoadBMP(path);
    SDL_Surface* optimized = SDL_ConvertSurface(loaded, screen_surface -> format, 0);
    SDL_SetColorKey(optimized, SDL_TRUE, SDL_MapRGB(optimized->format, 0x00, 0xFF, 0xFF));
    SDL_FreeSurface(loaded);
    return optimized;
}

void load_piece_surfaces(){
    piece_surfaces[9] = load_surface("img/white_pawn.bmp");
    piece_surfaces[10] = load_surface("img/white_bishop.bmp");
    piece_surfaces[11] = load_surface("img/white_knight.bmp");
    piece_surfaces[12] = load_surface("img/white_rook.bmp");
    piece_surfaces[13] = load_surface("img/white_queen.bmp");
    piece_surfaces[14] = load_surface("img/white_king.bmp");
    piece_surfaces[17] = load_surface("img/black_pawn.bmp");
    piece_surfaces[18] = load_surface("img/black_bishop.bmp");
    piece_surfaces[19] = load_surface("img/black_knight.bmp");
    piece_surfaces[20] = load_surface("img/black_rook.bmp");
    piece_surfaces[21] = load_surface("img/black_queen.bmp");
    piece_surfaces[22] = load_surface("img/black_king.bmp");
}


void draw_background(){
    SDL_FillRect(current_surface, NULL, BGCOLOR);
    SDL_Rect square = {0,0,PS,PS};
    for(int i = 0; i<8; i++){
        for(int j = 0; j<8; j++){
            square.x = (SWIDTH-PS*8)/2 + j*PS;
            square.y = (SHEIGHT - 8*PS)/2 + i*PS;
            SDL_FillRect(current_surface, &square, (i+j)%2==0 ? LIGHTSPACECOLOR : DARKSPACECOLOR );
        }
    }
    //SDL_BlitScaled(number_surface, NULL, current_surface, NULL);
}

//public
void render_init(){
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	}
    
    window = SDL_CreateWindow("Chess Display", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SWIDTH, SHEIGHT, SDL_WINDOW_SHOWN);
    if(FULLSCREEN) SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    
    screen_surface = SDL_GetWindowSurface(window);


    //number_surface = load_surface("img/board_ss.bmp");
    current_surface = load_surface("img/base.bmp");
    load_piece_surfaces();
}

//public
void render_board(int holding){
    draw_background();
    for(int i = 0; i<64; i++){
        if(board[i] && (holding==-1 || i!=holding)){
            SDL_Rect loc_rect = {XMARG + PS*(i%8), YMARG + PS*(i/8), PS, PS};
            SDL_BlitScaled(piece_surfaces[board[i]], NULL, current_surface, &loc_rect);
        }
    }
    if(holding != -1){
        int hx, hy; SDL_GetMouseState(&hx, &hy);
        SDL_Rect holding_loc_rect = {hx-PS/2, hy-PS/2, PS, PS};
        SDL_BlitScaled(piece_surfaces[board[holding]], NULL, current_surface, &holding_loc_rect);
    }

    SDL_BlitSurface(current_surface, NULL, screen_surface, NULL);
    SDL_UpdateWindowSurface(window);
}

//public
void render_update(){
    SDL_BlitSurface(current_surface, NULL, screen_surface, NULL);
    SDL_UpdateWindowSurface(window);
}

//public
void render_quit(){
    SDL_DestroyWindow(window);
    //free surfaces
    for(int i = 9; i<=14; i++){
        SDL_FreeSurface(piece_surfaces[i]);
    }
    for(int i = 17; i<=22; i++){
        SDL_FreeSurface(piece_surfaces[i]);
    }
    SDL_FreeSurface(current_surface);
    SDL_FreeSurface(screen_surface);

    SDL_Quit();
}