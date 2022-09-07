#include "dijkstra_renderer.h"

#define MAX_RENDER_TILES 512

static SDL_Renderer* Renderer;
static render_tile Tiles[MAX_RENDER_TILES];
static s32 NumTiles;

void InitRenderer(SDL_Renderer* InRenderer)
{
    Renderer = InRenderer;
}

void RendererDrawGridLines(s32 Cols, s32 Rows, s32 CellSize)
{
    SDL_SetRenderDrawColor(Renderer, 16, 16, 16, 255);

    s32 MaxX = Cols * CellSize;
    s32 MaxY = Rows * CellSize;

    // Horizontal lines
    for (s32 Y = 1; Y < Rows; Y++)
    {
        s32 X1 = 0;
        s32 Y1 = Y * CellSize;
        s32 X2 = MaxX;
        s32 Y2 = Y1;
        SDL_RenderDrawLine(Renderer, X1, Y1, X2, Y2);
    }

    // Vertical lines
    for (s32 X = 1; X < Cols; X++)
    {
        s32 X1 = X * CellSize;
        s32 Y1 = 0;
        s32 X2 = X1;
        s32 Y2 = MaxY;
        SDL_RenderDrawLine(Renderer, X1, Y1, X2, Y2);
    }
}

void RendererPushTile(render_tile Tile)
{
    Tiles[NumTiles] = Tile;
    NumTiles++;
}

void RendererDrawTiles()
{
    for (s32 i = 0; i < NumTiles; i++)
    {
        SDL_Rect Rect = {};
        Rect.x = Tiles[i].X;
        Rect.y = Tiles[i].Y;
        Rect.w = Tiles[i].Width;
        Rect.h = Tiles[i].Height;

        u8 R = Tiles[i].Color.r;
        u8 G = Tiles[i].Color.g;
        u8 B = Tiles[i].Color.b;
        u8 A = Tiles[i].Color.a;

        SDL_SetRenderDrawColor(Renderer, R, G, B, A);
        SDL_RenderFillRect(Renderer, &Rect);
    }
}

void RendererNextFrame()
{
    NumTiles = 0;
}

