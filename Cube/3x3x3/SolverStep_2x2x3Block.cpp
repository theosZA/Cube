#include "SolverStep_2x2x3Block.h"

#include <array>

SolverStep_2x2x3Block::SolverStep_2x2x3Block(const std::string& cacheFileName)
  : solver2x2x3(20, cacheFileName)
{}

std::vector<PartialSolution> SolverStep_2x2x3Block::Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar)
{
  std::vector<PartialSolution> partialSolutions;

  // There are 3 possible orientations of the solved 2x2x2 BLD block.
  std::array<Face, 3> newBackFaces{ Face::Back, Face::Left, Face::Down };
  std::array<Face, 3> newLeftFaces{ Face::Left, Face::Down, Face::Back };
  for (size_t i = 0; i < 3; ++i)
  {
    auto rotatedScramble = Rotate(scramble, std::make_pair(Face::Back, Face::Left), std::make_pair(newBackFaces[i], newLeftFaces[i]));
    auto rotatedSolutionSoFar = solutionSoFar.Rotate(std::make_pair(Face::Back, Face::Left), std::make_pair(newBackFaces[i], newLeftFaces[i]));
    auto rotatedScramblePlusSolution = rotatedSolutionSoFar.CombineScrambleAndSolution(rotatedScramble);
    auto rotatedStep = SolutionStep{ "2x2x3", solver2x2x3.Solve(rotatedScramblePlusSolution) };
    auto rotatedPartialSolution = rotatedSolutionSoFar + rotatedStep;
    auto partialSolution = rotatedPartialSolution.Rotate(std::make_pair(newBackFaces[i], newLeftFaces[i]), std::make_pair(Face::Back, Face::Left));
    partialSolutions.push_back(PartialSolution{ partialSolution, CubeGroup::Block2x2x3, std::make_pair(Face::Back, Face::Left), std::make_pair(newBackFaces[i], newLeftFaces[i]) });
  }

  return partialSolutions;
}
