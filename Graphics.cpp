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
        case SDL_MOUSEWHEEL:
            mScale += e.wheel.y * 0.1;
            break;
        case SDL_MOUSEMOTION:
            if(e.motion.state & SDL_BUTTON(1)) {
                mPosition.X -= e.motion.xrel * 0.1 * mScale * mScale;
                mPosition.Y -= e.motion.yrel * 0.1 * mScale * mScale;
            }
            break;
        case SDL_KEYDOWN:
            switch(e.key.keysym.sym)
            {
            case SDLK_p:
                mGame.togglePaused();
                break;
            case SDLK_k:
                mGame.increaseSpeed();
                break;
            case SDLK_l:
                mGame.decreaseSpeed();
                break;
            case SDLK_i:
                mScale += 0.25;
                break;
            case SDLK_o:
                mScale -= 0.25;
                break;
            case SDLK_LEFT:
                mPosition.X -= 1;
                break;
            case SDLK_RIGHT:
                mPosition.X += 1;
                break;
            case SDLK_DOWN:
                mPosition.Y += 1;
                break;
            case SDLK_UP:
                mPosition.Y -= 1;
                break;
            }
            break;
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
    mGame.notifyRenderStart();

    SDL_SetRenderDrawColor(mRenderer, 0,0,0, 255);
    SDL_RenderClear(mRenderer);

    vector<const Tile*> tiles = mGame.getActiveTiles();

    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

    for(const Tile* tile: tiles)
    {
        vector2 root = tile->getRectPos();

        for(vector2 rect: tile->getActiveRects())
        {
            int32_t x = root.X + rect.X - mPosition.X;
            int32_t y = root.Y + rect.Y - mPosition.Y;

            int32_t size = max<int32_t>(mScale, 1);

            SDL_Rect sdlrect = {x * mScale, y * mScale, size, size};
            SDL_RenderFillRect(mRenderer, &sdlrect);
        }
    }

    stringstream generation;
    generation << "Generation #" << mGame.getRoundNumber();

    drawText(generation.str(), vector2(5,5));

    stringstream speed;
    speed << "(Minimum) Round Length: ";

    if(mGame.getSpeed() == 0)
        speed << "unlimited";
    else
        speed << mGame.getSpeed() << "ms";

    drawText(speed.str(), vector2(300, 5));

    SDL_RenderPresent(mRenderer);

    mGame.notifyRenderStop();
}

void Graphics::drawText(const string& text, const vector2& pos)
{
    SDL_Color col = {255,255,255,255};
    SDL_Color bg = {0,0,0,0};
    SDL_Surface *text_surface = TTF_RenderText_Shaded(mFont, text.c_str(), col, bg);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(mRenderer, text_surface);
    SDL_FreeSurface(text_surface);

    int w = 0,h = 0;
    TTF_SizeText(mFont, text.c_str(), &w, &h);

    SDL_Rect src = {0,0, w,h};
    SDL_Rect dst = {pos.X, pos.Y, w,h};
    SDL_RenderCopy(mRenderer, texture, &src, &dst);
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
    mFont = TTF_OpenFont("./DejaVuSerif.ttf", 15);

    if(!mFont)
    {
        std::cout << "Couldn't open font: " << TTF_GetError() << std::endl;
        return false;
    }

    return true;
}

