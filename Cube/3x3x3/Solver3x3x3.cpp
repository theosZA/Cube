#include "Solver3x3x3.h"

#include "Cube3x3x3.h"
#include "SolverCorners.h"

Solver3x3x3::Solver3x3x3(const std::string& cache2x2x2FileName,
                         const std::string& cache2x2x3FileName,
                         const std::string& cache2FaceEOFileName,
                         const std::string& cache2FaceAB5CFileName)
: solver2x2x2(20, cache2x2x2FileName),
  solver2x2x3(20, cache2x2x3FileName),
  solver2FaceEO(cache2FaceEOFileName, cache2x2x2FileName),
  solver2FaceAB5C(20, cache2FaceAB5CFileName)
{}

std::vector<CubeMove> Solver3x3x3::Solve(const std::vector<CubeMove>& scramble) const
{
  auto step2x2x2 = solver2x2x2.Solve(scramble, Face::Back, Face::Left);

  return Solve3Faces(scramble, step2x2x2);
}

std::vector<CubeMove> Solver3x3x3::Solve3Faces(const std::vector<CubeMove>& scramble, const std::vector<CubeMove>& solutionSoFar) const
{
  return Solve2Faces(scramble, solutionSoFar + solver2x2x3.Solve(scramble + solutionSoFar));
}

std::vector<CubeMove> Solver3x3x3::Solve2Faces(const std::vector<CubeMove>& scramble, const std::vector<CubeMove>& solutionSoFar) const
{
  auto edgeOrientationStep = solver2FaceEO.Solve(scramble + solutionSoFar);
  auto ab5cStep = solver2FaceAB5C.Solve(scramble + solutionSoFar + edgeOrientationStep);
  auto skeleton = solutionSoFar + edgeOrientationStep + ab5cStep;
  return SolverCorners::SolveCorners(scramble, skeleton);
}

