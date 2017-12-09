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
      
    // Find the shortest solution for the given scramble using an A* search.
    Solution Solve(const std::vector<CubeMove>& scramble);

    // Find the shortest solution reaching one of the target states from the given scramble using an A* search.
    PartialSolution SolveToState(const std::vector<CubeMove>& scramble, const std::set<CubeGroup>& targetStates = std::set<CubeGroup>{ CubeGroup::Solved });

  private:
    std::map<CubeGroup, std::unique_ptr<SolverStep>> solverSteps;
};

