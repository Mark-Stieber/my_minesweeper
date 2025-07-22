#include <stdio.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_render.h>
#include <minesweeper.h>
#include <stdbool.h>

// Tile Spacing
#define SPACING 32

///____Changeable_Values_____///

// Number of Tiles in X and Y
#define GAMELENX 16
#define GAMELENY 16

// Number of Mines
#define NUMMINES 40

///____Changeable_Values_____///


#define SCREEN_WIDTH GAMELENX*SPACING
#define SCREEN_HEIGHT GAMELENY*SPACING



SDL_Texture* Load_Texture(SDL_Renderer* renderer, SDL_Window* window, char* filename){
    SDL_Surface *surface;

    surface = IMG_Load(filename);

    if(surface == NULL){
        printf("Cannot Find filename\n");
        Quit(window,renderer);
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    return texture;
}

void Texture_Display(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, int w, int h){
    SDL_Rect dest = {x,y,w,h};

    SDL_RenderCopy(renderer,texture,NULL,&dest);
}

void Texture_Free(SDL_Texture* texture){
    SDL_DestroyTexture(texture);
}

void Quit(SDL_Window* window, SDL_Renderer* renderer){

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

// Goes through numTileary to find specific numbered Tile
numTile* findtile(numTile* numTileary[], int xt, int yt){
    int tile = 0;
    while(numTileary[tile] != NULL){
        if(numTileary[tile]->x_tile == xt && numTileary[tile]->y_tile == yt){
            return numTileary[tile];
        }
        tile++;
    }

    return NULL;
}

// Process of clearing all 0 Tiles around another 0 Tile
void zeroProcess(numTile* numTileary[], numTile* tile, int glenX, int glenY){
    int x = tile->x_tile;
    int y = tile->y_tile;

    if(x-1 >=0){
        if(y-1 >= 0){
            numTile* newtile = findtile(numTileary,x-1,y-1);
            if(newtile->value == 0 && newtile->cover == 0){
                newtile->cover = 1;
                zeroProcess(numTileary,newtile,glenX,glenY);
            }
            else{
                newtile->cover = 1;
            }
        }
        numTile* newtile = findtile(numTileary,x-1,y);
        if(newtile->value == 0 && newtile->cover == 0){
            newtile->cover = 1;
            zeroProcess(numTileary,newtile,glenX,glenY);
        }
        else{
            newtile->cover = 1;
        }

        if(y+1 < glenY){
            numTile* newtile = findtile(numTileary,x-1,y+1);
            if(newtile->value == 0 && newtile->cover == 0){
                newtile->cover = 1;
                zeroProcess(numTileary,newtile,glenX,glenY);
            }
            else{
                newtile->cover = 1;
            }
        }
    }

    if(y-1 >= 0){
        numTile* newtile = findtile(numTileary,x,y-1);
        if(newtile->value == 0 && newtile->cover == 0){
            newtile->cover = 1;
            zeroProcess(numTileary,newtile,glenX,glenY);
        }
        else{
            newtile->cover = 1;
        }
    }
    if(y+1 < glenY){
        numTile* newtile = findtile(numTileary,x,y+1);
        if(newtile->value == 0 && newtile->cover == 0){
            newtile->cover = 1;
            zeroProcess(numTileary,newtile,glenX,glenY);
        }
        else{
            newtile->cover = 1;
        }
    }

    if(x+1 < glenX){
        if(y-1 >= 0){
            numTile* newtile = findtile(numTileary,x+1,y-1);
            if(newtile->value == 0 && newtile->cover == 0){
                newtile->cover = 1;
                zeroProcess(numTileary,newtile,glenX,glenY);
            }
            else{
                newtile->cover = 1;
            }
        }
        numTile* newtile = findtile(numTileary,x+1,y);
        if(newtile->value == 0 && newtile->cover == 0){
            newtile->cover = 1;
            zeroProcess(numTileary,newtile,glenX,glenY);
        }
        else{
            newtile->cover = 1;
        }
        if(y+1 < glenY){
            numTile* newtile = findtile(numTileary,x+1,y+1);
            if(newtile->value == 0 && newtile->cover == 0){
                newtile->cover = 1;
                zeroProcess(numTileary,newtile,glenX,glenY);
            }
            else{
                newtile->cover = 1;
            }
        }
    }
    
}

// Event Handler
int processEvents(SDL_Window* window, GameState* gamestate){
    SDL_Event event;
    int done = 0;

    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_WINDOWEVENT_CLOSE:
            {
                if(window){
                SDL_DestroyWindow(window);
                window = NULL;
                done = 1;
                }
                break;
            }
            case SDL_MOUSEMOTION:
            {
                int x,y;
                SDL_GetMouseState(&x,&y);
                gamestate->mouse_x = x;
                gamestate->mouse_y = y;
            }
            break;
            case SDL_MOUSEBUTTONDOWN:
            {
                // Left Click Event
                if(event.button.button == SDL_BUTTON_LEFT){
                    int xt = gamestate->mouse_x/32;
                    int yt = gamestate->mouse_y/32;

                    numTile* foundTile = findtile(gamestate->numTileary,xt,yt);
                    if(foundTile != NULL){
                        if(foundTile->flag == 1){
                            break;
                        }
                        foundTile->cover = 1;
                        if(foundTile->value == 0){
                            zeroProcess(gamestate->numTileary,foundTile,GAMELENX,GAMELENY);
                        }
                        break;
                    }

                    for(int mine = 0; mine < gamestate->numMines; mine++){
                        if(gamestate->mineary[mine]->x_tile == xt && gamestate->mineary[mine]->y_tile == yt){
                            if(gamestate->mineary[mine]->flag == 1){
                                break;
                            }
                            gamestate->mineary[mine]->cover = 1;
                            gamestate->mineary[mine]->bomb = 1;
                            for(int j = 0; j < gamestate->numMines; j++){
                                gamestate->mineary[j]->cover = 1;
                            }
                            break;
                        }
                    }
                }
                // Right Click Event
                if(event.button.button == SDL_BUTTON_RIGHT){
                    int xt = gamestate->mouse_x/32;
                    int yt = gamestate->mouse_y/32;

                    numTile* foundTile = findtile(gamestate->numTileary,xt,yt);
                    if(foundTile != NULL){
                        if(foundTile->cover == 0){
                            if(foundTile->flag == 0){
                                foundTile->flag = 1;
                            }
                            else{
                                foundTile->flag = 0;
                            }
                            break;
                        }
                    }

                    for(int mine = 0; mine < gamestate->numMines; mine++){
                        if(gamestate->mineary[mine]->x_tile == xt && gamestate->mineary[mine]->y_tile == yt 
                        && gamestate->mineary[mine]->cover == 0){
                            if(gamestate->mineary[mine]->flag == 0){
                                gamestate->mineary[mine]->flag = 1;
                            }
                            else{
                                gamestate->mineary[mine]->flag = 0;
                            }
                            break;
                        }
                    }
                }
            }
            break;
            case SDL_KEYDOWN:
            {
                switch(event.key.keysym.sym){
                    // Close Game using ESC key
                    case SDLK_ESCAPE:
                    {
                        done = 1;
                        break;
                    }
                    // Restart Game using R key
                    case SDLK_r:
                    {
                        done = 2;
                        break;
                    }
                }
            }
            break;
            case SDL_QUIT:
            {
                // Quit out of the game
                done = 1;
                break;
            }
        }
    }
    return done;
}

// Using Number Texture Draw 1-8
void number_display(SDL_Renderer *renderer, SDL_Texture* texture, int x, int y, int number){
    // Each Number Texture is 32x32

    // Darker Grey to show TIle was clicked
    SDL_Rect rt = {32*(x/32),32*(y/32),SPACING,SPACING};
    SDL_SetRenderDrawColor(renderer,140, 140, 140,255);
    SDL_RenderFillRect(renderer, &rt );
    if(number == 0){

        return;
    }
    
    // Places the Src at the correct posistion for the correct number
    int sx = number*32 - 32;
    
    SDL_Rect src = {sx,0,32,32};
    SDL_Rect dest = {x,y,SPACING,SPACING};

    SDL_RenderCopy(renderer,texture,&src,&dest);
}

// Drawing mines to the screen
void mine_display(SDL_Renderer *renderer, SDL_Texture* mine_texture, SDL_Texture* flag_texture, Mine* mineary[], int numMines){
    for(int mine = 0; mine < numMines; mine++){
        if(mineary[mine]->cover == 1){
            SDL_Rect dest = {mineary[mine]->x_tile*SPACING,mineary[mine]->y_tile*SPACING,SPACING,SPACING};
            
            // If Mine is Left Clicked then set its tile to red 
            if(mineary[mine]->bomb == 1){
                SDL_SetRenderDrawColor(renderer,255, 0, 0,255);
                SDL_RenderFillRect(renderer, &dest );
            }
            SDL_RenderCopy(renderer,mine_texture,NULL,&dest);
        }
        else{
            // If not covered and flagged draw flag over Tile
            if(mineary[mine]->flag== 1){
                SDL_Rect dest = {mineary[mine]->x_tile*SPACING,mineary[mine]->y_tile*SPACING,SPACING,SPACING};
                SDL_RenderCopy(renderer,flag_texture,NULL,&dest);
            }
        }
    }
}

// Drawing a numTile to the screen
void numtile_display(SDL_Renderer *renderer, SDL_Texture* num_texture, SDL_Texture* flag_texture, numTile* numTileary[]){
    int tile = 0;
    while(numTileary[tile] != NULL){
        // Drawing the correct number on the Tile
        if(numTileary[tile]->cover == 1){
            number_display(renderer,num_texture,numTileary[tile]->x_tile*SPACING,numTileary[tile]->y_tile*SPACING,numTileary[tile]->value);
        }
        else{
            // If not covered and flagged draw flag over Tile
            if(numTileary[tile]->flag == 1){
                SDL_Rect dest = {numTileary[tile]->x_tile*SPACING,numTileary[tile]->y_tile*SPACING,SPACING,SPACING};
                SDL_RenderCopy(renderer,flag_texture,NULL,&dest);
            }
        }
        
        tile++;
    }
}

void doRender(SDL_Renderer *renderer, GameState* gamestate){
    // Set background to a Light Grey
    SDL_SetRenderDrawColor(renderer,180, 180, 180,255);
    SDL_RenderClear(renderer);

    // Mouse Hover displays a darker Grey on the Hovered Tile
    SDL_Rect dest = {32*(gamestate->mouse_x/32),32*(gamestate->mouse_y/32),SPACING,SPACING};
    SDL_SetRenderDrawColor(renderer,140, 140, 140,255);
    SDL_RenderFillRect(renderer, &dest );

    numtile_display(renderer, gamestate->NumberTexture, gamestate->FlagTexture, gamestate->numTileary);

    // Drawing lines to visually separate Tiles
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    for(int i = 0; i < SCREEN_WIDTH; i+= SPACING){
        SDL_RenderDrawLine(renderer, i, 0, i, SCREEN_HEIGHT);
    }
    for(int j = 0; j < SCREEN_HEIGHT; j+= SPACING){
        SDL_RenderDrawLine(renderer, 0, j, SCREEN_WIDTH, j);
    }

    mine_display(renderer, gamestate->MineTexture, gamestate->FlagTexture,gamestate->mineary, gamestate->numMines);

    SDL_RenderPresent(renderer);
}

// Mine Helper Functions
Mine* createMine(int x, int y){
    Mine* mine = (Mine*)malloc(sizeof(Mine));
    mine->x_tile = x;
    mine->y_tile = y;
    mine->cover = 0;
    mine->bomb = 0;
    mine->flag = 0;
    return mine;
}

void freeMines(Mine** mineary, int numMines){
    for(int i = 0; i < numMines; i++){
        free(mineary[i]);
    }
}

// Creating and randomly placing mines onto the gameboard
void placeMines(int gameboard[][GAMELENY], int glenX, int glenY, Mine* mineary[], int numMines){
    int rx = 0;
    int ry = 0;
    for(int mine = 0; mine < numMines; mine++){
        int placed = 0;
        while(placed == 0){
            rx = rand() % glenX;
            ry = rand() % glenY;
            if(gameboard[rx][ry] == 0){
                gameboard[rx][ry] = 9;
                mineary[mine] = createMine(rx,ry);
                placed = 1;
            }
        }
    }
}

// numTile Helper Functions
numTile* createnumTile(int x, int y, int value){
    numTile* tile = (numTile*)malloc(sizeof(numTile));
    tile->x_tile = x;
    tile->y_tile = y;
    tile->value = value;
    tile->cover = 0;
    tile->flag = 0;
    return tile;
}

void freenumTiles(numTile* numTileary[]){
    int i = 0;
    while(numTileary[i] != NULL){
        free(numTileary[i]);
        i++;
    }
}

// Summing Total number of mines around a Tile on the gameboard
int placeNumber(int gameboard[][GAMELENY], int glenX, int glenY, int x, int y){
    int sum = 0;
    if(gameboard[x][y] == 0){
        if(x-1 >=0){
            if(y-1 >= 0){
                if(gameboard[x-1][y-1] == 9){
                    sum++;
                }
            }
            if(gameboard[x-1][y] == 9){
                sum++;
            }
            if(y+1 < glenY){
                if(gameboard[x-1][y+1] == 9){
                    sum++;
                }
            }
        }

        if(y-1 >= 0){
            if(gameboard[x][y-1] == 9){
                sum++;
            }
        }
        if(y+1 < glenY){
            if(gameboard[x][y+1] == 9){
                sum++;
            }
        }

        if(x+1 < glenX){
            if(y-1 >= 0){
                if(gameboard[x+1][y-1] == 9){
                    sum++;
                }
            }
            if(gameboard[x+1][y] == 9){
                sum++;
            }
            if(y+1 < glenY){
                if(gameboard[x+1][y+1] == 9){
                    sum++;
                }
            }
        }
    }
    return sum;
}

int game(SDL_Window *window, SDL_Renderer *renderer){
    
    srand(time(NULL));

    // Initialized X by Y gameboard based on GAMELENX and GAMELENY
    int gameboard[GAMELENX][GAMELENY];
    for(int i = 0; i < GAMELENX; i++){
        for(int j = 0; j < GAMELENY; j++){
            gameboard[i][j] = 0;
        }
    }

    // For gameboard 9 == mine, 0-8 == numTile values

    // Initializing mineary and numTileary
    Mine* mineary[NUMMINES];
    const int numTileAryLen = GAMELENX*GAMELENY;
    numTile* numTileary[numTileAryLen];
    for(int tile = 0; tile < numTileAryLen; tile++){
        numTileary[tile] = NULL;
    }

    placeMines(gameboard,GAMELENX,GAMELENY,mineary,NUMMINES);

    //placing numbered tiles
    int gbx = GAMELENX;
    int gby = GAMELENY;
    int numtiles = 0;
    for(int x = 0; x < gbx; x++){
        for(int y = 0; y < gby; y++){
            if(gameboard[x][y] != 9){
                gameboard[x][y] = placeNumber(gameboard,GAMELENX,GAMELENY,x,y);
                
                numTileary[numtiles] = createnumTile(x,y,gameboard[x][y]);
                numtiles++;
                
            }
        }
    }

    // Define structs
    GameState* gamestate = (GameState*)malloc(sizeof(GameState));
    
    // Load images and values
    gamestate->NumberTexture = Load_Texture(renderer,window,"numbers.png");
    gamestate->MineTexture = Load_Texture(renderer,window,"mine.png");
    gamestate->FlagTexture = Load_Texture(renderer,window,"flag.png");
    gamestate->mineary = mineary;
    gamestate->numTileary = numTileary;
    gamestate->numMines = NUMMINES;
    gamestate->mouse_x = 0;
    gamestate->mouse_y = 0;


    // Main Game Loop, 
    // Quits: done == 1, Restarts: done == 2
    int done = 0;
    while(!done){ 
        done = processEvents(window,gamestate);
        doRender(renderer,gamestate);
    }

    // Free textures
    Texture_Free(gamestate->NumberTexture);
    Texture_Free(gamestate->MineTexture);

    // Free heap items
    freenumTiles(numTileary);
    freeMines(mineary,NUMMINES);
    free(gamestate);

    return done;
}

int main(int argc, char *argv[]){
    SDL_Window *window;
    SDL_Renderer *renderer;
            
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(  "Minesweeper", 
                                SDL_WINDOWPOS_UNDEFINED, 
                                SDL_WINDOWPOS_UNDEFINED, 
                                SCREEN_WIDTH, 
                                SCREEN_HEIGHT,
                                0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    while(game(window,renderer) == 2){
    }

    // Close and destory the window
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    // Clean up
    SDL_Quit();
    return 0;
}