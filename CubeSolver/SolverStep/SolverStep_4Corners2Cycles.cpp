#include "SolverStep_4Corners2Cycles.h"

#include <array>
#include <stdexcept>
#include <string>
#include <vector>

#include "Cube\3x3x3\CornerStructure.h"

#include "Insertions\Corner3Cycle.h"
#include "Insertions\Corners.h"

std::vector<PartialSolution> SolverStep_4Corners2Cycles::Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar)
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
  // We need to solve 1 of the corners.
  for (const auto& corner1 : wrongCorners)
  {
    // The second corner is the one in corner 1's target position.
    const auto& corner2 = cube[corner1];
    // The third corner can be either of the other 2 corners in any orientation.
    for (size_t i = 0; i < 4; ++i)
    {
      const auto& cornerPosition = wrongCorners[i];
      if (corner1 != cornerPosition && !CornerStructure::AreStickersOnSameCubie(corner2, cornerPosition))
      {
        for (const auto& corner3 : wrongCornerStickers[i])
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
  }
  return partialSolutions;
}
