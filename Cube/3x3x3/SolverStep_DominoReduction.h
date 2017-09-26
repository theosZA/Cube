#pragma once

#include <string>

#include "SolverStep.h"
#include "SolverDominoReduction.h"

class SolverStep_DominoReduction : public SolverStep
{
public:
  SolverStep_DominoReduction(const std::string& cacheFileName);

  CubeGroup GetInputCubeGroup() { return CubeGroup::EO; }

  std::vector<PartialSolution> Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar);

private:
  SolverDominoReduction solverDominoReduction;
};
