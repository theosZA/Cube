#pragma once

#include <istream>
#include <functional>
#include <ostream>
#include <unordered_map>
#include <utility>
#include <vector>

// A RootedGraph is a bi-directional graph where one vertex is is the root (or goal) vertex.
// We are primarily interested in the shortest path from a given vertex in the graph to the root vertex.
// In this implementation all edges are weight 1, and vertices are identified by a key
// value (suitable as a key in an unordered collection) so that we only need to persist the
// key values rather than potentially heavy state in a vertex.
template <class Vertex, class Key, class DirectedEdge>
class RootedGraph
{
public:
  RootedGraph(const std::function<Key(const Vertex&)>& getKey,
              const std::function<std::vector<std::pair<DirectedEdge, Vertex>>(const Vertex&)>& getAdjacentVertices)
  : getKey(getKey),
    getAdjacentVertices(getAdjacentVertices)
  {}

  // Creates the graph starting from the given root (or goal) vertex, adding the adjacent vertices
  // until there are no more unique vertices or the maximum path length has been reached.
  void Build(const Vertex& root, size_t maxPathLength);

  // Finds the shortest sequence of edges connecting source to root. If there is no such sequence
  // within the maximum path length specified in Build() then an exception is thrown.
  std::vector<DirectedEdge> FindShortestPathToRoot(const Vertex& source);

  void ReadFromStream(std::istream&);
  void WriteToStream(std::ostream&);

private:
  // Returns the path length to reach the root from the given vertex or -1 if no path is found.
  size_t SafeGetShortestPathLength(const Vertex&);
  // Returns the path length to reach the root from the given vertex. Throws an exception if no path is found.
  size_t GetShortestPathLength(const Vertex&);
  // Returns the edge along the shortest path from the given vertex to the root. Throws an exception if no path is found.
  std::pair<DirectedEdge, Vertex> GetShortestPathEdge(const Vertex&);

  std::function<Key(const Vertex&)> getKey;
  std::function<std::vector<std::pair<DirectedEdge, Vertex>>(const Vertex&)> getAdjacentVertices;
  std::unordered_map<Key, size_t> pathLengthFromVertexKey;
};

#include "RootedGraph.inl"