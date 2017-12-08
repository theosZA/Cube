#include "SolverStep_4CornersTwisted.h"

#include <array>
#include <stdexcept>
#include <string>
#include <vector>

#include "Cube\3x3x3\CornerStructure.h"

#include "Insertions\Corner3Cycle.h"
#include "Insertions\Corners.h"

std::vector<PartialSolution> SolverStep_4CornersTwisted::Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar)
{
  Cube3x3x3 cube;
  cube += scramble;
  cube += solutionSoFar.GetMoves();

  auto wrongCorners = Corners::FindWrongCorners(cube);
  if (wrongCorners.size() != 4)
  {
    throw std::domain_error(std::to_string(wrongCorners.size()) + " wrong corners when expecting 4 wrong corners");
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
  // We can use any 3 corners in any orientation. The first of the corners (in order in the array of wrong corners)
  // will be in it's default orientation to ensure we don't duplicate cycles.
  for (size_t corner1Index = 0; corner1Index < 2; ++corner1Index)
  {
    const auto& corner1 = wrongCorners[corner1Index];
    for (size_t corner2Index = corner1Index + 1; corner2Index < 4; ++corner2Index)
    {
      for (const auto& corner2 : wrongCornerStickers[corner2Index])
      {
        for (size_t corner3Index = corner1Index + 1; corner3Index < 4; ++corner3Index)
        {
          if (corner2Index != corner3Index)
          {
            for (const auto& corner3 : wrongCornerStickers[corner3Index])
            {
              auto cycle = std::array<StickerPosition, 3>{ corner1, corner2, corner3 };
              auto cycleSolutions = Corner3Cycle::FindAllCorner3CycleInsertions(solutionSoFar, cycle);
              for (const auto& cycleSolution : cycleSolutions)
              {
                partialSolutions.push_back(PartialSolution{ cycleSolution, CubeGroup::AB4C_3cycle });
              }
            }
          }
        }
      }
    }
  }
  return partialSolutions;
}
