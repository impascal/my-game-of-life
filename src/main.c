#include <stdio.h>
#include <time.h>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

/* window width and height*/
#define W_WIDTH 800
#define W_HEIGHT 800

/* how many seconds before next cells update*/
#define UPDATE_TIME 1.0

/* matrix and cells settings */
#define DRAW_MAT 1
#define M_COLS 52
#define M_ROWS 52
#define CELL_SIZE 15
char cells[M_COLS][M_ROWS];
enum {DEAD, ALIVE}; 

void fillRect(SDL_Renderer *renderer, int x, int y, int w, int h);
void cells_init();
void cells_update();
void cells_render(SDL_Renderer *renderer);

int main(int argc, const char *argv[]) {

    /* setup time tracking */
    clock_t start_time;
    double timer = 0.0;

    start_time = clock();  

    /* Initialize SDL */
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Failed to initialize SDL: %s\n",
            SDL_GetError());
        return -1;
    }

    /* Create window */
    SDL_Window *window = SDL_CreateWindow(
        "Game of Life by perahhh",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        W_WIDTH, W_HEIGHT,
        SDL_WINDOW_RESIZABLE);

    if(window == NULL) {
        fprintf(stderr, "Failed to create SDL window: %s\n",
            SDL_GetError());
        return -1;
    }

    /* create renderer */
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL) {
        fprintf(stderr, "Failed to create SDL renderer: %s\n",
            SDL_GetError());
    }

    /* setup cells */
    cells_init();

    /* setup loop */
    char running = 1;
    SDL_Event event;

    /* loop */
    while(running) {
        
        /* check loop start time and update elapsed time */
        start_time = clock();

        /* handle events */
        SDL_PollEvent(&event);
            switch(event.type) {
            case SDL_QUIT:
                fprintf(stdout, "Quitting...\n");
                running = 0;
                break;
            default:
                break;
            }

        /* update stuff periodically and reset timer */
        if (timer >= UPDATE_TIME && timer != 0) {
            cells_update();
            timer = 0;
        }
        
        /* clear screen with black */
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
        SDL_RenderClear(renderer);

        /* draw stuff on the backbuffer */
        cells_render(renderer);

        /* present back buffer on screen */
        SDL_RenderPresent(renderer);

        /* update elapsed time */
        timer += (double) (clock() - start_time) / CLOCKS_PER_SEC;
    }

    SDL_Quit();
    return 0;
}

/* draw rect given position and dimensions */
void fillRect(SDL_Renderer *renderer, int x, int y, int w, int h) {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    SDL_RenderFillRect(renderer, &rect);
}

/* initialize cells in a random way */
void cells_init() {

    /* start with everything dead */
    for (int i=0; i < M_COLS; i++) {
        for (int j=0; j < M_ROWS; j++) {
            cells[i][j] = 0;
        }
    }

    /* create a center of alive cells */
    int x = M_COLS / 2;
    int y = M_ROWS / 2;

    for(int i=-2; i <= 2; i++) {
        for(int j=-2; j <= 2; j++) {
            cells[x+i][y+j] = 1;
        }
    }
}

/* update cells state */
void cells_update() {
    /* n alive cells around */
    int cnt = 0;

    for (int i=0; i < M_COLS; i++) {
        for (int j=0; j < M_ROWS; j++) {

            for (int k=-1; k <= 1; k++) {
                for (int h=-1; h<=1; h++) {
                    if ( (i+k) >= 0 && (i+k) < M_COLS && (j+h) >= 0 && (j+h) < M_ROWS )
                        if (cells[i+k][j+h] == 1)
                            ++cnt;
                }
            }

            if (cells[i][j] == ALIVE) {
                --cnt;
                /* kill cell if underpop or overpop*/
                if (cnt < 2 || cnt > 3) 
                    cells[i][j] = DEAD;

            }else if(cells[i][j] == DEAD && cnt == 3) {
                cells[i][j] = ALIVE;
            }
        }
    }    
}

/* render matrix and cells matrix */
void cells_render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    /* draw matrix */
    if (DRAW_MAT == 1) {
        for (double x=0; x < W_WIDTH; x += CELL_SIZE) {
           SDL_RenderDrawLine(renderer, x, 0, x, W_HEIGHT);
        }

        for (double y=0; y < W_HEIGHT; y += CELL_SIZE) {
            SDL_RenderDrawLine(renderer, 0, y, W_WIDTH, y);
        }
    }

    /* draw alive cells */
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    for (int x=0; x < M_COLS; x++) {
        for (int y=0; y < M_ROWS; y++) {
            if (cells[x][y] == 1) {
                fillRect(renderer,
                    x*CELL_SIZE, y*CELL_SIZE, 
                    CELL_SIZE, CELL_SIZE);    
            }
        }
    }       
}