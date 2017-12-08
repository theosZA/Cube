#pragma once

#include <memory>

#include "SolverStep.h"

// Step for solving 1 of the last 4 corners where those corners form a 2 distinct 2-cycles, leaving the cube
// with 3 corners in a corner 3-cycle.
class SolverStep_4Corners2Cycles : public SolverStep
{
public:
  CubeGroup GetInputCubeGroup() const { return CubeGroup::AB4C_2cycles; }
  bool IsInsertionStep() const { return true; }

  std::vector<PartialSolution> Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar);
};
