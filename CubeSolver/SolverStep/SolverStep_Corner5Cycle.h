#pragma once

#include <memory>

#include "SolverStep.h"

// Step for solving 2 of the last 5 corners where those corners form a cycle.
class SolverStep_Corner5Cycle : public SolverStep
{
public:
  CubeGroup GetInputCubeGroup() const { return CubeGroup::AB5C_5cycle; }
  bool IsInsertionStep() const { return true; }

  std::vector<PartialSolution> Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar);
};
