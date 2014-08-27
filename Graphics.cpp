#include "Graphics.h"
#include "Game.h"

Graphics::Graphics(Game& game)
    : mGame(game)
{
}

Graphics::~Graphics()
{
    TTF_Quit();
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void Graphics::handleInput()
{
    SDL_Event e;

    while (SDL_PollEvent(&e))
    {
        switch(e.type)
        {
        case SDL_QUIT:
            mGame.stop();
            break;
        default:
            break;
        }
    }
}

void Graphics::draw()
{
    SDL_SetRenderDrawColor(mRenderer, 0,0,0, 255);
    SDL_RenderClear(mRenderer);

    vector<const Tile*> tiles = mGame.getActiveTiles();

    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

    for(const Tile* tile: tiles)
    {
        vector2 root = tile->getRectPos();

        for(vector2 rect: tile->getActiveRects())
        {
            int32_t x = root.X + rect.X;
            int32_t y = root.Y + rect.Y;

            const auto scale = 5;

            SDL_Rect sdlrect = {x * scale, y * scale, scale, scale};
            SDL_RenderFillRect(mRenderer, &sdlrect);
        }
    }

    stringstream text;
    text << "Generation #" << mGame.getRoundNumber();

    SDL_Color col = {255,255,255,255};
    SDL_Color bg = {0,0,0,0};
    SDL_Surface *text_surface = TTF_RenderText_Shaded(mFont, text.str().c_str(), col, bg);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(mRenderer, text_surface);
    SDL_FreeSurface(text_surface);

    int w = 200;
    int h = 30;
    SDL_Rect src = {0,0, w,h};
    SDL_Rect dst = {5,5, w,h};
    SDL_RenderCopy(mRenderer, texture, &src, &dst);

    SDL_RenderPresent(mRenderer);
}

bool Graphics::init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    mWindow = SDL_CreateWindow("Game of Life", 100, 100, 1024, 768, SDL_WINDOW_SHOWN);
    if (mWindow == nullptr)
    {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return false;
    }

    mRenderer = SDL_CreateRenderer(mWindow, 0, SDL_RENDERER_ACCELERATED);
    if (mRenderer == nullptr)
    {
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return false;
    }

    TTF_Init();
    mFont = TTF_OpenFont("./DejaVuSerif.ttf", 25);

    if(!mFont)
    {
        std::cout << "Couldn't open font: " << TTF_GetError() << std::endl;
        return false;
    }

    return true;
}

