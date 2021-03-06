#pragma once

#include <array>
#include <vector>

#include "Cube\CubeMove.h"
#include "Cube\StickerPosition.h"

#include "Solution\Solution.h"
#include "Cube\3x3x3\Cube3x3x3.h"

namespace Corner3Cycle {

// Moves sticker 0 to position of sticker 1; 1 to 2; 2 to 0. Returns the shortest move sequence that does this.
std::vector<CubeMove> SolveThreeCorners(const std::array<StickerPosition, 3>& cornerStickers);

// After applying a skeleton solution to a cube you identify three corners that need to be cycled as
// specified by a sticker on each of their faces. Sticker 0 needs to move to sticker 1; 1 to 2; and 2 to 0.
// This function finds the best place to cycle these corners within the skeleton solution so as to have the
// shortest overall move count after simplifying. The combined skeleton plus insertion is returned.
Solution InsertCorner3CycleInSkeleton(const Solution& skeleton,
                                      const std::array<StickerPosition, 3>& skeletonCornerPositions);

// After applying a skeleton solution to a cube you identify three corners that need to be cycled as
// specified by a sticker on each of their faces. Sticker 0 needs to move to sticker 1; 1 to 2; and 2 to 0.
// This function gives you the minimal insertion at each possible position in the skeleton.
// For each insertion, the combined skeleton plus insertion is returned.
std::vector<Solution> FindAllCorner3CycleInsertions(const Solution& skeleton,
                                                    const std::array<StickerPosition, 3>& skeletonCornerPositions);

// Solves the first 3 corners that form a 3-cycle by inserting the cycle into the skeleton.
// Returns the new solution.
Solution SolveCorner3Cycle(Cube3x3x3 scrambledCube, const Solution& skeleton);

} // Corner3Cycle