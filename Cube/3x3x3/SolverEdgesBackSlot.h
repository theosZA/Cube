#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "..\CubeMove.h"
#include "..\Graph\CubeStateSolver.h"
#include "Cube3x3x3.h"

// A solver that finishes all remaining edges where the first 2 layers are done except for the back-right slot and
// edges are already oriented.
class SolverEdgesBackSlot
{
public:
  // Initializes the solver with all possible positions that can be solved in the given number of moves.
  // Will first try reading from the specified cache file, and only if that isn't found will the
  // solver generate the moves from scratch and then save to the cache file.
  SolverEdgesBackSlot(std::uint32_t maxMoves, const std::string& cacheFileName = "");

  // Returns the move sequence that will solve the remaining edges where F2L is done except BDR and BR.
  // If this can't be achieved within the number of moves specified in the constructor then an exception is thrown.
  std::vector<CubeMove> Solve(const Cube3x3x3&) const;
  // Returns the move sequence that will solve the remaining edges where F2L is done except BDR and BR from the given scramble.
  // If this can't be achieved within the number of moves specified in the constructor then an exception is thrown.
  std::vector<CubeMove> Solve(const std::vector<CubeMove>& scramble) const;

private:
  // Returns a key value that distinguishes based on the position of the edges that need to be solved.
  static std::uint32_t GetKeyValue(const Cube3x3x3&);

  CubeStateSolver<Cube3x3x3> solver;
};

