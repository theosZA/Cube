#include "Solver3x3x3.h"

#include <stdexcept>

#include "Graph\GraphAlgorithms.h"
#include "Solution\PartialSolution.h"
#include "SolverStep\SolverStep_2x2x2Block.h"
#include "SolverStep\SolverStep_2x2x3Block.h"
#include "SolverStep\SolverStep_2x2x3EO.h"
#include "SolverStep\SolverStep_F2LMinus1.h"
#include "SolverStep\SolverStep_InsertCorners.h"
#include "SolverStep\SolverStep_Skeleton.h"

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

Solver3x3x3::Solver3x3x3()
{
  solverSteps[CubeGroup::Scrambled].reset(new SolverStep_2x2x2Block("block2x2x2.3x3"));
  solverSteps[CubeGroup::Block2x2x2].reset(new SolverStep_2x2x3Block("block2x2x3.3x3"));
  solverSteps[CubeGroup::Block2x2x3].reset(new SolverStep_2x2x3EO("2faceEO.3x3"));
  solverSteps[CubeGroup::Block2x2x3_EO].reset(new SolverStep_F2LMinus1("f2l-1_front.3x3", "f2l-1_back.3x3"));
  solverSteps[CubeGroup::F2L_BSlot_EO].reset(new SolverStep_Skeleton<CubeGroup::F2L_BSlot_EO>("edges_back.3x3"));
  solverSteps[CubeGroup::F2L_FSlot_EO].reset(new SolverStep_Skeleton<CubeGroup::F2L_FSlot_EO>("edges_front.3x3"));
  solverSteps[CubeGroup::AB2C_twisted].reset(new SolverStep_InsertCorners<CubeGroup::AB2C_twisted>());
  solverSteps[CubeGroup::AB3C_3cycle].reset(new SolverStep_InsertCorners<CubeGroup::AB3C_3cycle>());
  solverSteps[CubeGroup::AB3C_twisted].reset(new SolverStep_InsertCorners<CubeGroup::AB3C_twisted>());
  solverSteps[CubeGroup::AB4C_4cycle].reset(new SolverStep_InsertCorners<CubeGroup::AB4C_4cycle>());
  solverSteps[CubeGroup::AB4C_2cycles].reset(new SolverStep_InsertCorners<CubeGroup::AB4C_2cycles>());
  solverSteps[CubeGroup::AB4C_3cycle].reset(new SolverStep_InsertCorners<CubeGroup::AB4C_3cycle>());
  solverSteps[CubeGroup::AB4C_twisted].reset(new SolverStep_InsertCorners<CubeGroup::AB4C_twisted>());
  solverSteps[CubeGroup::AB5C_5cycle].reset(new SolverStep_InsertCorners<CubeGroup::AB5C_5cycle>());
  solverSteps[CubeGroup::AB5C_4cycle].reset(new SolverStep_InsertCorners<CubeGroup::AB5C_4cycle>());
  solverSteps[CubeGroup::AB5C_3cycle].reset(new SolverStep_InsertCorners<CubeGroup::AB5C_3cycle>());
  solverSteps[CubeGroup::AB5C_2cycles].reset(new SolverStep_InsertCorners<CubeGroup::AB5C_2cycles>());
  solverSteps[CubeGroup::AB5C_twisted].reset(new SolverStep_InsertCorners<CubeGroup::AB5C_twisted>());
}

void Solver3x3x3::SetScramble(const std::vector<CubeMove>& scramble)
{
  this->scramble = scramble;
}

Solution Solver3x3x3::BestSolve(const std::set<CubeGroup>& targetStates)
{
  auto isSolved = [=](const PartialSolution& partialSolution)
  {
    return targetStates.find(partialSolution.cubeGroup) != targetStates.end();
  };
  auto movesSoFar = [](const PartialSolution& partialSolution)
  {
    return partialSolution.solutionSoFar.Length();
  };
  auto estimatedMovesToSolve = [](const PartialSolution& partialSolution)
  {
    return EstimateMovesRequired(partialSolution.cubeGroup);
  };
  auto generateSuccessorStates = [this](const PartialSolution& partialSolution)
  {
    // Use the solver step for this state. If we don't have a solver for it, then no successor states.
    auto findIter = solverSteps.find(partialSolution.cubeGroup);
    if (findIter == solverSteps.end())
    {
      return std::vector<PartialSolution>{};
    }
    auto& solverStep = *findIter->second;
    return GetAllPartialSolutions(solverStep, scramble, partialSolution);
  };
  return GraphAlgorithms::AStarSearchForClosestTargetNode<PartialSolution, int>(PartialSolution{ Solution{}, CubeGroup::Scrambled }, isSolved, movesSoFar, estimatedMovesToSolve, generateSuccessorStates).solutionSoFar;
}
