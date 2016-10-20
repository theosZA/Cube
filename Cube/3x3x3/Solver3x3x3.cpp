#include "Solver3x3x3.h"

#include <algorithm>
#include <iterator>

#include "Cube3x3x3.h"

Solver3x3x3::Solver3x3x3(const std::string& cache2x2x2FileName,
                         const std::string& cache2x2x3FileName)
: solver2x2x2(20, cache2x2x2FileName),
  solver2x2x3(20, cache2x2x3FileName)
{}

std::vector<CubeMove> Solver3x3x3::Solve(const Cube3x3x3& cube)
{
  auto solution = solver2x2x2.Solve(cube);
  auto solvingCube = cube;
  solvingCube += solution;
  auto nextStep = solver2x2x3.Solve(solvingCube);
  std::copy(nextStep.begin(), nextStep.end(), std::back_inserter(solution));
  return solution;
}

std::vector<CubeMove> Solver3x3x3::Solve(const std::vector<CubeMove>& scramble)
{
  Cube3x3x3 cube;
  cube += scramble;
  return Solve(cube);
}
