#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "..\CubeMove.h"
#include "..\Graph\CubeStateSolver.h"
#include "Cube3x3x3.h"

// A solver that finishes the first 2 layers except for the front-right slot from a 2x2x3 block where edges are oriented.
class SolverF2LMinusFrontSlot
{
public:
  // Initializes the solver with all possible positions that can be solved in the given number of moves.
  // Will first try reading from the specified cache file, and only if that isn't found will the
  // solver generate the moves from scratch and then save to the cache file.
  SolverF2LMinusFrontSlot(std::uint32_t maxMoves, const std::string& cacheFileName = "");

  // Returns the move sequence that will take a 2x2x3 block (with EO) to F2L solved except FDR and FR.
  // If this can't be achieved within the number of moves specified in the constructor then an exception is thrown.
  std::vector<CubeMove> Solve(const Cube3x3x3&) const;
  // Returns the move sequence that will take a 2x2x3 block (with EO) to F2L solved except FDR and FR from the given scramble.
  // If this can't be achieved within the number of moves specified in the constructor then an exception is thrown.
  std::vector<CubeMove> Solve(const std::vector<CubeMove>& scramble) const;

private:
  // Returns a key value that distinguishes based on the position of cubies in the BRD 2x2x1 block.
  static std::uint32_t GetKeyValue(const Cube3x3x3&);

  CubeStateSolver<Cube3x3x3> solver;
};

