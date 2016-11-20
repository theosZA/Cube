#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include "..\CubeMove.h"
#include "RootedGraph.h"

// An optimal solver for cubes based on an exhaustive graph of states connected to the goal state.
// To avoid having all possible cube states in the graph, we have an equivalence relation defined
// by a key value function. For a given move, when applied to all states with the same key value,
// should yield a new set of states all with the same (new) key value.
template<class CubeType>
class CubeStateSolver
{
public:
  // Initializes the solver with all possible positions that can be solved in the given number of moves.
  // Will first try reading from the specified cache file, and only if that isn't found will the
  // solver generate the moves from scratch and then save to the cache file.
  CubeStateSolver(std::uint32_t maxMoves, const std::function<std::uint32_t(const CubeType&)> getKey,
                  const std::vector<CubeMove>& possibleMoves, const std::string& cacheFileName = "");

  // Returns the move sequence that will solve the given cube. If the cube can't be solved within the number
  // of moves specified in the constructor then an exception is thrown.
  std::vector<CubeMove> Solve(const CubeType&) const;
  
  // Returns the number of moves required to solve the given cube. If the cube can't be solved within the
  // number of moves specified in the constructor then an exception is thrown.
  std::uint32_t GetRequiredMoveCount(const CubeType&) const;

  // Returns true if the given cube can be solved within the number of moves specified in the constructor.
  bool CanSolve(const CubeType&) const;

private:
  // Writes the set of positions to a cache file.
  void WriteCacheFile(const std::string& cacheFileName) const;
  // Reads the set of positions from a cache file written by WriteCacheFile().
  // Returns true only if the file was found and the set of positions was populated.
  bool ReadCacheFile(const std::string& cacheFileName);

  RootedGraph<CubeType, std::uint32_t, CubeMove> graph;
};

#include "CubeStateSolver.inl"