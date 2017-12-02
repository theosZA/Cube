#include "SolverStep_SkeletonFrontSlot.h"

SolverStep_SkeletonFrontSlot::SolverStep_SkeletonFrontSlot(const std::string& cacheFileName)
: solverEdgesFrontSlot(20, cacheFileName)
{}

std::vector<PartialSolution> SolverStep_SkeletonFrontSlot::Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar)
{
  // Only one solution for solving remaining edges.
  auto scramblePlusSolution = solutionSoFar.CombineScrambleAndSolution(scramble);
  auto stepMoves = solverEdgesFrontSlot.Solve(scramblePlusSolution);
  
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
