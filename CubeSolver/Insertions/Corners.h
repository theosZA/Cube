#pragma once

#include <array>
#include <vector>

#include "Cube\3x3x3\Cube3x3x3.h"
#include "Cube\StickerPosition.h"
#include "CubeGroup.h"

namespace Corners {

// Returns all corners that are in the correct position and correctly oriented.
std::vector<StickerPosition> FindCorrectCorners(const Cube3x3x3& scrambledCube);

// Returns all corners that are either in the wrong position or in the correct position but incorrectly oriented.
std::vector<StickerPosition> FindWrongCorners(const Cube3x3x3& scrambledCube);

// Returns all corners that are in the wrong position.
std::vector<StickerPosition> FindMispermutedCorners(const Cube3x3x3& scrambledCube);
// Returns all corners that are either in the correct position but incorrectly oriented.
std::vector<StickerPosition> FindMisorientedCorners(const Cube3x3x3& scrambledCube);

// Returns the number of wrong corners in ABnC cases. All other cases (except solved) will throw an exception.
int GetNumberOfWrongCorners(CubeGroup cubeGroup);

// Determines which cube group the given cube belongs to based only on the corners which are incorrectly permuted or oriented.
CubeGroup GetCubeGroupForWrongCorners(const Cube3x3x3& cube);

// Returns the first corner 3-cycle if one is present on the cube.
// Throws an exception if there are no corner 3-cycles.
std::array<StickerPosition, 3> FindCorner3Cycle(const Cube3x3x3& cube);

}
