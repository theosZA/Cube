#include "SolverStep_5Corners3Cycle.h"

#include <array>
#include <stdexcept>
#include <string>
#include <vector>

#include "Cube\3x3x3\CornerStructure.h"

#include "Insertions\Corner3Cycle.h"
#include "Insertions\Corners.h"

std::vector<PartialSolution> SolverStep_5Corners3Cycle::Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar)
{
  Cube3x3x3 cube;
  cube += scramble;
  cube += solutionSoFar.GetMoves();

  auto mispermutedCorners = Corners::FindMispermutedCorners(cube);
  if (mispermutedCorners.size() != 3)
  {
    throw std::domain_error(std::to_string(mispermutedCorners.size()) + " incorrectly permuted corners when expecting a twisted corner 3-cycle");
  }

  auto misorientedCorners = Corners::FindMisorientedCorners(cube);
  if (misorientedCorners.size() != 2)
  {
    throw std::domain_error(std::to_string(misorientedCorners.size()) + " correctly permuted but misoriented corners when expecting 2 misoriented corners");
  }
  std::vector<std::array<StickerPosition, 3>> misorientedCornerStickers;
  for (const auto& misorientedCorner : misorientedCorners)
  {
    misorientedCornerStickers.push_back(std::array<StickerPosition, 3>
    {
      misorientedCorner,
      CornerStructure::GetAdjacentCornerStickerPosition(misorientedCorner, true),
      CornerStructure::GetAdjacentCornerStickerPosition(misorientedCorner, false)
    });
  }

  std::vector<PartialSolution> partialSolutions;

  // Strategy 1: Cycle the 3-cycle to leave us with 2 or 3 corners twisted in place.
  {
    const auto& corner1 = mispermutedCorners[0];
    const auto& corner2 = cube[corner1];
    const auto& corner3 = cube[corner2];
    bool twisted3Cycle = (cube[corner3] != corner1);
    auto cubeGroup = twisted3Cycle ? CubeGroup::AB3C_twisted : CubeGroup::AB2C_twisted;
    auto cycle = std::array<StickerPosition, 3>{ corner1, corner2, corner3 };
    auto cycleSolutions = Corner3Cycle::FindAllCorner3CycleInsertions(solutionSoFar, cycle);
    for (const auto& cycleSolution : cycleSolutions)
    {
      partialSolutions.push_back(PartialSolution{ cycleSolution, cubeGroup });
    }
  }

  // Strategy 2: Cycle the 2 corners twisted in place with one of the corners in the 3-cycle.
  // Always start with the corner in the 3-cycle in its default orientation to avoid duplicating cycles.
  for (const auto& corner1 : mispermutedCorners)
  {
    for (size_t misorientedCorner1Index = 0; misorientedCorner1Index < 2; ++misorientedCorner1Index)
    {
      for (const auto& corner2 : misorientedCornerStickers[misorientedCorner1Index])
      {
        size_t misorientedCorner2Index = 1 - misorientedCorner1Index;
        for (const auto& corner3 : misorientedCornerStickers[misorientedCorner2Index])
        {
          auto cycle = std::array<StickerPosition, 3>{ corner1, corner2, corner3 };
          auto cycleSolutions = Corner3Cycle::FindAllCorner3CycleInsertions(solutionSoFar, cycle);
          for (const auto& cycleSolution : cycleSolutions)
          {
            partialSolutions.push_back(PartialSolution{ cycleSolution, CubeGroup::AB5C_5cycle });
          }
        }
      }
    }
  }

  return partialSolutions;
}
