#pragma once

#include <string>

#include "TreeSearch\TwoFaceEO.h"
#include "SolverStep.h"

class SolverStep_2x2x3EO : public SolverStep
{
public:
  SolverStep_2x2x3EO(const std::string& cacheFileName);

  CubeGroup GetInputCubeGroup() const { return CubeGroup::Block2x2x3; }
  bool IsInsertionStep() const { return false; }

  std::vector<PartialSolution> Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar);

private:
  TwoFaceEO solverTwoFaceEO;
};
