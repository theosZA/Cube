#pragma once

#include <memory>

#include "SolverStep.h"

// Step for handling the last 3 corners where those corners are all twisted in place. The corners are cycled
// to leave us with a corner 3-cycle.
class SolverStep_3CornersTwisted : public SolverStep
{
public:
  CubeGroup GetInputCubeGroup() const { return CubeGroup::AB3C_twisted; }
  bool IsInsertionStep() const { return true; }

  std::vector<PartialSolution> Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar);
};
