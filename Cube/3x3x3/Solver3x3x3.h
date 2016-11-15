#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "..\CubeMove.h"
#include "..\SolutionLogger.h"
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
  Solver3x3x3(const std::string& solutionLogFileName = "",
              const std::string& cache2x2x2FileName = "",
              const std::string& cache2x2x3FileName = "",
              const std::string& cache2FaceEOFileName = "",
              const std::string& cache2FaceAB5CFileName = "");

  // Returns the move sequence that solve the given scramble.
  std::vector<CubeMove> Solve(const std::vector<CubeMove>& scramble) const override;

private:
  // Remaps all faces in the move sequence such that the old faces are replaced by the new
  // faces and all other faces are mapped to correspond. Neither the old face nor the new face pairs
  // can be opposite faces.
  static std::vector<CubeMove> RemapFaces(const std::vector<CubeMove>& moves, std::pair<Face, Face> oldFaces, std::pair<Face, Face> newFaces);

  // Returns a solution for when all but 3 faces, F, U and R, are solved (i.e. a 2x2x2 block has been created).
  std::vector<CubeMove> Solve3Faces(const std::vector<CubeMove>& scramble, const std::vector<CubeMove>& solutionSoFar) const;

  // Returns a solution for when all but 2 faces, U and R, are solved.
  std::vector<CubeMove> Solve2Faces(const std::vector<CubeMove>& scramble, const std::vector<CubeMove>& solutionSoFar) const;

  mutable SolutionLogger solutionLogger;
  Solver2x2x2Block solver2x2x2;
  Solver2x2x3Block solver2x2x3;
  TwoFaceEO solver2FaceEO;
  TwoFaceAB5C solver2FaceAB5C;
};

