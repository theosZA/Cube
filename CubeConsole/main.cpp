#include <map>
#include <iostream>
#include <random>
#include <stdexcept>

#include "..\Cube\3x3x3\CubeGroup.h"
#include "..\Cube\Scrambler\Scrambler.h"

#include "AnalysisSolver.h"

int main()
{
  try
  {
    std::random_device randomSource;
    Scrambler scrambler(randomSource());

    std::mt19937 generator;
  
    std::map<int, int> solveLengthCounts;
    AnalysisSolver solver;

    for (size_t i = 0; i < 100; ++i)
    {
      solver.SetScramble(scrambler.CreateRandomScramble(25));
      auto solution = solver.BestSolve(CubeGroup::Block2x2x3);

      int stepLength = solution.Length();
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
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}