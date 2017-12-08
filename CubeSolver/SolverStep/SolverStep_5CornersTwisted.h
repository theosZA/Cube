#pragma once

#include <memory>

#include "SolverStep.h"

// Step for handling the last 4 corners where those corners are all twisted in place. 3 of the corners are cycled
// to leave us in a state where there are 5 corners with one (twisted or non-twisted) 3-cycle.
class SolverStep_5CornersTwisted : public SolverStep
{
public:
  CubeGroup GetInputCubeGroup() const { return CubeGroup::AB5C_twisted; }
  bool IsInsertionStep() const { return true; }

  std::vector<PartialSolution> Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar);
};
