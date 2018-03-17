#pragma once

#include <memory>

#include "SolverStep.h"

// Step for solving 1 of the last 4 corners where 3 of those corners form a twisted 3-cycle, leaving the cube
// with 3 corners in a corner 3-cycle.
class SolverStep_4Corners3Cycle : public SolverStep
{
public:
  CubeGroup GetInputCubeGroup() const { return CubeGroup::AB4C_3cycle; }
  bool IsInsertionStep() const { return true; }

  std::vector<PartialSolution> Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar);
};
