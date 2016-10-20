#pragma once

#include <vector>

#include "CubeMove.h"

// Defines an interface for cube-based puzzle solvers.
class Solver
{
public:
  // Returns the move sequence that will solve the given scramble. If the scramble can't be solved within the
  // number of moves specified in the constructor then an exception is thrown.
  virtual std::vector<CubeMove> Solve(const std::vector<CubeMove>& scramble) = 0;
};
