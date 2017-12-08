#include "SolverStep_4Corners3Cycle.h"

#include <array>
#include <stdexcept>
#include <string>

#include "Cube\3x3x3\CornerStructure.h"

#include "Insertions\Corner3Cycle.h"
#include "Insertions\Corners.h"

std::vector<PartialSolution> SolverStep_4Corners3Cycle::Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar)
{
  Cube3x3x3 cube;
  cube += scramble;
  cube += solutionSoFar.GetMoves();

  // We always have to include the correctly positioned but misoriented corner in the corner 3-cycle to ensure it becomes part of the resulting corner 3-cycle.
  auto misorientedCorners = Corners::FindMisorientedCorners(cube);
  if (misorientedCorners.size() != 1)
  {
    throw std::domain_error(std::to_string(misorientedCorners.size()) + " correctly permuted but misoriented corners when expecting 1 misoriented corner");
  }
  std::array<StickerPosition, 3> misorientedCornerStickers
  {
    misorientedCorners[0],
    CornerStructure::GetAdjacentCornerStickerPosition(misorientedCorners[0], true),
    CornerStructure::GetAdjacentCornerStickerPosition(misorientedCorners[0], false)
  };
  // The other 2 corners to include must be part of the twisted corner 3-cycle. These are all the wrong corners except our misoriented corner.
  auto mispermutedCorners = Corners::FindMispermutedCorners(cube);
  if (mispermutedCorners.size() != 3)
  {
    throw std::domain_error(std::to_string(mispermutedCorners.size()) + " incorrectly permuted corners when expecting a twisted corner 3-cycle");
  }

  std::vector<PartialSolution> partialSolutions;
  for (const auto& corner1 : misorientedCornerStickers)
  {
    for (const auto& corner2 : mispermutedCorners)
    {
      auto corner3 = cube[corner2];
      auto cycle = std::array<StickerPosition, 3>{ corner1, corner2, corner3 };
      auto cycleSolutions = Corner3Cycle::FindAllCorner3CycleInsertions(solutionSoFar, cycle);
      for (const auto& cycleSolution : cycleSolutions)
      {
        partialSolutions.push_back(PartialSolution{ cycleSolution, CubeGroup::AB3C_3cycle });
      }
    }
  }
  return partialSolutions;
}
