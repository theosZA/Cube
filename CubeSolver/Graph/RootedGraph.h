#pragma once

#include <cstdint>
#include <istream>
#include <functional>
#include <ostream>
#include <map>
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
  void Build(const Vertex& root, std::uint32_t maxPathLength);

  // Returns true if the graph contains the specified vertex.
  bool ContainsVertex(const Vertex&) const;

  // Finds the shortest sequence of edges connecting source to root. If there is no such sequence
  // within the maximum path length specified in Build() then an exception is thrown.
  std::vector<DirectedEdge> FindShortestPathToRoot(const Vertex& source) const;

  // Returns the path length to reach the root from the given vertex. Throws an exception if no path is found.
  std::uint32_t GetShortestPathLength(const Vertex&) const;

  // Reads the graph path info from a stream as written by WriteToStream().
  // Returns true only if the graph could be populated from the stream.
  bool ReadFromStream(std::istream&);
  // Writes the graph path info to a stream for persistence.
  void WriteToStream(std::ostream&) const;

private:
  // Returns the path length to reach the root from the given vertex or -1 if no path is found.
  std::uint32_t SafeGetShortestPathLength(const Vertex&) const;
  // Returns the edge along the shortest path from the given vertex to the root. Throws an exception if no path is found.
  std::pair<DirectedEdge, Vertex> GetShortestPathEdge(const Vertex&) const;

  std::function<Key(const Vertex&)> getKey;
  std::function<std::vector<std::pair<DirectedEdge, Vertex>>(const Vertex&)> getAdjacentVertices;
  std::map<Key, std::uint32_t> pathLengthFromVertexKey;
};

#include "RootedGraph.inl"