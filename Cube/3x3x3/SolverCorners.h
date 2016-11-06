#pragma once

#include <vector>

#include "..\CubeMove.h"

namespace SolverCorners {

// Solves the corners in as few corner 3-cycles as possible, all inserted into the skeleton.
// Returns the new solution.
std::vector<CubeMove> SolveCorners(const std::vector<CubeMove>& scramble, const std::vector<CubeMove>& skeleton);

} // SolverCorners
