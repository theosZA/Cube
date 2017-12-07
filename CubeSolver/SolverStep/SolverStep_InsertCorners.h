#pragma once

#include <memory>
#include <string>

#include "Insertions\SolverCorners.h"
#include "SolverStep.h"

// Step for inserting corners into a (ABnC) skeleton.
template <CubeGroup InputCubeGroup>
class SolverStep_InsertCorners : public SolverStep
{
public:
  CubeGroup GetInputCubeGroup() const { return InputCubeGroup; }
  bool IsInsertionStep() const { return true; }

  std::vector<PartialSolution> Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar)
  {
    auto partialSolution = PartialSolution{ SolverCorners::SolveCorners(scramble, solutionSoFar), CubeGroup::Solved };
    return std::vector<PartialSolution>{ partialSolution };
  }
};
