#include "SolverStep_Corner3Cycle.h"

#include "Insertions\Corner3Cycle.h"

std::vector<PartialSolution> SolverStep_Corner3Cycle::Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar)
{
  Cube3x3x3 cube;
  cube += scramble;
  auto partialSolution = PartialSolution{ Corner3Cycle::SolveCorner3Cycle(cube, solutionSoFar), CubeGroup::Solved };
  return std::vector<PartialSolution>{ partialSolution };
}
