#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#define FPS 40
#define SEGMENT_SIZE 15
#define APPLE_SIZE 25
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


int snakeX = -50;
int snakeY = -50;

int snakeLength = 0;

SDL_Color snakeColor = {56, 159, 109, 255};

bool isInverse = false;

typedef enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

Direction direction;


typedef struct SnakeSegment {
    int x, y;
    struct SnakeSegment* next;
} SnakeSegment;


typedef struct Apple {
    int x, y;
} Apple;

typedef struct BlackApple {
    int x, y;
} BlackApple;

typedef struct BlueApple {
    int x, y;
} BlueApple;

typedef struct OrangeApple {
    int x, y;
} OrangeApple;

typedef struct PinkLady {
    int x, y;
} PinkLady;

typedef struct InverseApple {
    int x, y;
} InverseApple;

SnakeSegment* initSnake(int x, int y) {
    SnakeSegment* head = (SnakeSegment*)malloc(sizeof(SnakeSegment));
    head->x = x;
    head->y = y;
    head->next = NULL;
    return head;
}

void addSegment(SnakeSegment* head, int x, int y) {
    SnakeSegment* newSegment = (SnakeSegment*)malloc(sizeof(SnakeSegment));
    newSegment->x = x;
    newSegment->y = y;
    newSegment->next = head->next;
    head->next = newSegment;
    snakeLength++;
}

void drawSnake(SDL_Renderer* renderer, SnakeSegment* head) {
    SDL_SetRenderDrawColor(renderer, snakeColor.r, snakeColor.g, snakeColor.b, 255);
    SnakeSegment* current = head;
    while (current) {
        SDL_Rect rect = { current->x, current->y, SEGMENT_SIZE, SEGMENT_SIZE };
        SDL_RenderFillRect(renderer, &rect);
        current = current->next;
    }
}

void removeLastSegment(SnakeSegment* head) {
    if (head->next == NULL) {
        return; 
    } else {
        SnakeSegment* current = head;
        while (current->next->next != NULL) {
            current = current->next;
        }
        free(current->next);
        current->next = NULL;
        snakeLength--;
    }
}

Apple generateApple() {
    Apple apple;
    apple.x = (rand() % ((WINDOW_WIDTH - APPLE_SIZE) / APPLE_SIZE)) * APPLE_SIZE;
    apple.y = (rand() % ((WINDOW_HEIGHT - APPLE_SIZE) / APPLE_SIZE)) * APPLE_SIZE;
    return apple;
}

BlackApple generateBlackApple() {
    BlackApple blackApple;
    blackApple.x = (rand() % ((WINDOW_WIDTH - APPLE_SIZE) / APPLE_SIZE)) * APPLE_SIZE;
    blackApple.y = (rand() % ((WINDOW_HEIGHT - APPLE_SIZE) / APPLE_SIZE)) * APPLE_SIZE;
    return blackApple;
}

BlueApple generateBlueApple() {
    BlueApple blueApple;
    blueApple.x = (rand() % ((WINDOW_WIDTH - APPLE_SIZE) / APPLE_SIZE)) * APPLE_SIZE;
    blueApple.y = (rand() % ((WINDOW_HEIGHT - APPLE_SIZE) / APPLE_SIZE)) * APPLE_SIZE;
    return blueApple;
}

OrangeApple generateOrangeApple() {
    OrangeApple orangeApple;
    orangeApple.x = (rand() % ((WINDOW_WIDTH - APPLE_SIZE) / APPLE_SIZE)) * APPLE_SIZE;
    orangeApple.y = (rand() % ((WINDOW_HEIGHT - APPLE_SIZE) / APPLE_SIZE)) * APPLE_SIZE;
    return orangeApple;
}

PinkLady generatePinkLady() {
    PinkLady pinkLady;
    pinkLady.x = (rand() % ((WINDOW_WIDTH - APPLE_SIZE) / APPLE_SIZE)) * APPLE_SIZE;
    pinkLady.y = (rand() % ((WINDOW_HEIGHT - APPLE_SIZE) / APPLE_SIZE)) * APPLE_SIZE;
    return pinkLady;
}

InverseApple generateInverseApple() {
    InverseApple inverseApple;
    inverseApple.x = (rand() % ((WINDOW_WIDTH - APPLE_SIZE) / APPLE_SIZE)) * APPLE_SIZE;
    inverseApple.y = (rand() % ((WINDOW_HEIGHT - APPLE_SIZE) / APPLE_SIZE)) * APPLE_SIZE;
    return inverseApple;
}

bool checkCollision(SnakeSegment* head, int x, int y) {
    SnakeSegment* current = head;
    while (current) {
        if (current->x == x && current->y == y) {
            return true;
        }
        current = current->next;
    }
    return false;
}

void redistributeApples(Apple* apple, BlackApple* blackApple, BlueApple* blueApple, OrangeApple* orangeApple, PinkLady* pinkLady, InverseApple* inverseApple) {
    *apple = generateApple();
    *blackApple = generateBlackApple();
    *blueApple = generateBlueApple();
    *orangeApple = generateOrangeApple();
    *pinkLady = generatePinkLady();
    *inverseApple = generateInverseApple();
}

void drawLength(SDL_Renderer* renderer, TTF_Font* smallFont, int snakeX, int snakeY, int lengthTextX, int lengthTextWidth, int lengthTextY, int lengthTextHeight) {
    SDL_Color textColor;
    if (snakeX >= lengthTextX && snakeX <= lengthTextX + lengthTextWidth && snakeY >= lengthTextY && snakeY <= lengthTextY + lengthTextHeight) {
        textColor = (SDL_Color){rand() % 256, rand() % 256, rand() % 256, 255};
    } else {
        textColor = (SDL_Color){92, 63, 108, 255};
    }
    char lengthText[50];
    sprintf(lengthText, "Length : %d", snakeLength);
    SDL_Surface* textSurface = TTF_RenderText_Blended(smallFont, lengthText, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = {
            20,
            WINDOW_HEIGHT - 50,
            textSurface->w,
            textSurface->h
    };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}


int main(int argc, char* argv[]) {

    srand(time(NULL));

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("SNAKE GAME (signé Agathe)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (TTF_Init() != 0) {
        printf("Error initializing TTF: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }

    TTF_Font* font = TTF_OpenFont("C:\\Users\\agath\\Documents\\SDL\\Baloo-Regular.ttf", 35);
    TTF_Font* smallFont = TTF_OpenFont("C:\\Users\\agath\\Documents\\SDL\\Baloo-Regular.ttf", 25);
    if (!font) {
        printf("Error loading font: %s\n", TTF_GetError());
        TTF_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }


    SnakeSegment* snake = initSnake(snakeX, snakeY);


    snakeX = WINDOW_WIDTH / 2 - SEGMENT_SIZE / 2 ;
    snakeY = WINDOW_HEIGHT / 2 - SEGMENT_SIZE / 2;
    addSegment(snake, snakeX, snakeY);

    Apple apple = generateApple();
    BlueApple blueApple = generateBlueApple();
    BlackApple blackApple = generateBlackApple();
    OrangeApple orangeApple = generateOrangeApple();
    PinkLady pinkLady = generatePinkLady();
    InverseApple inverseApple = generateInverseApple();

    bool isGameOver = false;
    int running = 1;

    int lengthTextX = 20;
    int lengthTextY = WINDOW_HEIGHT - 50;
    int lengthTextWidth = 100; // A ajuster
    int lengthTextHeight = 25; //A ajuster


    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            if (event.type == SDL_KEYDOWN) {
                if (!isInverse) {
                    switch (event.key.keysym.sym) {
                        case SDLK_z: // ou W en QWERTY
                            direction = UP;
                            break;
                        case SDLK_q: // ou A en QWERTY
                            direction = LEFT;
                            break;
                        case SDLK_s:
                            direction = DOWN;
                            break;
                        case SDLK_d:
                            direction = RIGHT;
                            break;
                    }
                } else {
                    switch (event.key.keysym.sym) {
                        case SDLK_z: // ou W en QWERTY
                            direction = DOWN;
                            break;
                        case SDLK_q: // ou A en QWERTY
                            direction = RIGHT;
                            break;
                        case SDLK_s:
                            direction = UP;
                            break;
                        case SDLK_d:
                            direction = LEFT;
                            break;
                    }
                }
            }
        }

        switch (direction) {
            case UP:
                snakeY -= SEGMENT_SIZE;
                break;
            case DOWN:
                snakeY += SEGMENT_SIZE;
                break;
            case LEFT:
                snakeX -= SEGMENT_SIZE;
                break;
            case RIGHT:
                snakeX += SEGMENT_SIZE;
                break;
        }

        if (checkCollision(snake, snakeX, snakeY)) {
            isGameOver = true;


            SDL_Color textColor = {92, 63, 108, 255};
            SDL_Surface* textSurfaceGameOver = TTF_RenderText_Blended(font, "Game Over", textColor);
            SDL_Texture* textTextureGameOver = SDL_CreateTextureFromSurface(renderer, textSurfaceGameOver);

            int textWidth, textHeight;
            TTF_SizeText(font, "Game Over", &textWidth, &textHeight);
            int textHeightGameOver = textHeight;
            SDL_Rect textRectGameOver = {
                    (WINDOW_WIDTH - textWidth) / 2,  // Position x
                    (WINDOW_HEIGHT - textHeight) / 2, // Position y
                    textWidth,
                    textHeight
            };

            SDL_RenderCopy(renderer, textTextureGameOver, NULL, &textRectGameOver);
            SDL_RenderPresent(renderer);


            char lengthText[50];
            sprintf(lengthText, "Final length : %d", snakeLength);
            SDL_Surface* textSurfaceLength = TTF_RenderText_Blended(font, lengthText, textColor);
            SDL_Texture* textTextureLength = SDL_CreateTextureFromSurface(renderer, textSurfaceLength);


            int textWidthLength, textHeightLength;
            TTF_SizeText(font, lengthText, &textWidthLength, &textHeightLength);
            SDL_Rect textRectLength = {
                    (WINDOW_WIDTH - textWidthLength) / 2,
                    (WINDOW_HEIGHT - textHeightGameOver) / 2 + textHeightGameOver + 10,
                    textWidthLength,
                    textHeightLength
            };

            SDL_RenderCopy(renderer, textTextureLength, NULL, &textRectLength);
            SDL_RenderPresent(renderer);


            Uint32 start_ticks = SDL_GetTicks();
            while (SDL_GetTicks() - start_ticks < 5000) {
                SDL_PollEvent(&event);
                if (event.type == SDL_QUIT) {
                    running = 0;
                    break;
                }
            }
            running = 0;
        }

        if (running) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // blanc
            SDL_RenderClear(renderer);
        }


        addSegment(snake, snakeX, snakeY);

        if (snakeX < 0) {
            snakeX = WINDOW_WIDTH ;
        }

        if (snakeX > WINDOW_WIDTH ) {
            snakeX = 0 - SEGMENT_SIZE ;
        }

        if (snakeY < 0) {
            snakeY = WINDOW_HEIGHT  ;
        }

        if (snakeY > WINDOW_HEIGHT ) {
            snakeY = 0  - SEGMENT_SIZE;
        }

        SDL_SetRenderDrawColor(renderer, 187, 51, 51 , 255 ); 
        SDL_Rect appleRect = { apple.x, apple.y, APPLE_SIZE, APPLE_SIZE };
        SDL_RenderFillRect(renderer, &appleRect);

        SDL_SetRenderDrawColor(renderer, 121, 202, 211, 255); 
        SDL_Rect blueAppleRect = { blueApple.x, blueApple.y, APPLE_SIZE, APPLE_SIZE };
        SDL_RenderFillRect(renderer, &blueAppleRect);


        SDL_SetRenderDrawColor(renderer, 92, 63, 108, 255); 
        SDL_Rect blackAppleRect = { blackApple.x, blackApple.y, APPLE_SIZE, APPLE_SIZE };
        SDL_RenderFillRect(renderer, &blackAppleRect);


        SDL_SetRenderDrawColor(renderer, 250, 162, 35, 255); 
        SDL_Rect orangeAppleRect = { orangeApple.x, orangeApple.y, APPLE_SIZE, APPLE_SIZE };
        SDL_RenderFillRect(renderer, &orangeAppleRect);

        SDL_SetRenderDrawColor(renderer, 240, 140, 207, 255);
        SDL_Rect pinkLadyRect = { pinkLady.x, pinkLady.y, APPLE_SIZE, APPLE_SIZE };
        SDL_RenderFillRect(renderer, &pinkLadyRect);

      
        SDL_SetRenderDrawColor(renderer, 134, 218, 68, 255); 
        SDL_Rect inverseAppleRect = { inverseApple.x, inverseApple.y, APPLE_SIZE, APPLE_SIZE };
        SDL_RenderFillRect(renderer, &inverseAppleRect);


        drawSnake(renderer, snake);


        if ((snakeX >= apple.x && snakeX < apple.x + APPLE_SIZE &&
             snakeY >= apple.y && snakeY < apple.y + APPLE_SIZE) ||
            (snakeX + SEGMENT_SIZE > apple.x && snakeX + SEGMENT_SIZE <= apple.x + APPLE_SIZE &&
             snakeY >= apple.y && snakeY < apple.y + APPLE_SIZE) ||
            (snakeX >= apple.x && snakeX < apple.x + APPLE_SIZE &&
             snakeY + SEGMENT_SIZE > apple.y && snakeY + SEGMENT_SIZE <= apple.y + APPLE_SIZE) ||
            (snakeX + SEGMENT_SIZE > apple.x && snakeX + SEGMENT_SIZE <= apple.x + APPLE_SIZE &&
             snakeY + SEGMENT_SIZE > apple.y && snakeY + SEGMENT_SIZE <= apple.y + APPLE_SIZE)) {


            apple = generateApple();
            addSegment(snake, snakeX, snakeY);

        } else {

            removeLastSegment(snake);
        }


        if ((snakeX >= blueApple.x && snakeX < blueApple.x + APPLE_SIZE &&
             snakeY >= blueApple.y && snakeY < blueApple.y + APPLE_SIZE) ||
            (snakeX + SEGMENT_SIZE > blueApple.x && snakeX + SEGMENT_SIZE <= blueApple.x + APPLE_SIZE &&
             snakeY >= blueApple.y && snakeY < blueApple.y + APPLE_SIZE) ||
            (snakeX >= blueApple.x && snakeX < blueApple.x + APPLE_SIZE &&
             snakeY + SEGMENT_SIZE > blueApple.y && snakeY + SEGMENT_SIZE <= blueApple.y + APPLE_SIZE) ||
            (snakeX + SEGMENT_SIZE > blueApple.x && snakeX + SEGMENT_SIZE <= blueApple.x + APPLE_SIZE &&
             snakeY + SEGMENT_SIZE > blueApple.y && snakeY + SEGMENT_SIZE <= blueApple.y + APPLE_SIZE)) {

            blueApple = generateBlueApple();
            removeLastSegment(snake);
        }

        if ((snakeX >= blackApple.x && snakeX < blackApple.x + APPLE_SIZE &&
             snakeY >= blackApple.y && snakeY < blackApple.y + APPLE_SIZE) ||
            (snakeX + SEGMENT_SIZE > blackApple.x && snakeX + SEGMENT_SIZE <= blackApple.x + APPLE_SIZE &&
             snakeY >= blackApple.y && snakeY < blackApple.y + APPLE_SIZE) ||
            (snakeX >= blackApple.x && snakeX < blackApple.x + APPLE_SIZE &&
             snakeY + SEGMENT_SIZE > blackApple.y && snakeY + SEGMENT_SIZE <= blackApple.y + APPLE_SIZE) ||
            (snakeX + SEGMENT_SIZE > blackApple.x && snakeX + SEGMENT_SIZE <= blackApple.x + APPLE_SIZE &&
             snakeY + SEGMENT_SIZE > blackApple.y && snakeY + SEGMENT_SIZE <= blackApple.y + APPLE_SIZE)) {

            isGameOver = true;

            SDL_Color textColor = {92, 63, 108, 255};
            SDL_Surface* textSurfaceGameOver = TTF_RenderText_Blended(font, "Game Over", textColor);
            SDL_Texture* textTextureGameOver = SDL_CreateTextureFromSurface(renderer, textSurfaceGameOver);

            
            int textWidth, textHeight;
            TTF_SizeText(font, "Game Over", &textWidth, &textHeight);
            int textHeightGameOver = textHeight;
            SDL_Rect textRectGameOver = {
                    (WINDOW_WIDTH - textWidth) / 2,  // Position x
                    (WINDOW_HEIGHT - textHeight) / 2, // Position y
                    textWidth,
                    textHeight
            };

            SDL_RenderCopy(renderer, textTextureGameOver, NULL, &textRectGameOver);
            SDL_RenderPresent(renderer);


           
            char lengthText[50];
            sprintf(lengthText, "Final length : %d", snakeLength);
            SDL_Surface* textSurfaceLength = TTF_RenderText_Blended(font, lengthText, textColor);
            SDL_Texture* textTextureLength = SDL_CreateTextureFromSurface(renderer, textSurfaceLength);


            int textWidthLength, textHeightLength;
            TTF_SizeText(font, lengthText, &textWidthLength, &textHeightLength);
            SDL_Rect textRectLength = {
                    (WINDOW_WIDTH - textWidthLength) / 2,
                    (WINDOW_HEIGHT - textHeightGameOver) / 2 + textHeightGameOver + 10,
                    textWidthLength,
                    textHeightLength
            };

            SDL_RenderCopy(renderer, textTextureLength, NULL, &textRectLength);
            SDL_RenderPresent(renderer);


            
            Uint32 start_ticks = SDL_GetTicks();
            while (SDL_GetTicks() - start_ticks < 5000) {
                SDL_PollEvent(&event);
                if (event.type == SDL_QUIT) {
                    running = 0;
                    break;
                }
            }
            running = 0;
        }

       
        if ((snakeX >= orangeApple.x && snakeX < orangeApple.x + APPLE_SIZE &&
             snakeY >= orangeApple.y && snakeY < orangeApple.y + APPLE_SIZE) ||
            (snakeX + SEGMENT_SIZE > orangeApple.x && snakeX + SEGMENT_SIZE <= orangeApple.x + APPLE_SIZE &&
             snakeY >= orangeApple.y && snakeY < orangeApple.y + APPLE_SIZE) ||
            (snakeX >= orangeApple.x && snakeX < orangeApple.x + APPLE_SIZE &&
             snakeY + SEGMENT_SIZE > orangeApple.y && snakeY + SEGMENT_SIZE <= orangeApple.y + APPLE_SIZE) ||
            (snakeX + SEGMENT_SIZE > orangeApple.x && snakeX + SEGMENT_SIZE <= orangeApple.x + APPLE_SIZE &&
             snakeY + SEGMENT_SIZE > orangeApple.y && snakeY + SEGMENT_SIZE <= orangeApple.y + APPLE_SIZE)) {

            redistributeApples(&apple, &blackApple, &blueApple, &orangeApple, &pinkLady, &inverseApple);
        }

     
        if ((snakeX >= pinkLady.x && snakeX < pinkLady.x + APPLE_SIZE &&
             snakeY >= pinkLady.y && snakeY < pinkLady.y + APPLE_SIZE) ||
            (snakeX + SEGMENT_SIZE > pinkLady.x && snakeX + SEGMENT_SIZE <= pinkLady.x + APPLE_SIZE &&
             snakeY >= pinkLady.y && snakeY < pinkLady.y + APPLE_SIZE) ||
            (snakeX >= pinkLady.x && snakeX < pinkLady.x + APPLE_SIZE &&
             snakeY + SEGMENT_SIZE > pinkLady.y && snakeY + SEGMENT_SIZE <= pinkLady.y + APPLE_SIZE) ||
            (snakeX + SEGMENT_SIZE > pinkLady.x && snakeX + SEGMENT_SIZE <= pinkLady.x + APPLE_SIZE &&
             snakeY + SEGMENT_SIZE > pinkLady.y && snakeY + SEGMENT_SIZE <= pinkLady.y + APPLE_SIZE)) {

            pinkLady = generatePinkLady();
            snakeColor = (SDL_Color){rand() % 256, rand() % 256, rand() % 256, 255}; 

        }

     
        if ((snakeX >= inverseApple.x && snakeX < inverseApple.x + APPLE_SIZE &&
             snakeY >= inverseApple.y && snakeY < inverseApple.y + APPLE_SIZE) ||
            (snakeX + SEGMENT_SIZE > inverseApple.x && snakeX + SEGMENT_SIZE <= inverseApple.x + APPLE_SIZE &&
             snakeY >= inverseApple.y && snakeY < inverseApple.y + APPLE_SIZE) ||
            (snakeX >= inverseApple.x && snakeX < inverseApple.x + APPLE_SIZE &&
             snakeY + SEGMENT_SIZE > inverseApple.y && snakeY + SEGMENT_SIZE <= inverseApple.y + APPLE_SIZE) ||
            (snakeX + SEGMENT_SIZE > inverseApple.x && snakeX + SEGMENT_SIZE <= inverseApple.x + APPLE_SIZE &&
             snakeY + SEGMENT_SIZE > inverseApple.y && snakeY + SEGMENT_SIZE <= inverseApple.y + APPLE_SIZE)) {

            inverseApple = generateInverseApple();
            isInverse = !isInverse;
        }


        if (!isGameOver) {
            drawLength(renderer, smallFont, snakeX, snakeY, lengthTextX, lengthTextWidth, lengthTextY, lengthTextHeight);
        }

    
        SDL_RenderPresent(renderer);

       
        SDL_Delay(1000/FPS);
    }


    while (snake) {
        SnakeSegment* temp = snake;
        snake = snake->next;
        free(temp);

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

