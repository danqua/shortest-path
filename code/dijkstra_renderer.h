#pragma once
#include "dijkstra_types.h"
#include <SDL_render.h>

struct render_tile
{
    s32 X;
    s32 Y;
    s32 Width;
    s32 Height;
    SDL_Color Color;
};

void InitRenderer(SDL_Renderer* Renderer);

void RendererDrawGridLines(s32 Cols, s32 Rows, s32 CellSize);
void RendererPushTile(render_tile Tile);
void RendererDrawTiles();
void RendererNextFrame();