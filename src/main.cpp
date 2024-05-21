#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <bits/stdc++.h>
#include "Bullet.cpp"
#include "Enemy.cpp"
#include "Character.cpp"
#include "initialize.cpp"
#include "Menu.cpp"
#include "Menugameover.cpp"

int main(int argc, char* argv[]) {
    if (!initializeSDL()) {
        return 1;
    }
 
    SDL_Window* window = SDL_CreateWindow("Space Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface* backgroundSurface = IMG_Load("image/background.png");
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    SDL_Surface* characterSurface = IMG_Load("image/character.png");
    SDL_Texture* characterTexture = SDL_CreateTextureFromSurface(renderer, characterSurface);
    SDL_FreeSurface(characterSurface);

    SDL_Surface* enemySurface = IMG_Load("image/enemy.png");
    SDL_Texture* enemyTexture = SDL_CreateTextureFromSurface(renderer, enemySurface);
    SDL_FreeSurface(enemySurface);

    SDL_Surface* bulletSurface = IMG_Load("image/bullet.png");
    SDL_Texture* bulletTexture = SDL_CreateTextureFromSurface(renderer, bulletSurface);
    SDL_FreeSurface(bulletSurface);

    SDL_Surface* HPSurface = IMG_Load("image/HP.png");
    SDL_Texture* HPTexture = SDL_CreateTextureFromSurface(renderer, HPSurface);
    SDL_FreeSurface(HPSurface);

    Mix_Chunk* startSound = Mix_LoadWAV("sound/start.mp3");
    Mix_Chunk* explosionSound = Mix_LoadWAV("sound/bomb.wav");
    Mix_Chunk* clickSound = Mix_LoadWAV("sound/click.mp3");
    Mix_Chunk* shootSound = Mix_LoadWAV("sound/shoot.wav");

    TTF_Font* font = loadFont("arial.ttf", 24);


    Character player(50, 300, 10, 1000, 600);
    std::vector<Enemy> enemies;

    bool isRunning = true;
    bool gameOver = false;
    bool playAgain = false;

    SDL_Event event;
    srand(time(NULL));

    showMenu(renderer, font);
    countdown(renderer, font);

    while (isRunning ) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP: player.moveUp(); break;
                    case SDLK_DOWN: player.moveDown(); break;
                    case SDLK_LEFT: player.moveLeft(); break;
                    case SDLK_RIGHT: player.moveRight(); break;
                    case SDLK_SPACE: player.shoot();
                    Mix_PlayChannel(-1, shootSound, 0);
                    break;
                }
              }
            }

        if (gameOver) {
            if (showMenuOver(renderer, font, player)) {
                Mix_PlayChannel(-1, clickSound, 0);
                resetGame(player, enemies);
                gameOver = false;
                countdown(renderer, font);
            } else {
                isRunning = false;
            }
        }
        player.updatePosition();
        std::vector<Enemy> remainingEnemies;

        for (const auto& enemy : enemies) {
            if (!player.isCollidingWithEnemy(enemy)) {
                remainingEnemies.push_back(enemy);
            } else {
                Mix_PlayChannel(-1, explosionSound, 0);
                player.decreaseHP(1);
            }
        }
        enemies = remainingEnemies;

        if (rand() % 250 == 0) {
            int enemyY = rand() % 550;
            enemies.emplace_back(1000, enemyY, 2.5);
        }

        for (size_t i = 0; i < enemies.size(); ) {
            enemies[i].move();
            const std::vector<Bullet>& playerBullets = player.getBullets();
            for (size_t j = 0; j < playerBullets.size(); ++j) {
                const Bullet& bullet = playerBullets[j];
                if (bullet.getX() >= enemies[i].getX() && bullet.getX() <= enemies[i].getX() + 50 &&
                    bullet.getY() >= enemies[i].getY() - 25 && bullet.getY() <= enemies[i].getY() + 50) {
                    player.increaseScore(1);
                    Mix_PlayChannel(-1, explosionSound, 0);
                    std::vector<Bullet>& mutableBullets = const_cast<std::vector<Bullet>&>(player.getBullets());
                    mutableBullets.erase(mutableBullets.begin() + j);
                    enemies.erase(enemies.begin() + i);
                    continue;
                }
            }
            if (enemies[i].isOffScreen()) {
                player.decreaseHP(1);
                enemies.erase(enemies.begin() + i);
            } else {
                ++i;
            }
            if(player.getHP() <= 0){
                gameOver = true;
                 continue;
            }
            if (gameOver) {
                showMenuOver(renderer, font, player);
                gameOver = false;
                continue;
            }
}

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        SDL_Rect characterRect = { player.getX(), player.getY(), 50, 50 };
        SDL_RenderCopy(renderer, characterTexture, NULL, &characterRect);
        std::string scoreText = "Score: " + std::to_string(player.getScore());
        renderText(renderer, font, scoreText, 10, 10);
        if(player.getScore() < player.getBestScore()){
        std::string bestScoreText = "Best Score: " + std::to_string(player.getBestScore());
        renderText(renderer, font, bestScoreText, 10, 40);
}
        else{
        std::string scoreText = "Best Score: " + std::to_string(player.getScore());
        renderText(renderer, font, scoreText, 10, 40);
}
        std::string HPText = "HP: " + std::to_string(player.getHP());
        renderText(renderer, font, HPText, 840, 10);
        int x = 0;
        int currentHP = player.getHP();
        int numHearts = std::min(currentHP, 3);
        for (int i = 0; i < numHearts; ++i) {
        SDL_Rect HPRect = { 900 + x , 10, 30, 30 };
        SDL_RenderCopy(renderer, HPTexture, NULL, &HPRect);
        x += 30;
        }
        for (const auto& enemy : enemies) {
            SDL_Rect enemyRect = { enemy.getX(), enemy.getY(), 50, 50};
            SDL_RenderCopy(renderer, enemyTexture, NULL, &enemyRect);
        }
        for (const auto& bullet : player.getBullets()) {
            SDL_Rect bulletRect = { bullet.getX(), bullet.getY(), 50, 50 };
            SDL_RenderCopy(renderer, bulletTexture, NULL, &bulletRect);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(characterTexture);
    SDL_DestroyTexture(enemyTexture);
    SDL_DestroyTexture(bulletTexture);
    SDL_DestroyTexture(HPTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    Mix_Quit();
    return 0;
}
