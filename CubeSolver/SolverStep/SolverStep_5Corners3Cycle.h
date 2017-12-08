#pragma once

#include <memory>

#include "SolverStep.h"

// Step for the last 5 corners where 3 of those corners form a (twisted or non-twisted) 3-cycle and the 
// other 2 are twisted in place. We can either cycle the corner 3-cycle, leaving 2 or 3 corners twisted
// in place; or we can cycle the 2 twisted corners with 1 of the corners in the 3-cycle, leaving a 5-cycle.
class SolverStep_5Corners3Cycle : public SolverStep
{
public:
  CubeGroup GetInputCubeGroup() const { return CubeGroup::AB5C_3cycle; }
  bool IsInsertionStep() const { return true; }

  std::vector<PartialSolution> Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar);
};
