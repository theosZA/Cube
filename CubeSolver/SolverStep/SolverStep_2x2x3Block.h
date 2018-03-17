#pragma once

#include <memory>
#include <string>

#include "CubeStateSolver\CubeStateSolver.h"
#include "SolverStep.h"

class SolverStep_2x2x3Block : public SolverStep
{
public:
  SolverStep_2x2x3Block(const std::string& cacheFileName);

  CubeGroup GetInputCubeGroup() const { return CubeGroup::Block2x2x2; }
  bool IsInsertionStep() const { return false; }

  std::vector<PartialSolution> Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar);

private:
  std::unique_ptr<CubeStateSolver<Cube3x3x3>> solver2x2x3;
};