#pragma once

#include <map>
#include <memory>
#include <set>
#include <vector>

#include "Cube\CubeMove.h"

#include "Solution\Solution.h"
#include "SolverStep\SolverStep.h"
#include "CubeGroup.h"

// Solves 3x3x3 cubes and provides analysis for individual steps.
class Solver3x3x3
{
  public:
    Solver3x3x3();
      
    // Starts a new analysis by starting with a new scramble.
    void SetScramble(const std::vector<CubeMove>& scramble);

    // Find the best solution we can using an A* search.
    Solution BestSolve(const std::set<CubeGroup>& targetStates = std::set<CubeGroup>{ CubeGroup::Solved });

  private:
    std::vector<CubeMove> scramble;
    std::map<CubeGroup, std::unique_ptr<SolverStep>> solverSteps;
};

