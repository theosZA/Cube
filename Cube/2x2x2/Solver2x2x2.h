#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "..\CubeMove.h"
#include "..\Solver.h"
#include "..\Graph\CubeStateSolver.h"
#include "Cube2x2x2.h"

// An optimal solver for 2x2x2 cubes.
class Solver2x2x2 : public Solver
{
public:
  // Initializes the solver with all possible positions that can be solved in the given number of moves.
  // Will first try reading from the specified cache file, and only if that isn't found will the
  // solver generate the moves from scratch and then save to the cache file.
  Solver2x2x2(std::uint32_t maxMoves, const std::string& cacheFileName = "");

  // Returns the move sequence that will solve the given cube. If the cube can't be solved within the number
  // of moves specified in the constructor then an exception is thrown.
  std::vector<CubeMove> Solve(Cube2x2x2) const;
  // Returns the move sequence that will solve the given scramble. If the scramble can't be solved within the
  // number of moves specified in the constructor then an exception is thrown.
  std::vector<CubeMove> Solve(const std::vector<CubeMove>& scramble) const override;

private:
  CubeStateSolver<Cube2x2x2> solver;
};