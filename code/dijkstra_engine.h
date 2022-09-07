#pragma once
#include "dijkstra_types.h"
#include <SDL_Render.h>
#include <SDL_scancode.h>

struct game;

struct input
{
    struct
    {
        s32 X;
        s32 Y;
        b32 LeftButton;
        b32 RightButton;
    } Mouse;

    b32 KeyDown[SDL_NUM_SCANCODES];
    b32 KeyPressed[SDL_NUM_SCANCODES];
};

struct engine
{
    input Input;

    SDL_Window* Window;
    SDL_Renderer* Renderer;
};

b32 InitEngine(engine* Engine, game* Game);