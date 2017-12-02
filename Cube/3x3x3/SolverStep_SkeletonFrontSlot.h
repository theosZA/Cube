#pragma once

#include <string>

#include "SolverStep.h"
#include "SolverEdgesFrontSlot.h"

// Step for taking an F2L-1 state (with the back slot remaining) to a skeleton (ABnC).
class SolverStep_SkeletonFrontSlot : public SolverStep
{
public:
  SolverStep_SkeletonFrontSlot(const std::string& cacheFileName);

  CubeGroup GetInputCubeGroup() { return CubeGroup::F2L_FSlot_EO; }

  std::vector<PartialSolution> Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar);

private:
  SolverEdgesFrontSlot solverEdgesFrontSlot;
};
