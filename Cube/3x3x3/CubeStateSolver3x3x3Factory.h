#pragma once

#include <memory>
#include <optional>
#include <string>

#include "..\Graph\CubeStateSolver.h"
#include "Cube3x3x3.h"
#include "CubeGroup.h"

namespace CubeStateSolver3x3x3Factory
{
  // Creates a solver to solve from a specified begin cube group to a specified end cube group.
  // The end cube group need not be provided when a solver can reach multiple possible groups
  // such as when completing a skeleton.
  // Will first try reading from the specified cache file, and only if that isn't found will the
  // solver generate the moves from scratch and then save to the cache file.
  std::unique_ptr<CubeStateSolver<Cube3x3x3>> CreateSolver(const std::string& cacheFileName, CubeGroup cubeGroupBegin, std::optional<CubeGroup> cubeGroupEnd = std::optional<CubeGroup>());
}