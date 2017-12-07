#pragma once

#include <array>
#include <memory>
#include <string>

#include "CubeStateSolver\CubeStateSolver.h"
#include "SolverStep.h"

class SolverStep_F2LMinus1 : public SolverStep
{
public:
  SolverStep_F2LMinus1(const std::string& cacheFileNameFrontSlot, const std::string& cacheFileNameBackSlot);

  CubeGroup GetInputCubeGroup() { return CubeGroup::Block2x2x3_EO; }

  std::vector<PartialSolution> Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar);

private:
  std::array<std::unique_ptr<CubeStateSolver<Cube3x3x3>>, 2> solvers;
};
