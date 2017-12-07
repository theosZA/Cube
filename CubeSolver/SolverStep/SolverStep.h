#pragma once

#include <vector>

#include "Cube\CubeMove.h"

#include "Solution\PartialSolution.h"
#include "Solution\Solution.h"
#include "CubeGroup.h"

// Base class for any step solving a 3x3x3 cube.
class SolverStep
{
public:
  // Returns the cube group which this step can handle, solving it into another cube group.
  virtual CubeGroup GetInputCubeGroup() = 0;
  // Returns a collection of solutions which reduce the cube specified by the scramble and solution so far into another cube group.
  virtual std::vector<PartialSolution> Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar) = 0;
};