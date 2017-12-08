#pragma once

#include <vector>

#include "Cube\CubeMove.h"

#include "Solution\Solution.h"

namespace SolverCorners {

// Solves the corners in as few corner 3-cycles as possible, all inserted into the skeleton.
// Returns the new solution.
Solution SolveCorners(const std::vector<CubeMove>& scramble, const Solution& skeleton);

} // SolverCorners
