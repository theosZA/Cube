#include "SolverStep_2x2x2Block.h"

#include <array>

SolverStep_2x2x2Block::SolverStep_2x2x2Block(const std::string& cacheFileName)
: solver2x2x2(20, cacheFileName)
{}

std::vector<PartialSolution> SolverStep_2x2x2Block::Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar)
{
  std::vector<PartialSolution> partialSolutions;

  // There are 8 possible starting 2x2x2s (by corner).
  const std::array<std::pair<Face, Face>, 8> corners{
    std::pair<Face,Face>{ Face::Back, Face::Left },
    std::pair<Face,Face>{ Face::Left, Face::Front },
    std::pair<Face,Face>{ Face::Front, Face::Right },
    std::pair<Face,Face>{ Face::Right, Face::Back },
    std::pair<Face,Face>{ Face::Left, Face::Back },
    std::pair<Face,Face>{ Face::Front, Face::Left },
    std::pair<Face,Face>{ Face::Right, Face::Front },
    std::pair<Face,Face>{ Face::Back, Face::Right }
  };

  for (const auto& corner : corners)
  {
    auto rotatedScramble = Rotate(scramble, corner, std::make_pair(Face::Back, Face::Left));
    auto rotatedStep = SolutionStep{ "2x2x2", solver2x2x2.Solve(rotatedScramble, Face::Back, Face::Left) };
    partialSolutions.push_back(PartialSolution{ Solution{} + rotatedStep, CubeGroup::Block2x2x3, std::make_pair(Face::Back, Face::Left), corner });
  }

  return partialSolutions;
}
