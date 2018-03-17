#include "SolverStep_2CornersTwisted.h"

#include <array>
#include <stdexcept>
#include <string>
#include <vector>

#include "Cube\3x3x3\CornerStructure.h"

#include "Insertions\Corner3Cycle.h"
#include "Insertions\Corners.h"

std::vector<PartialSolution> SolverStep_2CornersTwisted::Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar)
{
  Cube3x3x3 cube;
  cube += scramble;
  cube += solutionSoFar.GetMoves();

  auto wrongCorners = Corners::FindWrongCorners(cube);
  if (wrongCorners.size() != 2)
  {
    throw std::domain_error(std::to_string(wrongCorners.size()) + " wrong corners when expecting 2 wrong corners");
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
  // Our 2 misoriented corners will be used in the corner 3-cycle, plus we'll need to use at least one other
  // corner on the cube. This 3rd corner will be in its default orientation to ensure we don't duplicate cycles.
  auto correctCorners = Corners::FindCorrectCorners(cube);
  for (const auto& correctCorner : correctCorners)
  {
    for (size_t wrongCorner1Index = 0; wrongCorner1Index < 2; ++wrongCorner1Index)
    {
      for (const auto& wrongCorner1 : wrongCornerStickers[wrongCorner1Index])
      {
        size_t wrongCorner2Index = 1 - wrongCorner1Index;
        for (const auto& wrongCorner2 : wrongCornerStickers[wrongCorner2Index])
        {
          auto cycle = std::array<StickerPosition, 3>{ correctCorner, wrongCorner1, wrongCorner2 };
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
