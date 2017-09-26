#include "SolverStep_DominoReduction.h"

#include <array>

SolverStep_DominoReduction::SolverStep_DominoReduction(const std::string& cacheFileName)
: solverDominoReduction(20, cacheFileName)
{}

std::vector<PartialSolution> SolverStep_DominoReduction::Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar)
{
  std::vector<PartialSolution> partialSolutions;

  // There are 2 possible orientations (keeping F/B constant) for reducing to a domino.
  std::array<Face, 2> newLeftFaces{ Face::Left, Face::Down };
  std::array<Face, 2> newUpFaces{ Face::Up, Face::Left };
  for (size_t i = 0; i < 2; ++i)
  {
    auto rotatedScramble = Rotate(scramble, std::make_pair(Face::Left, Face::Up), std::make_pair(newLeftFaces[i], newUpFaces[i]));
    auto rotatedSolutionSoFar = solutionSoFar.Rotate(std::make_pair(Face::Left, Face::Up), std::make_pair(newLeftFaces[i], newUpFaces[i]));
    auto rotatedScramblePlusSolution = rotatedSolutionSoFar.CombineScrambleAndSolution(rotatedScramble);
    auto rotatedStep = SolutionStep{ "Reduce to domino", solverDominoReduction.Solve(rotatedScramblePlusSolution) };
    auto rotatedPartialSolution = rotatedSolutionSoFar + rotatedStep;
    auto partialSolution = rotatedPartialSolution.Rotate(std::make_pair(newLeftFaces[i], newUpFaces[i]), std::make_pair(Face::Left, Face::Up));
    partialSolutions.push_back(PartialSolution{ partialSolution, CubeGroup::Domino, std::make_pair(Face::Left, Face::Up), std::make_pair(newLeftFaces[i], newUpFaces[i]) });
  }

  return partialSolutions;
}
