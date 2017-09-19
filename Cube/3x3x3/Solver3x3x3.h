#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "..\CubeMove.h"
#include "..\Solution.h"
#include "..\SolutionLogger.h"
#include "..\Solver.h"
#include "Corner3Cycle.h"
#include "Cube3x3x3.h"
#include "Solver2x2x2Block.h"
#include "Solver2x2x3Block.h"
#include "TwoFaceEO.h"
#include "TwoFaceAB3C.h"
#include "TwoFaceAB5C.h"

// A block-based solver for 3x3x3 cubes.
class Solver3x3x3 : public Solver
{
public:
  // Initializes the solver using the specified cache files.
  Solver3x3x3(const std::string& solutionLogFileName = "",
              const std::string& cache2x2x2FileName = "",
              const std::string& cache2x2x3FileName = "",
              const std::string& cache2FaceEOFileName = "",
              const std::string& cache2FaceAB5CFileName = "",
              const std::string& cache2FaceAB3CFileName = "");

  // Returns the move sequence that solve the given scramble.
  std::vector<CubeMove> Solve(const std::vector<CubeMove>& scramble) const override;

private:
  // Returns a solution for when all but 3 faces, F, U and R, are solved (i.e. a 2x2x2 block has been created).
  Solution Solve3Faces(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar) const;

  // Returns a solution for when all but 2 faces, U and R, are solved.
  Solution Solve2Faces(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar) const;

  // Returns a solution for when all but 2 faces, U and R, are solved, with U and R edges oriented correctly.
  Solution Solve2FacesEdgesOriented(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar) const;

  mutable SolutionLogger solutionLogger;
  Solver2x2x2Block solver2x2x2;
  Solver2x2x3Block solver2x2x3;
  TwoFaceEO solver2FaceEO;
  TwoFaceAB5C solver2FaceAB5C;
  TwoFaceAB3C solver2FaceAB3C;
};

