#pragma once

#include <string>

#include "SolverStep.h"
#include "SolverEO.h"

class SolverStep_EO : public SolverStep
{
public:
  SolverStep_EO(const std::string& cacheFileName);

  CubeGroup GetInputCubeGroup() { return CubeGroup::Scrambled; }

  std::vector<PartialSolution> Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar);

private:
  SolverEO solverEO;
};
