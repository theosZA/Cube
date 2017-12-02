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

    auto scramble = scrambler.CreateRandomScramble(25);
    solver.SetScramble(scramble);
    std::cout << MoveSequenceToText(scramble) << "\n\n";

    auto solution = solver.BestSolve(std::set<CubeGroup>{ CubeGroup::F2L_FSlot_EO, CubeGroup::F2L_BSlot_EO });
    std::cout << solution << '\n';
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}