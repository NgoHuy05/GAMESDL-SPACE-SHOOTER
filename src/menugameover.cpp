void resetGame(Character& player, std::vector<Enemy>& enemies) {
    player.setX(50);
    player.setY(300);
    player.setScore(0);
    player.setHP(3);
    enemies.clear();
}

bool showMenuOver(SDL_Renderer* renderer, TTF_Font* font,  Character& player) {
        SDL_Surface* background2Surface = IMG_Load("image/background2.png");
        SDL_Texture* background2Texture = SDL_CreateTextureFromSurface(renderer, background2Surface);
        SDL_FreeSurface(background2Surface);


        SDL_Surface* gameoverSurface = IMG_Load("image/gameover.png");
        SDL_Texture* gameoverTexture = SDL_CreateTextureFromSurface(renderer, gameoverSurface);
        SDL_FreeSurface(gameoverSurface);


        SDL_Surface* restartSurface = IMG_Load("image/restart.png");
        SDL_Texture* restartTexture = SDL_CreateTextureFromSurface(renderer, restartSurface);
        SDL_FreeSurface(restartSurface);

        SDL_Surface* exitSurface = IMG_Load("image/exit.png");
        SDL_Texture* exitTexture = SDL_CreateTextureFromSurface(renderer, exitSurface);
        SDL_FreeSurface(exitSurface);

        Mix_Chunk* gameoverSound = Mix_LoadWAV("sound/gameover.wav");
        Mix_Chunk* clickSound = Mix_LoadWAV("sound/click.mp3");
        Mix_PlayChannel(-1, gameoverSound, 0);

        SDL_Event event;
        bool startGame = false;
        bool playAgain = true;

        while (!startGame) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    exit(0);
                } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);

                    SDL_Rect restartButtonRect = { 335, 300, 250, 75 };
                    SDL_Rect exitButtonRect = { 330, 400,  250, 70 };

                    if (isMouseOverButton(mouseX, mouseY, restartButtonRect)) {
                        playAgain = true;
                        startGame = true;
                    } else if (isMouseOverButton(mouseX, mouseY, exitButtonRect)) {
                        exit(0);
                    }
                }
            }
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            bool mouseOverRestartButton = isMouseOverButton(mouseX, mouseY, {335, 300, 250, 75});
            bool mouseOverExitButton = isMouseOverButton(mouseX, mouseY, {330, 400, 250, 70});

            player.updateBestScore();
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, background2Texture, NULL, NULL);
            SDL_Rect gameoverRect = { 330, 80, 250, 80 };

            SDL_RenderCopy(renderer, gameoverTexture, NULL, &gameoverRect);
            SDL_Rect restartRect = { 335, 300, 250, 75 };
            if (mouseOverRestartButton) {
                SDL_SetTextureColorMod(restartTexture, 255, 255, 100);
            } else {
                SDL_SetTextureColorMod(restartTexture, 255, 255, 255);
            }

            SDL_RenderCopy(renderer, restartTexture, NULL, &restartRect);
            SDL_Rect exitRect = { 330, 400, 250, 70 };
            if (mouseOverExitButton) {
                SDL_SetTextureColorMod(exitTexture, 255, 255, 100);
            } else {
                SDL_SetTextureColorMod(exitTexture, 255, 255, 255);
            }
            SDL_RenderCopy(renderer, exitTexture, NULL, &exitRect);
            std::string scoreText = "Your Score: " + std::to_string(player.getScore());
            renderText(renderer, font, scoreText, 390, 180);
            std::string bestScoreText = "Best Score: " + std::to_string(player.getBestScore());
            renderText(renderer, font, bestScoreText, 390, 230);
            SDL_RenderPresent(renderer);

        }
        Mix_FreeChunk(clickSound);
        SDL_DestroyTexture(background2Texture);
        SDL_DestroyTexture(gameoverTexture);
        SDL_DestroyTexture(restartTexture);
        SDL_DestroyTexture(exitTexture);

        return startGame;
    }
