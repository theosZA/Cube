#pragma once

#include <deque>
#include <string>
#include <vector>

#include "CubeMove.h"

struct Solution
{
  std::vector<CubeMove> moves;

  struct Step
  {
    std::string description;
    std::vector<CubeMove> moves;
    std::vector<CubeMove> skeletonPreceedingMoves;
    std::vector<CubeMove> skeletonSucceedingMoves;
  };
  std::deque<Step> steps;

  // Adds a new step to the start of the solution.
  void InsertStep(const std::string& description, const std::vector<CubeMove>& moves)
  {
    steps.push_front(Step{ description, moves });
  }
};