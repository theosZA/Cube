#pragma once

#include <string>

#include "SolverStep.h"
#include "SolverF2LMinusBackSlot.h"
#include "SolverF2LMinusFrontSlot.h"

class SolverStep_F2LMinus1 : public SolverStep
{
public:
  SolverStep_F2LMinus1(const std::string& cacheFileNameFrontSlot, const std::string& cacheFileNameBackSlot);

  CubeGroup GetInputCubeGroup() { return CubeGroup::Block2x2x3_EO; }

  std::vector<PartialSolution> Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar);

private:
  SolverF2LMinusBackSlot solverF2LMinusBackSlot;
  SolverF2LMinusFrontSlot solverF2LMinusFrontSlot;
};
