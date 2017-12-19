#pragma once

#include <map>
#include <memory>
#include <optional>
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
    PartialSolution SolveToState(const std::vector<CubeMove>& scramble, const PartialSolution& solutionSoFar = PartialSolution{ Solution{}, CubeGroup::Scrambled }, const std::set<CubeGroup>& targetStates = std::set<CubeGroup>{ CubeGroup::Solved });

    // Find the absolute shortest solution reaching one of the target states (including the estimated number of moves to solve from the target state in a linear solve) from the given scramble.
    // Will take a very long time for a full solve.
    PartialSolution ExhaustiveSolveToState(const std::vector<CubeMove>& scramble, const PartialSolution& solutionSoFar = PartialSolution{ Solution{}, CubeGroup::Scrambled }, const std::set<CubeGroup>& targetStates = std::set<CubeGroup>{ CubeGroup::Solved });

    // Find a solution reaching one of the target states by taking only the move sequence each step with the best total move count estimate.
    // If the target state can't be reached using this method then no solution is returned.
    std::optional<PartialSolution> LinearBestSolveToState(const std::vector<CubeMove>& scramble, const PartialSolution& solutionSoFar = PartialSolution{ Solution{}, CubeGroup::Scrambled }, const std::set<CubeGroup>& targetStates = std::set<CubeGroup>{ CubeGroup::Solved });

  private:
    std::vector<PartialSolution> GenerateAllSuccessorStates(const std::vector<CubeMove>& scramble, const PartialSolution& partialSolution);

    std::map<CubeGroup, std::unique_ptr<SolverStep>> solverSteps;
};

