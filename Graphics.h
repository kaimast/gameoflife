#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "defines.h"
#include <SDL2/SDL.h>

class Game;

class Graphics
{
public:
    Graphics(Game& game);
    ~Graphics();

    bool init();
    void handleInput();
    void draw();

private:
    Game& mGame;

    SDL_Window* mWindow;
    SDL_Renderer *mRenderer;
};

#endif // GRAPHICS_H
