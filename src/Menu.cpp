bool isMouseOverButton(int mouseX, int mouseY, const SDL_Rect& buttonRect) {
    return (mouseX >= buttonRect.x && mouseX <= (buttonRect.x + buttonRect.w) &&
            mouseY >= buttonRect.y && mouseY <= (buttonRect.y + buttonRect.h));
}
bool showMenu(SDL_Renderer* renderer, TTF_Font* font) {
        SDL_Surface* background2Surface = IMG_Load("image/background2.png");
        SDL_Texture* background2Texture = SDL_CreateTextureFromSurface(renderer, background2Surface);
        SDL_FreeSurface(background2Surface);

        SDL_Surface* titleSurface = IMG_Load("image/title.png");
        SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
        SDL_FreeSurface(titleSurface);

        SDL_Surface* playSurface = IMG_Load("image/play.png");
        SDL_Texture* playTexture = SDL_CreateTextureFromSurface(renderer, playSurface);
        SDL_FreeSurface(playSurface);

        SDL_Surface* exitSurface = IMG_Load("image/exit.png");
        SDL_Texture* exitTexture = SDL_CreateTextureFromSurface(renderer, exitSurface);
        SDL_FreeSurface(exitSurface);

        SDL_Surface* instructionSurface = IMG_Load("image/instruction.png");
        SDL_Texture* instructionTexture = SDL_CreateTextureFromSurface(renderer, instructionSurface);
        SDL_FreeSurface(instructionSurface);

    SDL_Event event;
    bool startGame = false;
    bool instruction = false;
    Mix_Chunk* clickSound = Mix_LoadWAV("sound/click.mp3");
    while (!startGame) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                 exit(0);
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                SDL_Rect startButtonRect = { 330, 250, 250, 80 };
                SDL_Rect exitButtonRect = { 330, 450,  250, 80 };
                SDL_Rect instructionButtonRect = { 330, 350,  250, 80 };
                SDL_Rect exitsettingButtonRect = { 700, 480,  250, 80 };
                if (isMouseOverButton(mouseX, mouseY, startButtonRect)) {
                    Mix_PlayChannel(-1, clickSound, 0);
                    startGame = true;
                } else if (isMouseOverButton(mouseX, mouseY, exitButtonRect)) {
                    Mix_PlayChannel(-1, clickSound, 0);
                    exit(0);
                }
                else if (isMouseOverButton(mouseX, mouseY, instructionButtonRect)) {
                    Mix_PlayChannel(-1, clickSound, 0);
                    instruction = true;
                }
                else if(isMouseOverButton(mouseX, mouseY, exitsettingButtonRect)){
                    Mix_PlayChannel(-1, clickSound, 0);
                    instruction = false;
                }
            }

        }
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        bool mouseOverStartButton = isMouseOverButton(mouseX, mouseY, {330, 250, 250, 80});
        bool mouseOverExitButton = isMouseOverButton(mouseX, mouseY, {330, 450, 250, 80});
        bool mouseOverInstructionButton = isMouseOverButton(mouseX, mouseY, {330, 350, 250, 80});
        bool mouseOverexitsettingButton = isMouseOverButton(mouseX, mouseY, {700, 480, 250, 80});

        if (instruction) {
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, background2Texture, NULL, NULL);
            SDL_Rect exitRect = { 700, 480,  250, 80 };

            if (mouseOverexitsettingButton) {
                SDL_SetTextureColorMod(exitTexture, 255, 255, 100);
            } else {
                SDL_SetTextureColorMod(exitTexture, 255, 255, 255);
            }

            SDL_RenderCopy(renderer, exitTexture, NULL, &exitRect);
            renderText(renderer, font, "Flying up    : UP", 100, 100);
            renderText(renderer, font, "Flying down  : DOWN", 100, 150);
            renderText(renderer, font, "Flying left  : LEFT", 500, 100);
            renderText(renderer, font, "Flying right : RIGHT", 500, 150);
            renderText(renderer, font, "SHOOT: SPACE", 100, 200);
            renderText(renderer, font, "If you let the enemy cross the boundary, you will lose 1 HP", 100, 400);
            renderText(renderer, font, "If you collide with the enemy, you will lose 1 HP", 100, 450);
            renderText(renderer, font, "If you run out of HP, you will lose", 100, 500);
            renderText(renderer, font, "Tip: Try to eliminate all enemies, don't let any of them pass, and avoid colliding with the enemies", 100, 350);

            SDL_RenderPresent(renderer);

        }
        else{
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, background2Texture, NULL, NULL);
            SDL_Rect titleRect = { 330, 100, 250, 150 };
            SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
            SDL_Rect playRect = { 330, 250, 250, 80 };

            if (mouseOverStartButton) {
                SDL_SetTextureColorMod(playTexture, 255, 255, 100);
            } else {
                SDL_SetTextureColorMod(playTexture, 255, 255, 255);
            }

            SDL_RenderCopy(renderer, playTexture, NULL, &playRect);
            SDL_Rect exitRect = { 330, 450,  250, 80 };
            if (mouseOverExitButton) {
                SDL_SetTextureColorMod(exitTexture, 255, 255, 100);
            } else {
                SDL_SetTextureColorMod(exitTexture, 255, 255, 255);
            }

            SDL_RenderCopy(renderer, exitTexture, NULL, &exitRect);
            SDL_Rect instructionRect = { 330, 350,  250, 80 };
            if (mouseOverInstructionButton) {
                SDL_SetTextureColorMod(instructionTexture, 255, 255, 100);
            } else {
                SDL_SetTextureColorMod(instructionTexture, 255, 255, 255);
            }
            SDL_RenderCopy(renderer, instructionTexture, NULL, &instructionRect);
            SDL_RenderPresent(renderer);
        }
    }
    return startGame;
}

bool showPauseMenu(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Surface* backgroundSurface = IMG_Load("image/background.png");
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    SDL_Surface* exit2Surface = IMG_Load("image/exit2.png");
    SDL_Texture* exit2Texture = SDL_CreateTextureFromSurface(renderer, exit2Surface);
    SDL_FreeSurface(exit2Surface);

    SDL_Surface* continueSurface = IMG_Load("image/continue.png");
    SDL_Texture* continueTexture = SDL_CreateTextureFromSurface(renderer, continueSurface);
    SDL_FreeSurface(continueSurface);

    SDL_Event event;
    bool continueGame = false;
    Mix_Chunk* clickSound = Mix_LoadWAV("sound/click.mp3");

    while (!continueGame) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                exit(0);
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                SDL_Rect continueButtonRect = { 200, 300, 250, 80 };
                SDL_Rect exitButtonRect = { 500, 300, 250, 80 };

                if (isMouseOverButton(mouseX, mouseY, continueButtonRect)) {
                    continueGame = true;
                } else if (isMouseOverButton(mouseX, mouseY, exitButtonRect)) {
                    Mix_PlayChannel(-1, clickSound, 0);
                    showMenu(renderer, font);
                }
            }
        }

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        bool mouseOverContinueButton = isMouseOverButton(mouseX, mouseY, {200, 300, 250, 80});
        bool mouseOverExit2Button = isMouseOverButton(mouseX, mouseY, {500, 300, 250, 80});

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        SDL_Rect continueRect = { 200, 300, 250, 80 };
        if (mouseOverContinueButton) {
            SDL_SetTextureColorMod(continueTexture, 255, 255, 100);
        } else {
            SDL_SetTextureColorMod(continueTexture, 255, 255, 255);
        }
        SDL_RenderCopy(renderer, continueTexture, NULL, &continueRect);

        SDL_Rect exit2Rect = { 500, 300, 250, 80 };
        if (mouseOverExit2Button) {
            SDL_SetTextureColorMod(exit2Texture, 255, 255, 100);
        } else {
            SDL_SetTextureColorMod(exit2Texture, 255, 255, 255);
        }
        SDL_RenderCopy(renderer, exit2Texture, NULL, &exit2Rect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(exit2Texture);
    SDL_DestroyTexture(continueTexture);
    Mix_FreeChunk(clickSound);
    return continueGame;
}
