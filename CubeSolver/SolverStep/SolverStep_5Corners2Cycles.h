#pragma once

#include <memory>

#include "SolverStep.h"

// Step for the last 5 corners where 4 of those corners form 2 distinct 2-cycles and the last corner
// is twisted in place. There a number of approaches to try which will leave us with either 4 or 5 corners
// that can always be solved in 2 corner 3-cycles.
class SolverStep_5Corners2Cycles : public SolverStep
{
public:
  CubeGroup GetInputCubeGroup() const { return CubeGroup::AB5C_2cycles; }
  bool IsInsertionStep() const { return true; }

  std::vector<PartialSolution> Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar);
};
