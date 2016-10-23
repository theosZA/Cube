#pragma once

#include <iterator>
#include <queue>
#include <stdexcept>

#include "RootedGraph.h"
#include "..\BinaryStream.h"

template <class Vertex, class Key, class DirectedEdge>
void RootedGraph<Vertex, Key, DirectedEdge>::Build(const Vertex& root, std::uint32_t maxPathLength)
{
  std::queue<std::pair<Vertex, std::uint32_t>> vertexDistanceQueue;

  auto key = getKey(root);
  pathLengthFromVertexKey.clear();
  pathLengthFromVertexKey.insert(std::pair<Key, std::uint32_t>{ key, 0 });
  vertexDistanceQueue.push(std::pair<Vertex, std::uint32_t>{ root, 0 });

  while (!vertexDistanceQueue.empty() && vertexDistanceQueue.front().second < maxPathLength)
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
        pathLengthFromVertexKey.insert(std::pair<Key, std::uint32_t>{ newKey, newDistance });
        vertexDistanceQueue.push(std::pair<Vertex, std::uint32_t>{ newVertex, newDistance });
      }
    }
  }
}

template <class Vertex, class Key, class DirectedEdge>
std::vector<DirectedEdge> RootedGraph<Vertex, Key, DirectedEdge>::FindShortestPathToRoot(const Vertex& source) const
{
  std::vector<DirectedEdge> shortestPath;
  auto shortestPathLength = GetShortestPathLength(source);
  shortestPath.reserve(shortestPathLength);

  auto vertex = source;
  for (std::uint32_t i = 0; i < shortestPathLength; ++i)
  {
    auto edge = GetShortestPathEdge(vertex);
    shortestPath.push_back(edge.first);
    vertex = edge.second;
  }

  return shortestPath;
}

template <class Vertex, class Key, class DirectedEdge>
bool RootedGraph<Vertex, Key, DirectedEdge>::ReadFromStream(std::istream& in)
{
  if (!in.good())
    return false;

  pathLengthFromVertexKey.clear();
  while (in.good() && !in.eof())
    pathLengthFromVertexKey.insert(BinaryStream::Read<std::pair<Key, std::uint32_t>>(in));
  return true;
}

template <class Vertex, class Key, class DirectedEdge>
void RootedGraph<Vertex, Key, DirectedEdge>::WriteToStream(std::ostream& out) const
{
  for (const auto& vertexPair : pathLengthFromVertexKey)
    BinaryStream::Write(out, vertexPair);
}

template <class Vertex, class Key, class DirectedEdge>
std::uint32_t RootedGraph<Vertex, Key, DirectedEdge>::SafeGetShortestPathLength(const Vertex& vertex) const
{
  auto findIter = pathLengthFromVertexKey.find(getKey(vertex));
  if (findIter == pathLengthFromVertexKey.end())
    return -1;
  return findIter->second;
}

template <class Vertex, class Key, class DirectedEdge>
std::uint32_t RootedGraph<Vertex, Key, DirectedEdge>::GetShortestPathLength(const Vertex& vertex) const
{
  auto length = SafeGetShortestPathLength(vertex);
  if (length == -1)
    throw std::runtime_error("Vertex not connected to root in graph");
  return length;
}

template <class Vertex, class Key, class DirectedEdge>
std::pair<DirectedEdge, Vertex> RootedGraph<Vertex, Key, DirectedEdge>::GetShortestPathEdge(const Vertex& vertex) const
{
  auto length = GetShortestPathLength(vertex);
  auto adjacentVertices = getAdjacentVertices(vertex);
  for (const auto& adjacentVertexPair : adjacentVertices)
  {
    const auto& nextVertex = adjacentVertexPair.second;
    if (SafeGetShortestPathLength(nextVertex) + 1 == length)
      return adjacentVertexPair;
  }

  throw std::runtime_error("No edge found along the shortest path");
}

