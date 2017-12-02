#pragma once

#include <string>

#include "SolverStep.h"
#include "SolverEdgesBackSlot.h"

// Step for taking an F2L-1 state (with the back slot remaining) to a skeleton (ABnC).
class SolverStep_SkeletonBackSlot : public SolverStep
{
public:
  SolverStep_SkeletonBackSlot(const std::string& cacheFileName);

  CubeGroup GetInputCubeGroup() { return CubeGroup::F2L_BSlot_EO; }

  std::vector<PartialSolution> Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar);

private:
  SolverEdgesBackSlot solverEdgesBackSlot;  
};
