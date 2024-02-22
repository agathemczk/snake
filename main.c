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


// Coordonnées initiales
int snakeX = -50;
int snakeY = -50;

int framesPerMove = 5; // Le serpent bouge toutes les 5 frames
int frameCount = 0; // Compteur de frames

typedef enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

// Variable pour stocker la direction actuelle
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


// Initialisation du serpent
SnakeSegment* initSnake(int x, int y) {
    SnakeSegment* head = (SnakeSegment*)malloc(sizeof(SnakeSegment));
    head->x = x;
    head->y = y;
    head->next = NULL;
    return head;
}

// Ajout d'un segment au serpent
void addSegment(SnakeSegment* head, int x, int y) {
    SnakeSegment* newSegment = (SnakeSegment*)malloc(sizeof(SnakeSegment));
    newSegment->x = x;
    newSegment->y = y;
    newSegment->next = head->next;
    head->next = newSegment;
}

// Affichage du serpent
void drawSnake(SDL_Renderer* renderer, SnakeSegment* head) {
    SDL_SetRenderDrawColor(renderer, 56, 159, 109 , 255);
    SnakeSegment* current = head;
    while (current) {
        SDL_Rect rect = { current->x, current->y, SEGMENT_SIZE, SEGMENT_SIZE };
        SDL_RenderFillRect(renderer, &rect);
        current = current->next;
    }
}

// Suppression du dernier segment du serpent
void removeLastSegment(SnakeSegment* head) {
    if (head->next == NULL) {
        return; // Ne rien faire si le serpent n'a qu'un seul segment
    } else {
        SnakeSegment* current = head;
        while (current->next->next != NULL) {
            current = current->next;
        }
        free(current->next);
        current->next = NULL;
    }
}

// Génération d'une pomme à une position aléatoire
Apple generateApple() {
    Apple apple;
    apple.x = (rand() % ((WINDOW_WIDTH - APPLE_SIZE) / APPLE_SIZE)) * APPLE_SIZE;
    apple.y = (rand() % ((WINDOW_HEIGHT - APPLE_SIZE) / APPLE_SIZE)) * APPLE_SIZE;
    return apple;
}

// Génération d'une pomme noire à une position aléatoire
BlackApple generateBlackApple() {
    BlackApple blackApple;
    blackApple.x = (rand() % ((WINDOW_WIDTH - APPLE_SIZE) / APPLE_SIZE)) * APPLE_SIZE;
    blackApple.y = (rand() % ((WINDOW_HEIGHT - APPLE_SIZE) / APPLE_SIZE)) * APPLE_SIZE;
    return blackApple;
}

// TOUJOURS PLUS DE POMMEESS !
BlueApple generateBlueApple() {
    BlueApple blueApple;
    blueApple.x = (rand() % ((WINDOW_WIDTH - APPLE_SIZE) / APPLE_SIZE)) * APPLE_SIZE;
    blueApple.y = (rand() % ((WINDOW_HEIGHT - APPLE_SIZE) / APPLE_SIZE)) * APPLE_SIZE;
    return blueApple;
}


// Vérifie si la tête du serpent touche n'importe quelle autre partie de son corps
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
    if (!font) {
        printf("Error loading font: %s\n", TTF_GetError());
        TTF_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }

    // Position initiale du serpent
    SnakeSegment* snake = initSnake(snakeX, snakeY);

    // Déplace le serpent une fois avant d'entrer dans la boucle principale (pour éviter le point au centre)
    snakeX = WINDOW_WIDTH / 2 - SEGMENT_SIZE / 2 ;
    snakeY = WINDOW_HEIGHT / 2 - SEGMENT_SIZE / 2;
    addSegment(snake, snakeX, snakeY);

    Apple apple = generateApple();
    BlueApple blueApple = generateBlueApple();
    BlackApple blackApple = generateBlackApple();


    int running = 1;


    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            if (event.type == SDL_KEYDOWN) {
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
            }
        }


        // Met à jour les coordonnées du serpent en fonction de la direction
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


        // Collision du serpent avec lui-même
        if (checkCollision(snake, snakeX, snakeY)) {

            // Affiche "Game Over"
            SDL_Color textColor = {92, 63, 108, 255};
            SDL_Surface* textSurfaceGameOver = TTF_RenderText_Blended(font, "Game Over", textColor);
            SDL_Texture* textTextureGameOver = SDL_CreateTextureFromSurface(renderer, textSurfaceGameOver);

            // Calcule la position du texte pour le centrer
            int textWidth, textHeight;
            TTF_SizeText(font, "Game Over", &textWidth, &textHeight);
            SDL_Rect textRectGameOver = {
                    (WINDOW_WIDTH - textWidth) / 2,  // Position x
                    (WINDOW_HEIGHT - textHeight) / 2, // Position y
                    textWidth,
                    textHeight
            };

            SDL_RenderCopy(renderer, textTextureGameOver, NULL, &textRectGameOver);
            SDL_RenderPresent(renderer);

            // Attendre 5s puis quitter
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







        // Affiche l'écran
        if (running) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // blanc
            SDL_RenderClear(renderer);
        }


        addSegment(snake, snakeX, snakeY);

        // Collisions avec les bordes de la fenêtre
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


        // Dessine la pomme rouge
        SDL_SetRenderDrawColor(renderer, 187, 51, 51 , 255 ); // Couleur rouge
        SDL_Rect appleRect = { apple.x, apple.y, APPLE_SIZE, APPLE_SIZE };
        SDL_RenderFillRect(renderer, &appleRect);

        // Dessine la pomme bleue
        SDL_SetRenderDrawColor(renderer, 20, 106, 124 , 255); // Couleur bleue
        SDL_Rect blueAppleRect = { blueApple.x, blueApple.y, APPLE_SIZE, APPLE_SIZE };
        SDL_RenderFillRect(renderer, &blueAppleRect);


        // Dessine la pomme noire
        SDL_SetRenderDrawColor(renderer, 92, 63, 108, 255); // Couleur tkt
        SDL_Rect blackAppleRect = { blackApple.x, blackApple.y, APPLE_SIZE, APPLE_SIZE };
        SDL_RenderFillRect(renderer, &blackAppleRect);


        // Affiche le serpent
        drawSnake(renderer, snake);


        // Vérifie si le serpent a mangé une pomme rouge
        if ((snakeX >= apple.x && snakeX < apple.x + APPLE_SIZE &&
             snakeY >= apple.y && snakeY < apple.y + APPLE_SIZE) ||
            (snakeX + SEGMENT_SIZE > apple.x && snakeX + SEGMENT_SIZE <= apple.x + APPLE_SIZE &&
             snakeY >= apple.y && snakeY < apple.y + APPLE_SIZE) ||
            (snakeX >= apple.x && snakeX < apple.x + APPLE_SIZE &&
             snakeY + SEGMENT_SIZE > apple.y && snakeY + SEGMENT_SIZE <= apple.y + APPLE_SIZE) ||
            (snakeX + SEGMENT_SIZE > apple.x && snakeX + SEGMENT_SIZE <= apple.x + APPLE_SIZE &&
             snakeY + SEGMENT_SIZE > apple.y && snakeY + SEGMENT_SIZE <= apple.y + APPLE_SIZE)) {

            // Nouvelle pomme
            apple = generateApple();

            // Ajoute deux nouveaux segments au serpent
            addSegment(snake, snakeX, snakeY);
            addSegment(snake, snakeX, snakeY);
        } else {

            // Supprime le dernier segment du serpent
            removeLastSegment(snake);
        }


        // Vérifie si le serpent a mangé une pomme bleue
        if ((snakeX >= blueApple.x && snakeX < blueApple.x + APPLE_SIZE &&
             snakeY >= blueApple.y && snakeY < blueApple.y + APPLE_SIZE) ||
            (snakeX + SEGMENT_SIZE > blueApple.x && snakeX + SEGMENT_SIZE <= blueApple.x + APPLE_SIZE &&
             snakeY >= blueApple.y && snakeY < blueApple.y + APPLE_SIZE) ||
            (snakeX >= blueApple.x && snakeX < blueApple.x + APPLE_SIZE &&
             snakeY + SEGMENT_SIZE > blueApple.y && snakeY + SEGMENT_SIZE <= blueApple.y + APPLE_SIZE) ||
            (snakeX + SEGMENT_SIZE > blueApple.x && snakeX + SEGMENT_SIZE <= blueApple.x + APPLE_SIZE &&
             snakeY + SEGMENT_SIZE > blueApple.y && snakeY + SEGMENT_SIZE <= blueApple.y + APPLE_SIZE)) {

            // Génére une nouvelle pomme bleue
            blueApple = generateBlueApple();

            // Réduit la taille du serpent d'un segment
            removeLastSegment(snake);
        }


        // Vérifie si le serpent a mangé une pomme noire
        if ((snakeX >= blackApple.x && snakeX < blackApple.x + APPLE_SIZE &&
             snakeY >= blackApple.y && snakeY < blackApple.y + APPLE_SIZE) ||
            (snakeX + SEGMENT_SIZE > blackApple.x && snakeX + SEGMENT_SIZE <= blackApple.x + APPLE_SIZE &&
             snakeY >= blackApple.y && snakeY < blackApple.y + APPLE_SIZE) ||
            (snakeX >= blackApple.x && snakeX < blackApple.x + APPLE_SIZE &&
             snakeY + SEGMENT_SIZE > blackApple.y && snakeY + SEGMENT_SIZE <= blackApple.y + APPLE_SIZE) ||
            (snakeX + SEGMENT_SIZE > blackApple.x && snakeX + SEGMENT_SIZE <= blackApple.x + APPLE_SIZE &&
             snakeY + SEGMENT_SIZE > blackApple.y && snakeY + SEGMENT_SIZE <= blackApple.y + APPLE_SIZE)) {

            // Affiche "Game Over"
            SDL_Color textColor = {92, 63, 108, 255};
            SDL_Surface* textSurfaceGameOver = TTF_RenderText_Blended(font, "Game Over", textColor);
            SDL_Texture* textTextureGameOver = SDL_CreateTextureFromSurface(renderer, textSurfaceGameOver);

            // Calcule la position du texte pour le centrer
            int textWidth, textHeight;
            TTF_SizeText(font, "Game Over", &textWidth, &textHeight);
            SDL_Rect textRectGameOver = {
                    (WINDOW_WIDTH - textWidth) / 2,  // Position x
                    (WINDOW_HEIGHT - textHeight) / 2, // Position y
                    textWidth,
                    textHeight
            };

            SDL_RenderCopy(renderer, textTextureGameOver, NULL, &textRectGameOver);
            SDL_RenderPresent(renderer);

            // Attendre 5 secondes puis quitter
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

        // Affiche l'écran
        SDL_RenderPresent(renderer);

        // Vitesse
        SDL_Delay(1000/FPS);
    }

    // Libére la mémoire
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

