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
  // We current need a rotation of (A,B)->(C,D) to orient correctly.
  // We're rotating our current solution (W,X)->(Y,Z).
  // So the new rotation that we need to orient correctly is (Y,Z)->(W,X) followed by (A,B)->(C,D)

  // Create the mapping necessary to undo this new rotation. (Y,Z)->(W,X)
  auto oldFace3 = GetNextFaceClockwise(oldFaces.first, oldFaces.second);
  auto newFace3 = GetNextFaceClockwise(newFaces.first, newFaces.second);
  std::array<Face, 6> undoRotationFace;
  undoRotationFace[static_cast<size_t>(newFaces.first)] = oldFaces.first;
  undoRotationFace[static_cast<size_t>(newFaces.second)] = oldFaces.second;
  undoRotationFace[static_cast<size_t>(newFace3)] = oldFace3;
  undoRotationFace[static_cast<size_t>(GetOppositeFace(newFaces.first))] = GetOppositeFace(oldFaces.first);
  undoRotationFace[static_cast<size_t>(GetOppositeFace(newFaces.second))] = GetOppositeFace(oldFaces.second);
  undoRotationFace[static_cast<size_t>(GetOppositeFace(newFace3))] = GetOppositeFace(oldFace3);

  // Create the mapping to apply our own rotation. (A,B)->(C,D)
  auto ourOldFace3 = GetNextFaceClockwise(rotationOldFaces.first, rotationOldFaces.second);
  auto ourNewFace3 = GetNextFaceClockwise(rotationNewFaces.first, rotationNewFaces.second);
  std::array<Face, 6> thisRotationFace;
  thisRotationFace[static_cast<size_t>(rotationOldFaces.first)] = rotationNewFaces.first;
  thisRotationFace[static_cast<size_t>(rotationOldFaces.second)] = rotationNewFaces.second;
  thisRotationFace[static_cast<size_t>(ourOldFace3)] = ourNewFace3;
  thisRotationFace[static_cast<size_t>(GetOppositeFace(rotationOldFaces.first))] = GetOppositeFace(rotationNewFaces.first);
  thisRotationFace[static_cast<size_t>(GetOppositeFace(rotationOldFaces.second))] = GetOppositeFace(rotationNewFaces.second);
  thisRotationFace[static_cast<size_t>(GetOppositeFace(ourOldFace3))] = GetOppositeFace(ourNewFace3);

  // Now determine the combined rotation.
  auto frontMapsTo = thisRotationFace[static_cast<size_t>(undoRotationFace[static_cast<size_t>(Face::Front)])];
  auto upMapsTo = thisRotationFace[static_cast<size_t>(undoRotationFace[static_cast<size_t>(Face::Up)])];

  return PartialSolution{ solutionSoFar.Rotate(oldFaces, newFaces),
                          cubeGroup,
                          std::make_pair(Face::Front, Face::Up),
                          std::make_pair(frontMapsTo, upMapsTo) };
}

PartialSolution PartialSolution::Invert() const
{
  return PartialSolution{ solutionSoFar.Invert(), cubeGroup, rotationOldFaces, rotationNewFaces };
}