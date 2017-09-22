#include "PartialSolution.h"

#include <array>

PartialSolution::PartialSolution(Solution solutionSoFar, CubeGroup cubeGroup)
: solutionSoFar(solutionSoFar),
  cubeGroup(cubeGroup),
  rotationOldFaces(Face::Front, Face::Right),
  rotationNewFaces(Face::Front, Face::Right)  // no rotation
{}

PartialSolution::PartialSolution(Solution solutionSoFar, CubeGroup cubeGroup, std::pair<Face, Face> rotationOldFaces, std::pair<Face, Face> rotationNewFaces)
: solutionSoFar(solutionSoFar),
  cubeGroup(cubeGroup),
  rotationOldFaces(rotationOldFaces),
  rotationNewFaces(rotationNewFaces)
{}

PartialSolution PartialSolution::Rotate(std::pair<Face, Face> oldFaces, std::pair<Face, Face> newFaces) const
{
  // Determine how all the old faces map to the new faces.
  auto oldFace3 = GetNextFaceClockwise(oldFaces.first, oldFaces.second);
  auto newFace3 = GetNextFaceClockwise(newFaces.first, newFaces.second);
  std::array<Face, 6> newFace;
  newFace[static_cast<size_t>(oldFaces.first)] = newFaces.first;
  newFace[static_cast<size_t>(oldFaces.second)] = newFaces.second;
  newFace[static_cast<size_t>(oldFace3)] = newFace3;
  newFace[static_cast<size_t>(GetOppositeFace(oldFaces.first))] = GetOppositeFace(newFaces.first);
  newFace[static_cast<size_t>(GetOppositeFace(oldFaces.second))] = GetOppositeFace(newFaces.second);
  newFace[static_cast<size_t>(GetOppositeFace(oldFace3))] = GetOppositeFace(newFace3);

  return PartialSolution{ solutionSoFar.Rotate(oldFaces, newFaces),
                          cubeGroup, 
                          std::make_pair(newFace[static_cast<size_t>(rotationOldFaces.first)], newFace[static_cast<size_t>(rotationOldFaces.second)]),
                          std::make_pair(newFace[static_cast<size_t>(rotationNewFaces.first)], newFace[static_cast<size_t>(rotationNewFaces.second)]) };
}

PartialSolution PartialSolution::Invert() const
{
  return PartialSolution{ solutionSoFar.Invert(), cubeGroup, rotationNewFaces, rotationOldFaces };
}