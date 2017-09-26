#pragma once

#include <string>

#include "SolverStep.h"
#include "SolverDominoToDoubleTurns.h"

class SolverStep_DominoToDoubleTurns : public SolverStep
{
public:
  SolverStep_DominoToDoubleTurns(const std::string& cacheFileName);

  CubeGroup GetInputCubeGroup() { return CubeGroup::Domino; }

  std::vector<PartialSolution> Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar);

private:
  SolverDominoToDoubleTurns solverDominoToDoubleTurns;
};
