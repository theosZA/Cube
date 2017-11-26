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
  
  // There are 9 possible pre-moves.
  const std::array<CubeMove, 9> preMoves{
    CubeMove{ Face::Back, 1 }, CubeMove{ Face::Back, -1 }, CubeMove{ Face::Back, 2 },
    CubeMove{ Face::Left, 1 }, CubeMove{ Face::Left, -1 }, CubeMove{ Face::Left, 2 },
    CubeMove{ Face::Down, 1 }, CubeMove{ Face::Down, -1 }, CubeMove{ Face::Down, 2 },
  };

  for (const auto& corner : corners)
  {
    auto rotatedScramble = Rotate(scramble, corner, std::make_pair(Face::Back, Face::Left));
    auto rotatedStep = SolutionStep{ "2x2x2", solver2x2x2.Solve(rotatedScramble, Face::Back, Face::Left) };
    auto rotatedPartialSolution = Solution{} + rotatedStep;
    auto partialSolution = rotatedPartialSolution.Rotate(std::make_pair(Face::Back, Face::Left), corner);
    partialSolutions.push_back(PartialSolution{ partialSolution, CubeGroup::Block2x2x2, corner, std::make_pair(Face::Back, Face::Left) });

    // Consider all pre-moves.
    for (const auto& preMove : preMoves)
    {
      auto rotatedScrambleWithPreMove = std::vector<CubeMove>{ preMove } + rotatedScramble;
      auto stepMoves = solver2x2x2.Solve(rotatedScrambleWithPreMove, Face::Back, Face::Left);
      std::vector<bool> isStepMoveOnInverseSolve(stepMoves.size(), false);
      stepMoves.push_back(InvertMove(preMove));
      isStepMoveOnInverseSolve.push_back(true); // only the pre-move is on the inverse solve
      auto rotatedStep = SolutionStep{ "2x2x2", stepMoves, isStepMoveOnInverseSolve };
      auto rotatedPartialSolution = Solution{} + rotatedStep;
      auto partialSolution = rotatedPartialSolution.Rotate(std::make_pair(Face::Back, Face::Left), corner);
      partialSolutions.push_back(PartialSolution{ partialSolution, CubeGroup::Block2x2x2, corner, std::make_pair(Face::Back, Face::Left) });
    }
  }

  return partialSolutions;
}
