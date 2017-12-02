#pragma once

#include <map>
#include <memory>
#include <vector>

#include "..\Cube\CubeMove.h"
#include "..\Cube\Solution.h"
#include "..\Cube\3x3x3\CubeGroup.h"
#include "..\Cube\3x3x3\SolverStep.h"

// Solves 3x3 cubes and provides analysis for individual steps.
class AnalysisSolver
{
  public:
    AnalysisSolver();
      
    // Starts a new analysis by starting with a new scramble.
    void SetScramble(const std::vector<CubeMove>& scramble);

    // Find the best solution we can using an A* search.
    Solution BestSolve(CubeGroup targetState = CubeGroup::Solved);

  private:
    std::vector<CubeMove> scramble;
    std::map<CubeGroup, std::unique_ptr<SolverStep>> solverSteps;
};

