#pragma once

#include <queue>
#include <stdexcept>

#include "RootedGraph.h"

template <class Vertex, class Key, class DirectedEdge>
void RootedGraph<Vertex, Key, DirectedEdge>::Build(const Vertex& root, size_t maxPathLength)
{
  std::queue<std::pair<Vertex, size_t>> vertexDistanceQueue;

  auto key = getKey(root);
  pathLengthFromVertexKey.clear();
  pathLengthFromVertexKey.insert(std::pair<Key, size_t>{ key, 0 });
  vertexDistanceQueue.push(std::pair<Vertex, size_t>{ root, 0 });

  while (!vertexDistanceQueue.empty() && vertexDistanceQueue.front().second <= maxPathLength)
  {
    auto vertex = vertexDistanceQueue.front().first;
    auto distance = vertexDistanceQueue.front().second;
    vertexDistanceQueue.pop();
    // Find all vertices adjacent to this one. Exclude those already in our graph.
    auto adjacentVertices = getAdjacentVertices(vertex);
    for (const auto& adjacentVertexPair : adjacentVertices)
    {
      auto newVertex = adjacentVertexPair.second;
      auto newKey = getKey(newVertex);
      auto findIter = pathLengthFromVertexKey.find(newKey);
      if (findIter == pathLengthFromVertexKey.end())
      {
        auto newDistance = distance + 1;
        pathLengthFromVertexKey.insert(std::pair<Key, size_t>{ newKey, newDistance });
        vertexDistanceQueue.push(std::pair<Vertex, size_t>{ newVertex, newDistance });
      }
    }
  }
}

template <class Vertex, class Key, class DirectedEdge>
std::vector<DirectedEdge> RootedGraph<Vertex, Key, DirectedEdge>::FindShortestPathToRoot(const Vertex& source)
{
  std::vector<DirectedEdge> shortestPath;
  auto shortestPathLength = GetShortestPathLength(source);
  shortestPath.reserve(shortestPathLength);

  auto vertex = source;
  for (int i = 0; i < shortestPathLength; ++i)
  {
    auto edge = GetShortestPathEdge(vertex);
    shortestPath.push_back(edge.first);
    vertex = edge.second;
  }

  return shortestPath;
}

template <class Vertex, class Key, class DirectedEdge>
void RootedGraph<Vertex, Key, DirectedEdge>::ReadFromStream(std::istream&)
{
}

template <class Vertex, class Key, class DirectedEdge>
void RootedGraph<Vertex, Key, DirectedEdge>::WriteToStream(std::ostream&)
{
}

template <class Vertex, class Key, class DirectedEdge>
size_t RootedGraph<Vertex, Key, DirectedEdge>::SafeGetShortestPathLength(const Vertex& vertex)
{
  auto findIter = pathLengthFromVertexKey.find(getKey(vertex));
  if (findIter == pathLengthFromVertexKey.end())
    return -1;
  return findIter->second;
}

template <class Vertex, class Key, class DirectedEdge>
size_t RootedGraph<Vertex, Key, DirectedEdge>::GetShortestPathLength(const Vertex& vertex)
{
  auto length = SafeGetShortestPathLength(vertex);
  if (length == -1)
    throw std::runtime_error("Vertex not connected to root in graph");
  return length;
}

template <class Vertex, class Key, class DirectedEdge>
std::pair<DirectedEdge, Vertex> RootedGraph<Vertex, Key, DirectedEdge>::GetShortestPathEdge(const Vertex& vertex)
{
  auto length = GetShortestPathLength(vertex);
  auto adjacentVertices = getAdjacentVertices(vertex);
  for (const auto& adjacentVertexPair : adjacentVertices)
  {
    const auto& nextVertex = adjacentVertexPair.second;
    if (GetShortestPathLength(nextVertex) + 1 == length)
      return adjacentVertexPair;
  }

  throw std::runtime_error("No edge found along the shortest path");
}

