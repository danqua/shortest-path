#pragma once
#include "dijkstra_types.h"
#include "dijkstra_graph.h"

struct engine;

enum user_state
{
    USER_STATE_DRAW_WALL,
    USER_STATE_DRAW_START,
    USER_STATE_DRAW_END,
    USER_STATE_ERASE
};

enum tile_type
{
    TILE_TYPE_NONE,
    TILE_TYPE_WALL,
    TILE_TYPE_START,
    TILE_TYPE_END
};

struct tile_map
{
    s32 Cols;
    s32 Rows;
    s32* Data;
};

struct game
{
    s32 StartIndex;
    s32 EndIndex;
    s32 UserState;
    s32 PrevUserState;
    tile_map TileMap;
    graph Graph;
    engine* Engine;
};

void GameInit(game* Game);
void GameUpdateAndRender(game* Game);