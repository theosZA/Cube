#include "SolverStep_3CornersTwisted.h"

#include <array>
#include <stdexcept>
#include <string>
#include <vector>

#include "Cube\3x3x3\CornerStructure.h"

#include "Insertions\Corner3Cycle.h"
#include "Insertions\Corners.h"

std::vector<PartialSolution> SolverStep_3CornersTwisted::Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar)
{
  Cube3x3x3 cube;
  cube += scramble;
  cube += solutionSoFar.GetMoves();

  auto wrongCorners = Corners::FindWrongCorners(cube);
  if (wrongCorners.size() != 3)
  {
    throw std::domain_error(std::to_string(wrongCorners.size()) + " wrong corners when expecting 3 wrong corners");
  }
  std::vector<std::array<StickerPosition, 3>> wrongCornerStickers;
  for (const auto& wrongCorner : wrongCorners)
  {
    wrongCornerStickers.push_back(std::array<StickerPosition, 3>
    {
      wrongCorner,
      CornerStructure::GetAdjacentCornerStickerPosition(wrongCorner, true),
      CornerStructure::GetAdjacentCornerStickerPosition(wrongCorner, false)
    });
  }

  std::vector<PartialSolution> partialSolutions;
  // We will cycle the 3 corners in any orientation. The first of the corners (index 0)
  // will be in its default orientation to ensure we don't duplicate cycles.
  const auto& corner1 = wrongCorners[0];
  for (size_t corner2Index = 1; corner2Index < 3; ++corner2Index)
  {
    for (const auto& corner2 : wrongCornerStickers[corner2Index])
    {
      size_t corner3Index = 3 - corner2Index;
      for (const auto& corner3 : wrongCornerStickers[corner3Index])
      {
        auto cycle = std::array<StickerPosition, 3>{ corner1, corner2, corner3 };
        auto cycleSolutions = Corner3Cycle::FindAllCorner3CycleInsertions(solutionSoFar, cycle);
        for (const auto& cycleSolution : cycleSolutions)
        {
          partialSolutions.push_back(PartialSolution{ cycleSolution, CubeGroup::AB3C_3cycle });
        }
      }
    }
  }
  return partialSolutions;
}
