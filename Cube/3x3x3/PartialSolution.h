#pragma once

#include <utility>

#include "..\Solution.h"

#include "CubeGroup.h"

struct PartialSolution
{
  Solution solutionSoFar;
  CubeGroup cubeGroup;  // The solution so far leaves the cube in this group.

  std::pair<Face, Face> rotationOldFaces;
  std::pair<Face, Face> rotationNewFaces;

  PartialSolution(Solution, CubeGroup);
  PartialSolution(Solution, CubeGroup, std::pair<Face, Face> rotationOldFaces, std::pair<Face, Face> rotationNewFaces);

  // Rotates the partial solution, applying the solution so far to a different set of faces.
  // All moves using the old faces are replaced by the same move on the new faces, and similarly for all other corresponding faces.
  // Neither the old face pair nor the new face pairs can consist of opposite faces since that wouldn't uniquely identify a rotation.
  PartialSolution Rotate(std::pair<Face, Face> oldFaces, std::pair<Face, Face> newFaces) const;

  // Returns the partial solution such that the solution so far is inverted
  PartialSolution Invert() const;
};