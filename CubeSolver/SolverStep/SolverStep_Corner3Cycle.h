#pragma once

#include <memory>

#include "SolverStep.h"

// Step for solving the last 3 corners where those corners form a cycle.
class SolverStep_Corner3Cycle : public SolverStep
{
public:
  CubeGroup GetInputCubeGroup() const { return CubeGroup::AB3C_3cycle; }
  bool IsInsertionStep() const { return true; }

  std::vector<PartialSolution> Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar);
};
