#ifndef MINESWEEPER_H
#define MINESWEEPER_H

typedef struct{
    int x_tile;
    int y_tile;
    int cover;
    int bomb;
    int flag;
}Mine;

typedef struct{
    int x_tile;
    int y_tile;
    int value;
    int cover;
    int flag;
}numTile;

typedef struct{
    SDL_Texture* NumberTexture;
    SDL_Texture* MineTexture;
    SDL_Texture* FlagTexture;
    Mine** mineary;
    numTile** numTileary;
    int numMines;
    int mouse_x;
    int mouse_y;
}GameState;

void Quit(SDL_Window* window, SDL_Renderer* renderer);

#endif