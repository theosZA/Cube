#pragma once

#include <memory>

#include "SolverStep.h"

// Step for handling the last 4 corners where those corners are all twisted in place. 3 of the corners are cycled
// to leave us in a state where there are 4 corners with one twisted 3-cycle.
class SolverStep_4CornersTwisted : public SolverStep
{
public:
  CubeGroup GetInputCubeGroup() const { return CubeGroup::AB4C_twisted; }
  bool IsInsertionStep() const { return true; }

  std::vector<PartialSolution> Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar);
};
