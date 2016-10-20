#pragma once

#include <memory>

#include "Solver.h"

namespace SolverFactory {
 
// Constructs Solver objects to solve cubes of various sizes.
std::unique_ptr<Solver> CreateSolver(size_t cubeSize);
      
}