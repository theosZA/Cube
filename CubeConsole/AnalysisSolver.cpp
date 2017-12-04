#include "AnalysisSolver.h"

#include <queue>
#include <stdexcept>

#include "..\Cube\3x3x3\PartialSolution.h"
#include "..\Cube\3x3x3\SolverStep_2x2x2Block.h"
#include "..\Cube\3x3x3\SolverStep_2x2x3Block.h"
#include "..\Cube\3x3x3\SolverStep_2x2x3EO.h"
#include "..\Cube\3x3x3\SolverStep_F2LMinus1.h"
#include "..\Cube\3x3x3\SolverStep_Skeleton.h"

std::vector<PartialSolution> GetAllPartialSolutions(SolverStep& solverStep, const std::vector<CubeMove>& scramble, const PartialSolution& partialSolution)
{
  // Rotate to canonical orientation.
  auto rotatedScramble = Rotate(scramble, partialSolution.rotationOldFaces, partialSolution.rotationNewFaces);
  auto rotatedSolutionSoFar = partialSolution.solutionSoFar.Rotate(partialSolution.rotationOldFaces, partialSolution.rotationNewFaces);

  // Get solutions.
  auto rotatedSolutions = solverStep.Solve(rotatedScramble, rotatedSolutionSoFar);
  auto inverseRotatedSolutions = solverStep.Solve(InvertMoveSequence(rotatedScramble), rotatedSolutionSoFar.Invert());
  for (auto& inverseRotatedSolution : inverseRotatedSolutions)
  {
    rotatedSolutions.push_back(inverseRotatedSolution.Invert());
  }

  // Rotate back to original orientation.
  std::vector<PartialSolution> solutions;
  solutions.reserve(rotatedSolutions.size());
  std::transform(rotatedSolutions.begin(), rotatedSolutions.end(), std::back_inserter(solutions),
    [&](const PartialSolution& rotatedSolution)
    {
      return rotatedSolution.Rotate(partialSolution.rotationNewFaces, partialSolution.rotationOldFaces);
    });
  return solutions;
}

template <class Random>
PartialSolution SolveRandom(SolverStep& solverStep, const std::vector<CubeMove>& scramble, const PartialSolution& partialSolution, Random& generator)
{
  auto solutions = GetAllPartialSolutions(solverStep, scramble, partialSolution);
  auto randomIndex = std::uniform_int_distribution<size_t>(0, solutions.size() - 1)(generator);
  return solutions[randomIndex];
}

PartialSolution SolveBest(SolverStep& solverStep, const std::vector<CubeMove>& scramble, const PartialSolution& partialSolution)
{
  auto solutions = GetAllPartialSolutions(solverStep, scramble, partialSolution);
  return *std::min_element(solutions.begin(), solutions.end(),
    [](const PartialSolution& a, const PartialSolution& b)
  {
    return a.solutionSoFar.Length() < b.solutionSoFar.Length();
  });
}

AnalysisSolver::AnalysisSolver()
{
  solverSteps[CubeGroup::Scrambled].reset(new SolverStep_2x2x2Block("block2x2x2.3x3"));
  solverSteps[CubeGroup::Block2x2x2].reset(new SolverStep_2x2x3Block("block2x2x3.3x3"));
  solverSteps[CubeGroup::Block2x2x3].reset(new SolverStep_2x2x3EO("2faceEO.3x3"));
  solverSteps[CubeGroup::Block2x2x3_EO].reset(new SolverStep_F2LMinus1("f2l-1_front.3x3", "f2l-1_back.3x3"));
  solverSteps[CubeGroup::F2L_BSlot_EO].reset(new SolverStep_Skeleton<CubeGroup::F2L_BSlot_EO>("edges_back.3x3"));
  solverSteps[CubeGroup::F2L_FSlot_EO].reset(new SolverStep_Skeleton<CubeGroup::F2L_FSlot_EO>("edges_front.3x3"));
}

void AnalysisSolver::SetScramble(const std::vector<CubeMove>& scramble)
{
  this->scramble = scramble;
}

Solution AnalysisSolver::BestSolve(const std::set<CubeGroup>& targetStates)
{
  auto priorityFunction = [](const PartialSolution& a, const PartialSolution& b)
  {
    return a.solutionSoFar.Length() + EstimateMovesRequired(a.cubeGroup) > b.solutionSoFar.Length() + EstimateMovesRequired(b.cubeGroup);
  };
  std::priority_queue<PartialSolution, std::vector<PartialSolution>, decltype(priorityFunction)> partialSolutions(priorityFunction);
  partialSolutions.push(PartialSolution{ Solution{}, CubeGroup::Scrambled });
  
  while (!partialSolutions.empty() && targetStates.find(partialSolutions.top().cubeGroup) == targetStates.end())
  {
    auto currentPartialSolution = partialSolutions.top();
    partialSolutions.pop();
    
    // Get the solver step for this state. If we don't have a solver for it, then skip it.
    auto findIter = solverSteps.find(currentPartialSolution.cubeGroup);
    if (findIter == solverSteps.end())
    {
      continue;
    }
    auto& solverStep = *findIter->second;

    // Now solve to get all successor states and add them to our priority queue.
    auto successorPartialSolutions = GetAllPartialSolutions(solverStep, scramble, currentPartialSolution);
    for (const auto& partialSolution : successorPartialSolutions)
    {
      partialSolutions.push(partialSolution);
    }
  }

  if (partialSolutions.empty())
  {
    throw std::runtime_error("Failed to find solution");
  }

  return partialSolutions.top().solutionSoFar;
}
