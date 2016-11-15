#pragma once

#include <vector>

#include "..\CubeMove.h"
#include "..\Solution.h"

namespace SolverCorners {

// Solves the corners in as few corner 3-cycles as possible, all inserted into the skeleton.
// Returns the new solution.
Solution SolveCorners(const std::vector<CubeMove>& scramble, const std::vector<CubeMove>& skeleton);

} // SolverCorners
