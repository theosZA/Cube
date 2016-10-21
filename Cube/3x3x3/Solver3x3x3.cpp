#include "Solver3x3x3.h"

#include <algorithm>
#include <iterator>
#include <map>

#include "Cube3x3x3.h"

Solver3x3x3::Solver3x3x3(const std::string& cache2x2x2FileName,
                         const std::string& cache2x2x3FileName)
: solver2x2x2(20, cache2x2x2FileName),
  solver2x2x3(20, cache2x2x3FileName)
{}

std::vector<CubeMove> Solver3x3x3::Solve(const std::vector<CubeMove>& scramble)
{
  auto scramblePlusSolution = scramble;
  std::vector<CubeMove> solution;

  auto step1 = solver2x2x2.Solve(scramble, Face::Back, Face::Left);
  std::copy(step1.begin(), step1.end(), std::back_inserter(scramblePlusSolution));
  solution = step1;

  auto step2 = solver2x2x3.Solve(scramblePlusSolution);
  std::copy(step2.begin(), step2.end(), std::back_inserter(scramblePlusSolution));
  std::copy(step2.begin(), step2.end(), std::back_inserter(solution));

  return solution;
}
