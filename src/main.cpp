#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <bits/stdc++.h>
#include "Bullet.cpp"
#include "Enemy.cpp"
#include "Threat.cpp"
#include "Boss.cpp"
#include "Character.cpp"
#include "initialize.cpp"
#include "Menu.cpp"
#include "Menugameover.cpp"
#include "highscore.cpp"



int main(int argc, char* argv[]) {
    if (!initializeSDL()) {
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Space Shooter create by Huyvjppro", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 600, SDL_WINDOW_SHOWN);
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

    SDL_Surface* threatSurface = IMG_Load("image/threat.png");
    SDL_Texture* threatTexture = SDL_CreateTextureFromSurface(renderer, threatSurface);
    SDL_FreeSurface(threatSurface);

    SDL_Surface* bossSurface = IMG_Load("image/boss.png");
    SDL_Texture* bossTexture = SDL_CreateTextureFromSurface(renderer, bossSurface);
    SDL_FreeSurface(bossSurface);

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

    TTF_Font* font = loadFont("image/arial.ttf", 24);

    const std::string highScoreFile = "highscore.txt";
    int bestScore = readHighScore(highScoreFile);
    Character player(50, 300, 10, 1000, 600);
    Boss* boss = nullptr;

    player.setBestScore(bestScore);
    std::vector<Enemy> enemies;
    std::vector<Threat> threats;

    bool isRunning = true;
    bool gameOver = false;
    bool playAgain = false;
    bool bossSpawned = false;

    SDL_Event event;
    srand(time(NULL));

    showMenu(renderer, font);

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
                    case SDLK_SPACE: player.shoot();  Mix_PlayChannel(-1, shootSound, 0); break;
                    case SDLK_RETURN: showPauseMenu(renderer,font); Mix_PlayChannel(-1, clickSound, 0); break;
                }
              }
            }
        if (gameOver) {
            if (showMenuOver(renderer, font, player)) {
                Mix_PlayChannel(-1, clickSound, 0);
                resetGame(player, enemies, threats, boss);
                gameOver = false;
                bossSpawned = false;
                delete boss;
                boss = nullptr;
            } else {
                isRunning = false;
            }
            continue;
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
        if (rand() % 150 == 0) {
            int enemyY = rand() % 550;
            enemies.emplace_back(1000, enemyY, 1.5);
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
            if(player.getHP() == 0){
                gameOver = true;
                 if (player.getScore() > bestScore) {
                    bestScore = player.getScore();
                    writeHighScore(highScoreFile, bestScore);
                }
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



        if (player.getScore() == 25 && !bossSpawned) {
            boss = new Boss(880, 250, 1.5, 1000);
            enemies.clear();
            bossSpawned = true;


        }

    if (boss) {
        boss->move();
        enemies.clear();
        std::vector<Threat> remainingThreats;
        for (const auto& threat : threats) {
            if (!player.isCollidingWithThreat(threat)) {
                remainingThreats.push_back(threat);
            } else {
                Mix_PlayChannel(-1, explosionSound, 0);
                player.decreaseHP(1);
            }
        }
        threats = remainingThreats;

        if (rand() % 80 == 0) {
            int threatY = rand() % 550;
            threats.emplace_back(1000, threatY, 5);
}
        for (auto& threat : threats) {
            threat.move();
        }
        SDL_Rect bossRect = { boss->getX(), boss->getY(), 120, 120 };
        SDL_RenderCopy(renderer, bossTexture, NULL, &bossRect);
        std::string HPBOSSText = "HP BOSS: " + std::to_string(boss->getHP());
        renderText(renderer, font, HPBOSSText, 500, 10);

    for (size_t i = 0; i < threats.size(); ++i) {
        for (size_t j = 0; j < player.getBullets().size(); ++j) {
            const Bullet& bullet = player.getBullets()[j];
            const Threat& threat = threats[i];
            if (bullet.getX() >= threat.getX() && bullet.getX() <= threat.getX() + 80 &&
                bullet.getY() >= threat.getY() && bullet.getY() - 40 <= threat.getY() + 80) {
                Mix_PlayChannel(-1, explosionSound, 0);
                std::vector<Bullet>& mutableBullets = const_cast<std::vector<Bullet>&>(player.getBullets());
                mutableBullets.erase(mutableBullets.begin() + j);
                break;
            }
        }
    }
    std::vector<Bullet>& playerBullets = const_cast<std::vector<Bullet>&>(player.getBullets());
    for (size_t i = 0; i < playerBullets.size(); ++i) {
        const Bullet& bullet = playerBullets[i];
        if (bullet.getX() >= boss->getX() && bullet.getX() <= boss->getX() + 120 &&
            bullet.getY() >= boss->getY() && bullet.getY() <= boss->getY() + 120) {
            Mix_PlayChannel(-1, explosionSound, 0);
            boss->decreaseHP(1);
            playerBullets.erase(playerBullets.begin() + i);
            break;
        }
    }
    if (boss->getHP() <= 0) {
        delete boss;
        boss = nullptr;
        player.increaseScore(13122005);
        bossSpawned = false;
    }

}
        for (const auto& threat : threats) {
        SDL_Rect threatRect = { threat.getX(), threat.getY(), 80, 80 };
        SDL_RenderCopy(renderer, threatTexture, NULL, &threatRect);
    }
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
    delete boss;

    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(characterTexture);
    SDL_DestroyTexture(enemyTexture);
    SDL_DestroyTexture(bulletTexture);
    SDL_DestroyTexture(HPTexture);
    SDL_DestroyTexture(threatTexture);
    SDL_DestroyTexture(bossTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    Mix_Quit();
    return 0;
}
