#include "SolverStep_2x2x3EO.h"

SolverStep_2x2x3EO::SolverStep_2x2x3EO(const std::string& cacheFileName)
: solverTwoFaceEO(cacheFileName)
{}

std::vector<PartialSolution> SolverStep_2x2x3EO::Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar)
{
  // Only one solution for edge orientation.
  auto scramblePlusSolution = solutionSoFar.CombineScrambleAndSolution(scramble);
  auto step = SolutionStep{ "EO", solverTwoFaceEO.Solve(scramblePlusSolution) };
  auto partialSolution = PartialSolution{ solutionSoFar + step, CubeGroup::Block2x2x3_EO };
  return std::vector<PartialSolution>{ partialSolution };
}
