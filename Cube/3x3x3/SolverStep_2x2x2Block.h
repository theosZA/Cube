#pragma once

#include <string>

#include "Solver2x2x2Block.h"
#include "SolverStep.h"

class SolverStep_2x2x2Block : public SolverStep
{
public:
  SolverStep_2x2x2Block(const std::string& cacheFileName);

  CubeGroup GetInputCubeGroup() { return CubeGroup::Scrambled; }

  std::vector<PartialSolution> Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar);

private:
  Solver2x2x2Block solver2x2x2;
};