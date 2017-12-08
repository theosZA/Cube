#pragma once

#include <memory>

#include "SolverStep.h"

// Step for handling the last 2 corners where both those corners are twisted in place. The corners are cycled
// with another corner on the cube to leave us with a corner 3-cycle.
class SolverStep_2CornersTwisted : public SolverStep
{
public:
  CubeGroup GetInputCubeGroup() const { return CubeGroup::AB2C_twisted; }
  bool IsInsertionStep() const { return true; }

  std::vector<PartialSolution> Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar);
};
