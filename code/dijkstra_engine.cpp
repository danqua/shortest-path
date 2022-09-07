#include "dijkstra_engine.h"
#include "dijkstra_config.h"
#include "dijkstra_renderer.h"
#include "dijkstra_game.h"
#include <SDL.h>

static b32 PollEvents(engine* Engine)
{
    input* Input = &Engine->Input;
    SDL_memset(Input->KeyPressed, 0, sizeof(b32) * SDL_NUM_SCANCODES);

    SDL_Event Event = {};
    while (SDL_PollEvent(&Event))
    {
        if (Event.type == SDL_QUIT)
        {
            return false;
        }
        else if (Event.type == SDL_KEYDOWN)
        {
            if (Event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            {
                return false;
            }
            
            if (!Input->KeyDown[Event.key.keysym.scancode])
            {
                Input->KeyPressed[Event.key.keysym.scancode] = true;
            }
            Input->KeyDown[Event.key.keysym.scancode] = true;
        }
        else if (Event.type == SDL_KEYUP)
        {
            Input->KeyPressed[Event.key.keysym.scancode] = false;
            Input->KeyDown[Event.key.keysym.scancode] = false;
        }
        else if (Event.type == SDL_MOUSEMOTION)
        {
            Engine->Input.Mouse.X = Event.motion.x;
            Engine->Input.Mouse.Y = Event.motion.y;
        }
        else if (Event.type == SDL_MOUSEBUTTONDOWN)
        {
            if (Event.button.button == SDL_BUTTON_LEFT) Engine->Input.Mouse.LeftButton = true;
            if (Event.button.button == SDL_BUTTON_RIGHT) Engine->Input.Mouse.RightButton = true;
        }
        else if (Event.type == SDL_MOUSEBUTTONUP)
        {
            if (Event.button.button == SDL_BUTTON_LEFT) Engine->Input.Mouse.LeftButton = false;
            if (Event.button.button == SDL_BUTTON_RIGHT) Engine->Input.Mouse.RightButton = false;
        }
    }
    return true;
}

static void Run(engine* Engine, game* Game)
{
    b32 Running = true;
    u64 LastTime = SDL_GetPerformanceCounter();

    Game->Engine = Engine;
    GameInit(Game);

    while (PollEvents(Engine))
    {
        u64 ElapsedTime = SDL_GetPerformanceCounter() - LastTime;
        LastTime = SDL_GetPerformanceCounter();

        SDL_SetRenderDrawColor(Engine->Renderer, 0, 0, 0, 255);
        SDL_RenderClear(Engine->Renderer);
        GameUpdateAndRender(Game);
        SDL_RenderPresent(Engine->Renderer);
    }

    SDL_DestroyRenderer(Engine->Renderer);
    SDL_DestroyWindow(Engine->Window);
    SDL_Quit();
}

b32 InitEngine(engine* Engine, game* Game)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return false;

    SDL_Window* Window = SDL_CreateWindow(
        "Dijkstra Pathfinding",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_W,
        WINDOW_H,
        SDL_WINDOW_SHOWN
    );

    if (!Window) return false;

    SDL_Renderer* Renderer = SDL_CreateRenderer(
        Window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!Renderer) return false;

    Engine->Window = Window;
    Engine->Renderer = Renderer;

    InitRenderer(Renderer);
    Run(Engine, Game);

    return true;
}