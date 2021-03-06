#pragma once

#include <memory>
#include <string>

#include "CubeStateSolver\CubeStateSolver3x3x3Factory.h"
#include "Insertions\Corners.h"
#include "SolverStep.h"

// Step for taking an F2L-1 state to a skeleton (ABnC).
template <CubeGroup InputCubeGroup>
class SolverStep_Skeleton : public SolverStep
{
public:
  SolverStep_Skeleton(const std::string& cacheFileName)
  : solver(CubeStateSolver3x3x3Factory::CreateSolver(cacheFileName, InputCubeGroup))
  {}

  CubeGroup GetInputCubeGroup() const { return InputCubeGroup; }
  bool IsInsertionStep() const { return false; }

  std::vector<PartialSolution> Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar)
  {
    // Only one solution for solving remaining edges.
    auto scramblePlusSolution = solutionSoFar.CombineScrambleAndSolution(scramble);
    auto stepMoves = solver->Solve(scramblePlusSolution);

    Cube3x3x3 cube;
    cube += scramblePlusSolution;
    cube += stepMoves;
    auto cubeGroup = Corners::GetCubeGroupForWrongCorners(cube);
    auto wrongCornerCount = Corners::GetNumberOfWrongCorners(cubeGroup);
    std::string stepName;
    if (wrongCornerCount == 0)
    {
      stepName = "Finish solve";
    }
    else
    {
      stepName = "AB" + std::to_string(wrongCornerCount) + "C";
    }

    auto step = SolutionStep{ stepName, stepMoves, true };
    auto partialSolution = PartialSolution{ solutionSoFar + step, cubeGroup };
    return std::vector<PartialSolution>{ partialSolution };
  }

private:
  std::unique_ptr<CubeStateSolver<Cube3x3x3>> solver;
};
