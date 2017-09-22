#include <iostream>
#include <map>

#include "..\Cube\3x3x3\SolverStep_2x2x2Block.h"
#include "..\Cube\Scrambler\Scrambler.h"

int main()
{
  std::random_device randomSource;
  Scrambler scrambler(randomSource());

  std::map<int, int> solveLengthCounts;

  SolverStep_2x2x2Block solver("block2x2x2.3x3");

  for (size_t i = 0; i < 100000; ++i)
  {
    auto scramble = scrambler.CreateRandomScramble(25);
    int shortestSolutionLength = std::numeric_limits<int>::max();

    auto solutions = solver.Solve(scramble, Solution{});
    for (const auto& solution : solutions)
    {
      if (solution.solutionSoFar.Length() < shortestSolutionLength)
      {
        shortestSolutionLength = solution.solutionSoFar.Length();
      }
    }

    auto inverseSolutions = solver.Solve(InvertMoveSequence(scramble), Solution{});
    for (const auto& inverseSolution : inverseSolutions)
    {
      if (inverseSolution.solutionSoFar.Length() < shortestSolutionLength)
      {
        shortestSolutionLength = inverseSolution.solutionSoFar.Length();
      }
    }

    auto findIter = solveLengthCounts.find(shortestSolutionLength);
    if (findIter == solveLengthCounts.end())
    {
      solveLengthCounts.insert(std::make_pair(shortestSolutionLength, 1));
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