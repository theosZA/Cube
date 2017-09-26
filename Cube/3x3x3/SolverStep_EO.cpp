#include "SolverStep_EO.h"

#include <array>

SolverStep_EO::SolverStep_EO(const std::string& cacheFileName)
: solverEO(20, cacheFileName)
{}

std::vector<PartialSolution> SolverStep_EO::Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar)
{
  std::vector<PartialSolution> partialSolutions;

  // There are 3 possible orientations for doing edge orientation.
  std::array<Face, 3> newFrontFaces{ Face::Front, Face::Right, Face::Down };
  std::array<Face, 3> newUpFaces{ Face::Up, Face::Up, Face::Front };
  for (size_t i = 0; i < 3; ++i)
  {
    auto rotatedScramble = Rotate(scramble, std::make_pair(Face::Front, Face::Up), std::make_pair(newFrontFaces[i], newUpFaces[i]));
    auto rotatedSolutionSoFar = solutionSoFar.Rotate(std::make_pair(Face::Front, Face::Up), std::make_pair(newFrontFaces[i], newUpFaces[i]));
    auto rotatedScramblePlusSolution = rotatedSolutionSoFar.CombineScrambleAndSolution(rotatedScramble);
    auto rotatedStep = SolutionStep{ "EO", solverEO.Solve(rotatedScramblePlusSolution) };
    auto rotatedPartialSolution = rotatedSolutionSoFar + rotatedStep;
    auto partialSolution = rotatedPartialSolution.Rotate(std::make_pair(newFrontFaces[i], newUpFaces[i]), std::make_pair(Face::Front, Face::Up));
    partialSolutions.push_back(PartialSolution{ partialSolution, CubeGroup::EO, std::make_pair(Face::Front, Face::Up), std::make_pair(newFrontFaces[i], newUpFaces[i]) });
  }

  return partialSolutions;
}
