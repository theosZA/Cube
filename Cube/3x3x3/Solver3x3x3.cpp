#include "Solver3x3x3.h"

#include <array>
#include <algorithm>
#include <iterator>

#include "Cube3x3x3.h"
#include "SolverCorners.h"

Solver3x3x3::Solver3x3x3(const std::string& solutionLogFileName,
                         const std::string& cache2x2x2FileName,
                         const std::string& cache2x2x3FileName,
                         const std::string& cache2FaceEOFileName,
                         const std::string& cache2FaceAB5CFileName,
                         const std::string& cache2FaceAB3CFileName)
: solutionLogger(solutionLogFileName),
  solver2x2x2(20, cache2x2x2FileName),
  solver2x2x3(20, cache2x2x3FileName),
  solver2FaceEO(cache2FaceEOFileName, cache2x2x2FileName),
  solver2FaceAB5C(20, cache2FaceAB5CFileName),
  solver2FaceAB3C(20, cache2FaceAB3CFileName)
{}

std::vector<CubeMove> Solver3x3x3::Solve(const std::vector<CubeMove>& scramble) const
{
  solutionLogger.LogScramble(scramble);

  // Consider the 8 possible starting 2x2x2s (by corner) and see which leads to the best solution.
  const std::array<std::pair<Face, Face>, 8> corners {
    std::pair<Face,Face>{ Face::Back, Face::Left },
    std::pair<Face,Face>{ Face::Left, Face::Front },
    std::pair<Face,Face>{ Face::Front, Face::Right },
    std::pair<Face,Face>{ Face::Right, Face::Back },
    std::pair<Face,Face>{ Face::Left, Face::Back },
    std::pair<Face,Face>{ Face::Front, Face::Left },
    std::pair<Face,Face>{ Face::Right, Face::Front },
    std::pair<Face,Face>{ Face::Back, Face::Right }
  };
  bool haveSolution = false;
  Solution bestSolution;
  for (const auto& corner : corners)
  {
    auto newScramble = Rotate(scramble, corner, std::make_pair(Face::Back, Face::Left));
    auto step = solver2x2x2.Solve(newScramble, Face::Back, Face::Left);
    auto solution = Solve3Faces(std::vector<CubeMove>{}, newScramble, step);
    if (!haveSolution || solution.moves.size() < bestSolution.moves.size())
    {
      haveSolution = true;
      solution.InsertStep("2x2x2", step);
      bestSolution = solution.Rotate(std::make_pair(Face::Back, Face::Left), corner);
    }
  }

  // Consider the inverse scramble to see if there's a 2x2x2 block that leads to a better solution.
  auto inverseScramble = InvertMoveSequence(scramble);
  for (const auto& corner : corners)
  {
    auto newInverseScamble = Rotate(inverseScramble, corner, std::make_pair(Face::Back, Face::Left));
    auto inverseStep = solver2x2x2.Solve(newInverseScamble, Face::Back, Face::Left);
    auto stepAsPreMove = InvertMoveSequence(inverseStep);

    auto newScramble = Rotate(scramble, corner, std::make_pair(Face::Back, Face::Left));
    auto solution = Solve3Faces(stepAsPreMove, newScramble, std::vector<CubeMove>{});
    if (!haveSolution || solution.moves.size() < bestSolution.moves.size())
    {
      haveSolution = true;
      solution.InsertStep("2x2x2 on inverted scramble", inverseStep);
      bestSolution = solution.Rotate(std::make_pair(Face::Back, Face::Left), corner);
    }
  }

  solutionLogger.LogSolution(bestSolution);
  return bestSolution.moves;
}

Solution Solver3x3x3::Solve3Faces(const std::vector<CubeMove>& preMoves, const std::vector<CubeMove>& scramble, const std::vector<CubeMove>& solutionSoFar) const
{
  // Consider the 3 possible orientations of the solved 2x2x2 BLD block and see which leads to the best solution.
  std::array<Face, 3> newBackFaces { Face::Back, Face::Left, Face::Down };
  std::array<Face, 3> newLeftFaces { Face::Left, Face::Down, Face::Back };
  bool haveSolution = false;
  Solution bestSolution;
  for (size_t i = 0; i < 3; ++i)
  {
    auto newPreMoves = Rotate(preMoves, std::make_pair(Face::Back, Face::Left), std::make_pair(newBackFaces[i], newLeftFaces[i]));
    auto newScramble = Rotate(scramble, std::make_pair(Face::Back, Face::Left), std::make_pair(newBackFaces[i], newLeftFaces[i]));
    auto newSolutionSoFar = Rotate(solutionSoFar, std::make_pair(Face::Back, Face::Left), std::make_pair(newBackFaces[i], newLeftFaces[i]));
    auto step = solver2x2x3.Solve(newPreMoves + newScramble + newSolutionSoFar);
    auto solution = Solve2Faces(newPreMoves, newScramble, newSolutionSoFar + step);
    if (!haveSolution || solution.moves.size() < bestSolution.moves.size())
    {
      haveSolution = true;
      solution.InsertStep("2x2x3", step);
      bestSolution = solution.Rotate(std::make_pair(newBackFaces[i], newLeftFaces[i]), std::make_pair(Face::Back, Face::Left));
    }
  }

  // Consider the inverse scramble to see if there's a 2x2x3 block that leads to a better solution.
  auto inversePreMoves = InvertMoveSequence(solutionSoFar);
  auto inverseScramble = InvertMoveSequence(scramble);
  auto inverseSolutionSoFar = InvertMoveSequence(preMoves);
  for (size_t i = 0; i < 3; ++i)
  {
    auto newInversePreMoves = Rotate(inversePreMoves, std::make_pair(Face::Back, Face::Left), std::make_pair(newBackFaces[i], newLeftFaces[i]));
    auto newInverseScramble = Rotate(inverseScramble, std::make_pair(Face::Back, Face::Left), std::make_pair(newBackFaces[i], newLeftFaces[i]));
    auto newInverseSolutionSoFar = Rotate(inverseSolutionSoFar, std::make_pair(Face::Back, Face::Left), std::make_pair(newBackFaces[i], newLeftFaces[i]));
    auto inverseStep = solver2x2x3.Solve(newInversePreMoves + newInverseScramble + newInverseSolutionSoFar);
    auto stepAsPreMove = InvertMoveSequence(inverseStep);

    auto newPreMoves = Rotate(preMoves, std::make_pair(Face::Back, Face::Left), std::make_pair(newBackFaces[i], newLeftFaces[i]));
    auto newScramble = Rotate(scramble, std::make_pair(Face::Back, Face::Left), std::make_pair(newBackFaces[i], newLeftFaces[i]));
    auto newSolutionSoFar = Rotate(solutionSoFar, std::make_pair(Face::Back, Face::Left), std::make_pair(newBackFaces[i], newLeftFaces[i]));
    auto solution = Solve2Faces(stepAsPreMove + newPreMoves, newScramble, newSolutionSoFar);
    if (!haveSolution || solution.moves.size() < bestSolution.moves.size())
    {
      haveSolution = true;
      solution.InsertStep("2x2x3 on inverted scramble", inverseStep);
      bestSolution = solution.Rotate(std::make_pair(newBackFaces[i], newLeftFaces[i]), std::make_pair(Face::Back, Face::Left));
    }
  }

  return bestSolution;
}

Solution Solver3x3x3::Solve2Faces(const std::vector<CubeMove>& preMoves, const std::vector<CubeMove>& scramble, const std::vector<CubeMove>& solutionSoFar) const
{
  auto edgeOrientationStep = solver2FaceEO.Solve(preMoves + scramble + solutionSoFar);
  auto solution = Solve2FacesEdgesOriented(preMoves, scramble, solutionSoFar + edgeOrientationStep);
  solution.InsertStep("EO", edgeOrientationStep);
  return solution;
}

Solution Solver3x3x3::Solve2FacesEdgesOriented(const std::vector<CubeMove>& preMoves, const std::vector<CubeMove>& scramble, const std::vector<CubeMove>& solutionSoFar) const
{
  const std::array<std::pair<Face, Face>, 6> corners{
    std::pair<Face, Face>{ Face::Right, Face::Back },
    std::pair<Face, Face>{ Face::Right, Face::Down },
    std::pair<Face, Face>{ Face::Right, Face::Front },
    std::pair<Face, Face>{ Face::Right, Face::Up },
    std::pair<Face, Face>{ Face::Up, Face::Front },
    std::pair<Face, Face>{ Face::Up, Face::Left }
  };
  bool haveSolution = false;
  Solution bestSolution;

  // Try AB3C
  for (size_t i = 0; i < corners.size(); ++i)
    for (size_t j = i + 1; j < corners.size(); ++j)
      for (size_t k = j + 1; k < corners.size(); ++k)
      {
        std::array<TwoFaceAB3C::Corner, 3> currentCorners{
          TwoFaceAB3C::Corner{ corners[i].first, corners[i].second },
          TwoFaceAB3C::Corner{ corners[j].first, corners[j].second },
          TwoFaceAB3C::Corner{ corners[k].first, corners[k].second },
        };
        if (solver2FaceAB3C.CanSolve(preMoves + scramble + solutionSoFar, currentCorners))
        {
          auto ab3cStep = solver2FaceAB3C.Solve(preMoves + scramble + solutionSoFar, currentCorners);
          auto skeleton = solutionSoFar + ab3cStep + preMoves;
          auto solution = SolverCorners::SolveCorners(scramble, skeleton);
          if (!haveSolution || solution.moves.size() < bestSolution.moves.size())
          {
            haveSolution = true;
            solution.InsertStep("AB3C", ab3cStep);
            bestSolution = solution;
          }
        }
      }

  // Try AB5C
  for (const auto& corner : corners)
  {
    auto ab5cStep = solver2FaceAB5C.Solve(preMoves + scramble + solutionSoFar, corner.first, corner.second);
    auto skeleton = solutionSoFar + ab5cStep + preMoves;
    auto solution = SolverCorners::SolveCorners(scramble, skeleton);
    if (!haveSolution || solution.moves.size() < bestSolution.moves.size())
    {
      haveSolution = true;
      solution.InsertStep("AB5C", ab5cStep);
      bestSolution = solution;
    }
  }

  return bestSolution;
}
