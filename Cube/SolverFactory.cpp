#include "SolverFactory.h"

#include <stdexcept>
#include <string>

#include "2x2x2\Solver2x2x2.h"
#include "3x3x3\Solver2x2x2Block.h"

namespace SolverFactory {

std::unique_ptr<Solver> CreateSolver(size_t cubeSize)
{
  switch (cubeSize)
  {
    case 2: return std::unique_ptr<Solver>{ new Solver2x2x2(20, "solution.2x2") };
    // No complete 3x3x3 solver yet...
    case 3: return std::unique_ptr<Solver>{ new Solver2x2x2Block(20, "block2x2x2.3x3") };

    default:
      throw std::domain_error("No solver for cube size " + std::to_string(cubeSize));
  }
}

}