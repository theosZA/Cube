#include "SolverStep_DominoToDoubleTurns.h"

SolverStep_DominoToDoubleTurns::SolverStep_DominoToDoubleTurns(const std::string& cacheFileName)
: solverDominoToDoubleTurns(20, cacheFileName)
{}

std::vector<PartialSolution> SolverStep_DominoToDoubleTurns::Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar)
{
  // Only one orientation so only one solution.
  auto scramblePlusSolution = solutionSoFar.CombineScrambleAndSolution(scramble);
  auto step = SolutionStep{ "Reduce to double turns", solverDominoToDoubleTurns.Solve(scramblePlusSolution) };
  auto partialSolution = PartialSolution{ solutionSoFar + step, CubeGroup::DoubleTurns };
  return std::vector<PartialSolution>{ partialSolution };
}
