#include <algorithm>
#include <iostream>
#include <map>
#include <random>

#include "..\Cube\3x3x3\SolverStep_2x2x2Block.h"
#include "..\Cube\3x3x3\SolverStep_2x2x3Block.h"
#include "..\Cube\3x3x3\SolverStep_2x2x3EO.h"
#include "..\Cube\3x3x3\SolverStep_EO.h"
#include "..\Cube\3x3x3\SolverStep_DominoReduction.h"
#include "..\Cube\3x3x3\SolverStep_DominoToDoubleTurns.h"
#include "..\Cube\Scrambler\Scrambler.h"

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

int main()
{
  std::random_device randomSource;
  Scrambler scrambler(randomSource());

  std::mt19937 generator;
  
  std::map<int, int> solveLengthCounts;
/*
  SolverStep_2x2x2Block solver2x2x2("block2x2x2.3x3");
  SolverStep_2x2x3Block solver2x2x3("block2x2x3.3x3");
  SolverStep_2x2x3EO solver2x2x3EO("2faceEO.3x3");
*/
  SolverStep_EO solverEO("EO.3x3");
  SolverStep_DominoReduction solverDominoReduction("DominoReduction.3x3");
  SolverStep_DominoToDoubleTurns solverDominoToDoubleTurns("DominoToDoubleTurns.3x3");

  for (size_t i = 0; i < 100000; ++i)
  {
    auto scramble = scrambler.CreateRandomScramble(25);
    
    /*auto partialSolution_2x2x2 = SolveRandom(solver2x2x2, scramble, PartialSolution{ Solution{}, CubeGroup::Scrambled }, generator);
    auto partialSolution_2x2x3 = SolveRandom(solver2x2x3, scramble, partialSolution_2x2x2, generator);
    auto solution = SolveBest(solver2x2x3EO, scramble, partialSolution_2x2x3);*/
    auto partialSolution_EO = SolveRandom(solverEO, scramble, PartialSolution{ Solution{}, CubeGroup::Scrambled }, generator);
    auto partialSolution_Domino = SolveRandom(solverDominoReduction, scramble, partialSolution_EO, generator);
    auto solution = SolveBest(solverDominoToDoubleTurns, scramble, partialSolution_Domino);

    int stepLength = solution.solutionSoFar.Length() - partialSolution_Domino.solutionSoFar.Length();
    auto findIter = solveLengthCounts.find(stepLength);
    if (findIter == solveLengthCounts.end())
    {
      solveLengthCounts.insert(std::make_pair(stepLength, 1));
    }
    else
    {
      ++(findIter->second);
    }

    if (i % 1000 == 0)
    {
      std::cout << i << ' ';
    }
  }
  std::cout << "\n\n";

  for (const auto& solveLengthCount : solveLengthCounts)
  {
    std::cout << solveLengthCount.first << ": " << solveLengthCount.second << '\n';   
  }
}