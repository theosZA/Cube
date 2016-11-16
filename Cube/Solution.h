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
  void InsertStep(const std::string& description, const std::vector<CubeMove>& moves);

  // Rotates the cube, applying the solution and steps to a different set of faces.
  // All moves using the old faces are replaced by the same move the new faces, and similarly
  // for all other corresponding faces.
  // Neither the old face pair nor the new face pairs can consist of opposite faces since that
  // wouldn't uniquely identify a rotation.
  Solution Rotate(std::pair<Face, Face> oldFaces, std::pair<Face, Face> newFaces) const;
};