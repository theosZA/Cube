#pragma once

#include <string>

#include "Solver2x2x3Block.h"
#include "SolverStep.h"

class SolverStep_2x2x3Block : public SolverStep
{
public:
  SolverStep_2x2x3Block(const std::string& cacheFileName);

  CubeGroup GetInputCubeGroup() { return CubeGroup::Block2x2x2; }

  std::vector<PartialSolution> Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar);

private:
  Solver2x2x3Block solver2x2x3;
};