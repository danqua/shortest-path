#pragma once
#include "dijkstra_types.h"
#include "dijkstra_config.h"

struct edge
{
    s32 Front;
    s32 Back;
    s32 Weight;
};

struct node
{
    s32 Cost;
    s32 FirstEdge;
    s32 NumEdges;
    s32 PrevNode;
    b32 Visited;
};

struct node_buffer
{
    s32 Size;
    s32 Data[MAX_NODE_BUFFER_SIZE];
};

struct graph
{
    s32 NumEdges;
    edge* Edges;
    s32 NumNodes;
    node* Nodes;
    s32 PathLength;
    s32 Path[MAX_PATH_LENGTH];
};

void NodeBufferAdd(node_buffer* Buffer, s32 Element);
void NodeBufferRemove(node_buffer* Buffer, s32 Element);
s32 FindNodeWithLowestCost(const node* Nodes, const node_buffer* Buffer);
void RebuildGraph(graph* Graph, s32* Grid, s32 Cols, s32 Rows, s32 Ignore);
bool FindShortestPath(graph* Graph, s32 Start, s32 End);