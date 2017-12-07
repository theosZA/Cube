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

    auto solution = solver.BestSolve(std::set<CubeGroup>
    {
      CubeGroup::Solved, 
      CubeGroup::AB5C_twisted,
      CubeGroup::AB5C_2cycles,
      CubeGroup::AB5C_3cycle,
      CubeGroup::AB5C_4cycle,
      CubeGroup::AB5C_5cycle,
      CubeGroup::AB4C_twisted,
      CubeGroup::AB4C_3cycle,
      CubeGroup::AB4C_2cycles,
      CubeGroup::AB4C_4cycle,
      CubeGroup::AB3C_twisted,
      CubeGroup::AB3C_3cycle,
      CubeGroup::AB2C_twisted
    });
    std::cout << solution << "\n\n";

    // Solve corners separately.
    std::cout << "Inserting corners...\n\n";
    auto fullSolution = SolverCorners::SolveCorners(scramble, solution);
    std::cout << fullSolution << '\n';
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}