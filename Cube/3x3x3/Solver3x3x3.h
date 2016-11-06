#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "..\CubeMove.h"
#include "..\Solver.h"
#include "Corner3Cycle.h"
#include "Cube3x3x3.h"
#include "Solver2x2x2Block.h"
#include "Solver2x2x3Block.h"
#include "TwoFaceEO.h"
#include "TwoFaceAB5C.h"

// A solver that extends a 2x2x2 block to a 2x2x3 block on a 3x3x3 cube.
// A block-based solver for 3x3x3 cubes.
class Solver3x3x3 : public Solver
{
public:
  // Initializes the solver using the specified cache files.
  Solver3x3x3(const std::string& cache2x2x2FileName = "",
              const std::string& cache2x2x3FileName = "",
              const std::string& cache2FaceEOFileName = "",
              const std::string& cache2FaceAB5CFileName = "");

  // Returns the move sequence that solve the given scramble.
  std::vector<CubeMove> Solve(const std::vector<CubeMove>& scramble) const override;

private:
  Solver2x2x2Block solver2x2x2;
  Solver2x2x3Block solver2x2x3;
  TwoFaceEO solver2FaceEO;
  TwoFaceAB5C solver2FaceAB5C;
};

