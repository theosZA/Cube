#include <algorithm>
#include <iostream>
#include <map>
#include <random>

#include "..\Cube\3x3x3\SolverStep_2x2x2Block.h"
#include "..\Cube\3x3x3\SolverStep_2x2x3Block.h"
#include "..\Cube\Scrambler\Scrambler.h"

template <class Random>
PartialSolution Solve2x2x2Random(SolverStep_2x2x2Block& solver, const std::vector<CubeMove>& scramble, Random& generator)
{
  auto solutions = solver.Solve(scramble, Solution{});
  auto inverseSolutions = solver.Solve(InvertMoveSequence(scramble), Solution{});
  for (auto& inverseSolution : inverseSolutions)
  {
    solutions.push_back(inverseSolution.Invert());
  }
  auto randomIndex = std::uniform_int_distribution<size_t>(0, solutions.size() - 1)(generator);
  return solutions[randomIndex];
}

PartialSolution Solve2x2x3Best(SolverStep_2x2x3Block& solver, const std::vector<CubeMove>& scramble, const PartialSolution& partialSolution)
{
  // Rotate to canonical orientation.
  auto rotatedScramble = Rotate(scramble, partialSolution.rotationOldFaces, partialSolution.rotationNewFaces);
  auto rotatedSolutionSoFar = partialSolution.solutionSoFar.Rotate(partialSolution.rotationOldFaces, partialSolution.rotationNewFaces);

  // Get solutions.
  auto rotatedSolutions = solver.Solve(rotatedScramble, rotatedSolutionSoFar);
  auto inverseRotatedSolutions = solver.Solve(InvertMoveSequence(rotatedScramble), rotatedSolutionSoFar.Invert());
  for (auto& inverseRotatedSolution : inverseRotatedSolutions)
  {
    rotatedSolutions.push_back(inverseRotatedSolution.Invert());
  }
  
  // Get best solution.
  auto bestRotatedSolution = std::min_element(rotatedSolutions.begin(), rotatedSolutions.end(),
      [](const PartialSolution& a, const PartialSolution& b)
      {
        return a.solutionSoFar.Length() < b.solutionSoFar.Length();
      });

  // Rotate back to original orientation.
  return bestRotatedSolution->Rotate(partialSolution.rotationNewFaces, partialSolution.rotationOldFaces);
}


int main()
{
  std::random_device randomSource;
  Scrambler scrambler(randomSource());

  std::mt19937 generator;
  
  std::map<int, int> solveLengthCounts;

  SolverStep_2x2x2Block solver2x2x2("block2x2x2.3x3");
  SolverStep_2x2x3Block solver2x2x3("block2x2x3.3x3");

  for (size_t i = 0; i < 50000; ++i)
  {
    auto scramble = scrambler.CreateRandomScramble(25);
    
    auto partialSolution = Solve2x2x2Random(solver2x2x2, scramble, generator);
    auto solution = Solve2x2x3Best(solver2x2x3, scramble, partialSolution);

    int stepLength = solution.solutionSoFar.Length() - partialSolution.solutionSoFar.Length();
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