#include <map>
#include <iostream>
#include <random>
#include <stdexcept>

#include "Cube\Scrambler\Scrambler.h"
#include "CubeSolver\CubeGroup.h"
#include "CubeSolver\Insertions\SolverCorners.h"
#include "CubeSolver\Solver\Solver3x3x3.h"

int main()
{
  try
  {
    std::random_device randomSource;
    Scrambler scrambler(randomSource());

    std::mt19937 generator;
  
    std::map<int, int> solveLengthCounts;
    Solver3x3x3 solver;

    auto scramble = scrambler.CreateRandomScramble(25);
    solver.SetScramble(scramble);
    std::cout << MoveSequenceToText(scramble) << "\n\n";

    auto solution = solver.BestSolve(std::set<CubeGroup>{ CubeGroup::Solved });
    std::cout << solution << "\n\n";
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}