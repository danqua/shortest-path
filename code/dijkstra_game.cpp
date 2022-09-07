#include "dijkstra_game.h"
#include "dijkstra_renderer.h"
#include "dijkstra_engine.h"
#include "dijkstra_config.h"
#include <stdio.h>

static void ClearTileMap(tile_map* TileMap)
{
    s32 NumTiles = TileMap->Cols * TileMap->Rows;
    for (s32 I = 0; I < NumTiles; I++)
    {
        TileMap->Data[I] = TILE_TYPE_NONE;
    }
}

static void InitTileMap(tile_map* TileMap, s32 Cols, s32 Rows)
{
    s32 NumTiles = Cols * Rows;
    s32 SizeInBytes = NumTiles * sizeof(s32);
    TileMap->Cols = Cols;
    TileMap->Rows = Rows;
    TileMap->Data = (s32*)malloc(SizeInBytes);
    ClearTileMap(TileMap);
}

static s32 TileDataFromUserState(s32 State)
{
    s32 Result = TILE_TYPE_NONE;
    switch (State)
    {
        case USER_STATE_DRAW_WALL:  Result = TILE_TYPE_WALL; break;
        case USER_STATE_DRAW_START: Result = TILE_TYPE_START; break;
        case USER_STATE_DRAW_END:   Result = TILE_TYPE_END; break;
        case USER_STATE_ERASE:      Result = TILE_TYPE_NONE; break;
    }
    return Result;
}

static SDL_Color ColorFromUserState(s32 State)
{
    SDL_Color Result = {};
    
    switch (State)
    {
        case USER_STATE_DRAW_WALL:  Result = { 64, 64, 64, 255 };    break;
        case USER_STATE_DRAW_START: Result = { 81, 181, 108, 255 };  break;
        case USER_STATE_DRAW_END:   Result = { 81, 108, 181, 255 };  break;
        case USER_STATE_ERASE:      Result = { 181, 108, 108, 255 }; break;
    }

    return Result;
}

static void UpdateUserState(game* Game, input* Input)
{
    if (Input->KeyPressed[SDL_SCANCODE_1])
    {
        Game->UserState = USER_STATE_DRAW_WALL;
    }
    else if (Input->KeyPressed[SDL_SCANCODE_2])
    {
        Game->UserState = USER_STATE_DRAW_START;
    }
    else if (Input->KeyPressed[SDL_SCANCODE_3])
    {
        Game->UserState = USER_STATE_DRAW_END;
    }
    else if (Input->KeyPressed[SDL_SCANCODE_E])
    {
        Game->UserState = USER_STATE_ERASE;
    }
    else if (Input->KeyPressed[SDL_SCANCODE_C])
    {
        tile_map* TileMap = &Game->TileMap;
        ClearTileMap(TileMap);
        Game->StartIndex = -1;
        Game->EndIndex = -1;
        Game->Graph.PathLength = 0;
    }
    else if (Input->KeyPressed[SDL_SCANCODE_R])
    {
        Game->Graph.PathLength = 0;
    }
    else if (Input->KeyPressed[SDL_SCANCODE_SPACE])
    {
        graph* Graph = &Game->Graph;
        tile_map* TileMap = &Game->TileMap;
        RebuildGraph(Graph, TileMap->Data, TileMap->Cols, TileMap->Rows, TILE_TYPE_WALL);
        if (Game->StartIndex != -1 && Game->EndIndex != -1)
        {
            FindShortestPath(Graph, Game->StartIndex, Game->EndIndex);
        }
    }

    if (Input->Mouse.LeftButton)
    {
        tile_map* TileMap = &Game->TileMap;

        s32 MouseX = Input->Mouse.X;
        s32 MouseY = Input->Mouse.Y;
        s32 TileMapX = (MouseX / TILE_SIZE);
        s32 TileMapY = (MouseY / TILE_SIZE);
        s32 TileMapIndex = TileMapX + TileMapY * TileMap->Cols;
        s32 TileData = TileDataFromUserState(Game->UserState);

        // NOTE: It is possible that there is already a start or end point defined.
        // In that case we don't want to have multiple start and end points in the tile map.
        if (Game->UserState == USER_STATE_DRAW_START)
        {
            if (Game->StartIndex != -1)
            {
                TileMap->Data[Game->StartIndex] = TILE_TYPE_NONE;
            }
            Game->StartIndex = TileMapIndex;
        }

        if (Game->UserState == USER_STATE_DRAW_END)
        {
            if (Game->EndIndex != -1)
            {
                TileMap->Data[Game->EndIndex] = TILE_TYPE_NONE;
            }
            Game->EndIndex = TileMapIndex;
        }

        if (Game->UserState == USER_STATE_ERASE)
        {
            if (TileMap->Data[TileMapIndex] == TILE_TYPE_START)
            {
                Game->StartIndex = -1;
            }
            else if (TileMap->Data[TileMapIndex] == TILE_TYPE_END)
            {
                Game->EndIndex = -1;
            }
        }
        
        TileMap->Data[TileMapIndex] = TileData;
    }
}

static void DrawMousePosition(s32 MouseX, s32 MouseY, SDL_Color Color)
{
    render_tile MousePositionTile = {};
    MousePositionTile.X = (MouseX / TILE_SIZE) * TILE_SIZE;
    MousePositionTile.Y = (MouseY / TILE_SIZE) * TILE_SIZE;
    MousePositionTile.Width = TILE_SIZE;
    MousePositionTile.Height = TILE_SIZE;
    MousePositionTile.Color = Color;

    RendererPushTile(MousePositionTile);
}

static void DrawTileMap(tile_map* TileMap)
{
    for (s32 Y = 0; Y < TileMap->Rows; Y++)
    {
        for (s32 X = 0; X < TileMap->Cols; X++)
        {
            s32 TileIndex = X + Y * TileMap->Cols;

            render_tile RenderTile = {};
            RenderTile.X = X * TILE_SIZE;
            RenderTile.Y = Y * TILE_SIZE;
            RenderTile.Width = TILE_SIZE;
            RenderTile.Height = TILE_SIZE;

            if (TileMap->Data[TileIndex] == TILE_TYPE_WALL)
            {
                // TODO: There might be a better way to define colors for different
                // tile types. Exploiting the user state for the color generation is
                // maybe not the best solution here.
                RenderTile.Color = ColorFromUserState(USER_STATE_DRAW_WALL);
                RendererPushTile(RenderTile);
            }
            else if (TileMap->Data[TileIndex] == TILE_TYPE_START)
            {
                RenderTile.Color = ColorFromUserState(USER_STATE_DRAW_START);
                RendererPushTile(RenderTile);
            }
            else if (TileMap->Data[TileIndex] == TILE_TYPE_END)
            {
                RenderTile.Color = ColorFromUserState(USER_STATE_DRAW_END);
                RendererPushTile(RenderTile);
            }
        }
    }
}

void DrawShortestPath(graph* Graph, s32 Cols)
{
    for (s32 I = 0; I < Graph->PathLength; I++)
    {
        render_tile RenderTile = {};
        RenderTile.X = (Graph->Path[I] % Cols) * TILE_SIZE;
        RenderTile.Y = (Graph->Path[I] / Cols) * TILE_SIZE;
        RenderTile.Width = TILE_SIZE;
        RenderTile.Height = TILE_SIZE;
        RenderTile.Color = { 128, 128, 128, 255 };
        RendererPushTile(RenderTile);
    }
}

void GameInit(game* Game)
{
    Game->StartIndex = -1;
    Game->EndIndex = -1;

    tile_map* TileMap = &Game->TileMap;
    s32 Cols = WINDOW_W / TILE_SIZE;
    s32 Rows = WINDOW_H / TILE_SIZE;

    InitTileMap(TileMap, Cols, Rows);

    graph* Graph = &Game->Graph;
    s32 NumNodes = Cols * Rows;
    InitGraph(Graph, NumNodes);
}

void GameUpdateAndRender(game* Game)
{
    engine* Engine = Game->Engine;
    tile_map* TileMap = &Game->TileMap;
    graph* Graph = &Game->Graph;

    UpdateUserState(Game, &Engine->Input);

    RendererDrawGridLines(TileMap->Cols, TileMap->Rows, TILE_SIZE);
    DrawShortestPath(Graph, TileMap->Cols);
    DrawTileMap(TileMap);

    SDL_Color CursorColor = ColorFromUserState(Game->UserState);
    DrawMousePosition(Engine->Input.Mouse.X, Engine->Input.Mouse.Y, CursorColor);

    RendererDrawTiles();
    RendererNextFrame();
}