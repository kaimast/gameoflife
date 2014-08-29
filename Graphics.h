#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "defines.h"
#include "vector2.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

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
    void drawText(const string& text, const vector2& pos);

    vector2 mPosition = vector2(0,0);
    float mScale = 5.0;

    Game& mGame;

    SDL_Window* mWindow;
    SDL_Renderer *mRenderer;

    TTF_Font* mFont;
};

#endif // GRAPHICS_H
