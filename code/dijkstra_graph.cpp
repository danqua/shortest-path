#include "dijkstra_graph.h"
#include <stdlib.h>
#include <limits.h>

static void ResetNodes(graph* Graph)
{
    node* NodePtr = Graph->Nodes;
    while (NodePtr != &Graph->Nodes[Graph->NumNodes])
    {
        NodePtr->Cost = INT_MAX;
        NodePtr->Visited = false;
        NodePtr->PrevNode = -1;
        NodePtr++;
    }
}

void NodeBufferAdd(node_buffer* Buffer, s32 Element)
{
    // Make sure to only add elements that aren't in the buffer yet.
    for (s32 i = 0; i < Buffer->Size; i++)
    {
        if (Buffer->Data[i] == Element)
        {
            return;
        }
    }
    Buffer->Data[Buffer->Size] = Element;
    Buffer->Size++;
}

void NodeBufferRemove(node_buffer* Buffer, s32 Element)
{
    for (s32 i = 0; i < Buffer->Size; i++)
    {
        if (Buffer->Data[i] == Element)
        {
            s32 LastElement = Buffer->Data[Buffer->Size - 1];
            Buffer->Data[Buffer->Size - 1] = 0;
            Buffer->Data[i] = LastElement;
            Buffer->Size--;
            return;
        }
    }
}

s32 FindNodeWithLowestCost(const node* Nodes, const node_buffer* Buffer)
{
    s32 Result = -1;
    s32 Cost = INT_MAX;
    for (s32 i = 0; i < Buffer->Size; i++)
    {
        s32 NodeIndex = Buffer->Data[i];
        s32 NodeCost = Nodes[NodeIndex].Cost;
        if (NodeCost < Cost)
        {
            Cost = NodeCost;
            Result = NodeIndex;
        }
    }
    return Result;
}

void InitGraph(graph* Graph, s32 NumNodes)
{
    Graph->Nodes = (node*)malloc(sizeof(node) * NumNodes);
    Graph->Edges = (edge*)malloc(sizeof(edge) * NumNodes * 4);
}

void RebuildGraph(graph* Graph, s32* Grid, s32 Cols, s32 Rows, s32 Ignore)
{
    Graph->PathLength = 0;
    Graph->NumNodes = 0;
    Graph->NumEdges = 0;

    for (s32 Y = 0; Y < Rows; Y++)
    {
        for (s32 X = 0; X < Cols; X++)
        {
            s32 Index = X + Y * Cols;

            if (Grid[Index] == Ignore) continue;

            s32 NumEdges = 0;
            s32 FirstEdge = Graph->NumEdges;

            // West
            if (X - 1 >= 0)
            {
                s32 WestIndex = (X - 1) + Y * Cols;
                if (Grid[WestIndex] != Ignore)
                {
                    Graph->Edges[Graph->NumEdges].Front = Index;
                    Graph->Edges[Graph->NumEdges].Back = WestIndex;
                    Graph->Edges[Graph->NumEdges].Weight = 1;
                    Graph->NumEdges++;
                    NumEdges++;
                }
            }
            // East
            if (X + 1 < Cols)
            {
                s32 EastIndex = (X + 1) + Y * Cols;
                if (Grid[EastIndex] != Ignore)
                {
                    Graph->Edges[Graph->NumEdges].Front = Index;
                    Graph->Edges[Graph->NumEdges].Back = EastIndex;
                    Graph->Edges[Graph->NumEdges].Weight = 1;
                    Graph->NumEdges++;
                    NumEdges++;
                }
            }
            // North
            if (Y - 1 >= 0)
            {
                s32 NorthIndex = X + (Y - 1) * Cols;
                if (Grid[NorthIndex] != Ignore)
                {
                    Graph->Edges[Graph->NumEdges].Front = Index;
                    Graph->Edges[Graph->NumEdges].Back = NorthIndex;
                    Graph->Edges[Graph->NumEdges].Weight = 1;
                    Graph->NumEdges++;
                    NumEdges++;
                }
            }
            // South
            if (Y + 1 < Rows)
            {
                s32 SouthIndex = X + (Y + 1) * Cols;
                if (Grid[SouthIndex] != Ignore)
                {
                    Graph->Edges[Graph->NumEdges].Front = Index;
                    Graph->Edges[Graph->NumEdges].Back = SouthIndex;
                    Graph->Edges[Graph->NumEdges].Weight = 1;
                    Graph->NumEdges++;
                    NumEdges++;
                }
            }
            Graph->Nodes[Index].Cost = INT_MAX;
            Graph->Nodes[Index].PrevNode = -1;
            Graph->Nodes[Index].Visited = false;
            Graph->Nodes[Index].FirstEdge = FirstEdge;
            Graph->Nodes[Index].NumEdges = NumEdges;
            Graph->NumNodes++;
        }
    }
}

bool FindShortestPath(graph* Graph, s32 Start, s32 End)
{
    node_buffer NodeBuffer = {};

    ResetNodes(Graph);
    NodeBufferAdd(&NodeBuffer, Start);
    Graph->Nodes[Start].Cost = 0;

    while (NodeBuffer.Size != 0)
    {
        s32 NodeIndex = FindNodeWithLowestCost(Graph->Nodes, &NodeBuffer);
        NodeBufferRemove(&NodeBuffer, NodeIndex);
        node* Front = &Graph->Nodes[NodeIndex];
        Front->Visited = true;

        // Get all adjacent nodes
        for (s32 EdgeIndex = Front->FirstEdge; EdgeIndex < Front->FirstEdge + Front->NumEdges; EdgeIndex++)
        {
            if (EdgeIndex == -1) continue;

            edge* Edge = &Graph->Edges[EdgeIndex];
            node* Back = &Graph->Nodes[Edge->Back];
            s32 Cost = Front->Cost + Edge->Weight;

            // Check if the costs from the current node to the adjacent node is less than
            // the currently set costs of the adjacent node. If the costs to travel are less
            // then update the node, otherwise leave it as is.
            if (Cost < Back->Cost)
            {
                Back->PrevNode = Edge->Front;
                Back->Cost = Cost;
                NodeBufferAdd(&NodeBuffer, Edge->Back);
            }
        }
    }

    // No path available
    if (Graph->Nodes[End].PrevNode == -1)
        return false;

    Graph->PathLength = 0;
    while (Start != End)
    {
        Graph->Path[Graph->PathLength] = End;
        End = Graph->Nodes[End].PrevNode;
        Graph->PathLength++;
    }
    Graph->Path[Graph->PathLength] = Start;
    Graph->PathLength++;

    return true;
}