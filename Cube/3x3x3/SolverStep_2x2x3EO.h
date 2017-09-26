#pragma once

#include <string>

#include "SolverStep.h"
#include "TwoFaceEO.h"

class SolverStep_2x2x3EO : public SolverStep
{
public:
  SolverStep_2x2x3EO(const std::string& cacheFileName);

  CubeGroup GetInputCubeGroup() { return CubeGroup::Block2x2x3; }

  std::vector<PartialSolution> Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar);

private:
  TwoFaceEO solverTwoFaceEO;
};
