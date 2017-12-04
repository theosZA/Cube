#include "SolverStep_2x2x3Block.h"

#include <array>

#include "CubeStateSolver3x3x3Factory.h"

SolverStep_2x2x3Block::SolverStep_2x2x3Block(const std::string& cacheFileName)
: solver2x2x3(CubeStateSolver3x3x3Factory::CreateSolver(cacheFileName, CubeGroup::Block2x2x2, CubeGroup::Block2x2x3))
{}

std::vector<PartialSolution> SolverStep_2x2x3Block::Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar)
{
  std::vector<PartialSolution> partialSolutions;

  // There are 3 possible orientations of the solved 2x2x2 BLD block.
  std::array<Face, 3> newBackFaces{ Face::Back, Face::Left, Face::Down };
  std::array<Face, 3> newLeftFaces{ Face::Left, Face::Down, Face::Back };

  // There are 3 possible pre-moves.
  const std::array<CubeMove, 3> preMoves{
    CubeMove{ Face::Front, 1 }, CubeMove{ Face::Front, -1 }, CubeMove{ Face::Front, 2 }
  };

  for (size_t i = 0; i < 3; ++i)
  {
    auto rotatedScramble = Rotate(scramble, std::make_pair(Face::Back, Face::Left), std::make_pair(newBackFaces[i], newLeftFaces[i]));
    auto rotatedSolutionSoFar = solutionSoFar.Rotate(std::make_pair(Face::Back, Face::Left), std::make_pair(newBackFaces[i], newLeftFaces[i]));
    auto rotatedScramblePlusSolution = rotatedSolutionSoFar.CombineScrambleAndSolution(rotatedScramble);
    auto rotatedStep = SolutionStep{ "2x2x3", solver2x2x3->Solve(rotatedScramblePlusSolution) };
    auto rotatedPartialSolution = rotatedSolutionSoFar + rotatedStep;
    auto partialSolution = rotatedPartialSolution.Rotate(std::make_pair(newBackFaces[i], newLeftFaces[i]), std::make_pair(Face::Back, Face::Left));
    partialSolutions.push_back(PartialSolution{ partialSolution, CubeGroup::Block2x2x3, std::make_pair(Face::Back, Face::Left), std::make_pair(newBackFaces[i], newLeftFaces[i]) });

    // Consider all pre-moves.
    for (const auto& preMove : preMoves)
    {
      auto rotatedScramblePlusSolutionWithPreMove = std::vector<CubeMove>{ preMove } +rotatedScramblePlusSolution;
      auto stepMoves = solver2x2x3->Solve(rotatedScramblePlusSolutionWithPreMove);
      std::vector<bool> isStepMoveOnInverseSolve(stepMoves.size(), false);
      stepMoves.push_back(InvertMove(preMove));
      isStepMoveOnInverseSolve.push_back(true); // only the pre-move is on the inverse solve
      auto rotatedStep = SolutionStep{ "2x2x3", stepMoves, isStepMoveOnInverseSolve };
      auto rotatedPartialSolution = rotatedSolutionSoFar + rotatedStep;
      auto partialSolution = rotatedPartialSolution.Rotate(std::make_pair(newBackFaces[i], newLeftFaces[i]), std::make_pair(Face::Back, Face::Left));
      partialSolutions.push_back(PartialSolution{ partialSolution, CubeGroup::Block2x2x3, std::make_pair(Face::Back, Face::Left), std::make_pair(newBackFaces[i], newLeftFaces[i]) });
    }
  }

  return partialSolutions;
}
