#include "SolverStep_SkeletonBackSlot.h"

SolverStep_SkeletonBackSlot::SolverStep_SkeletonBackSlot(const std::string& cacheFileName)
: solverEdgesBackSlot(20, cacheFileName)
{}

std::vector<PartialSolution> SolverStep_SkeletonBackSlot::Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar)
{
  // Only one solution for solving remaining edges.
  auto scramblePlusSolution = solutionSoFar.CombineScrambleAndSolution(scramble);
  auto stepMoves = solverEdgesBackSlot.Solve(scramblePlusSolution);
  
  Cube3x3x3 cube;
  cube += scramblePlusSolution;
  cube += stepMoves;
  auto cubeGroup = GetCubeGroupForSkeleton(cube);
  auto wrongCornerCount = GetNumberOfWrongCorners(cubeGroup);
  auto stepName = "AB" + std::to_string(wrongCornerCount) + "C";

  auto step = SolutionStep{ stepName, stepMoves, true };
  auto partialSolution = PartialSolution{ solutionSoFar + step, cubeGroup };
  return std::vector<PartialSolution>{ partialSolution };
}
