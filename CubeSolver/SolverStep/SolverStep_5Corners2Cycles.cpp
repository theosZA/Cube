#include "SolverStep_5Corners2Cycles.h"

#include <array>
#include <stdexcept>
#include <string>
#include <vector>

#include "Cube\3x3x3\CornerStructure.h"

#include "Insertions\Corner3Cycle.h"
#include "Insertions\Corners.h"

void FindAllInsertions(const Solution& solutionSoFar, const StickerPosition& corner1, const StickerPosition& corner2, const StickerPosition& corner3, CubeGroup resultingCubeGroup, std::vector<PartialSolution>& partialSolutions)
{
  auto cycle = std::array<StickerPosition, 3>{ corner1, corner2, corner3 };
  auto cycleSolutions = Corner3Cycle::FindAllCorner3CycleInsertions(solutionSoFar, cycle);
  for (const auto& cycleSolution : cycleSolutions)
  {
    partialSolutions.push_back(PartialSolution{ cycleSolution, resultingCubeGroup });
  }
}

std::vector<PartialSolution> SolverStep_5Corners2Cycles::Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar)
{
  Cube3x3x3 cube;
  cube += scramble;
  cube += solutionSoFar.GetMoves();

  auto mispermutedCorners = Corners::FindMispermutedCorners(cube);
  if (mispermutedCorners.size() != 4)
  {
    throw std::domain_error(std::to_string(mispermutedCorners.size()) + " incorrectly permuted corners when expecting 2 distinct 2-cycles");
  }
  std::vector<std::array<StickerPosition, 3>> mispermutedCornerStickers;
  for (const auto& mispermutedCorner : mispermutedCorners)
  {
    mispermutedCornerStickers.push_back(std::array<StickerPosition, 3>
    {
      mispermutedCorner,
      CornerStructure::GetAdjacentCornerStickerPosition(mispermutedCorner, true),
      CornerStructure::GetAdjacentCornerStickerPosition(mispermutedCorner, false)
    });
  }

  auto misorientedCorners = Corners::FindMisorientedCorners(cube);
  if (misorientedCorners.size() != 1)
  {
    throw std::domain_error(std::to_string(misorientedCorners.size()) + " correctly permuted but misoriented corners when expecting 1 misoriented corner");
  }
  const auto& misorientedCorner = misorientedCorners[0];
  std::array<StickerPosition, 3> misorientedCornerStickers
  {
    misorientedCorner,
    CornerStructure::GetAdjacentCornerStickerPosition(misorientedCorner, true),
    CornerStructure::GetAdjacentCornerStickerPosition(misorientedCorner, false)
  };

  std::vector<PartialSolution> partialSolutions;
  
  // Strategy 1: Fix one of the corners in a 2-cycle. The 3rd corner to use when cycling will be the corner that is twisted in place.
  // This will leave us with 2 2-cycles.
  for (const auto& corner1 : mispermutedCorners)
  {
    const auto& corner2 = cube[corner1];
    for (const auto& corner3 : misorientedCornerStickers)
    {
      FindAllInsertions(solutionSoFar, corner1, corner2, corner3, CubeGroup::AB4C_2cycles, partialSolutions);
    }
  }
  
  // Strategy 2: Fix one of the corners in a 2-cycle. The 3rd corner to use when cycling will be a corner in the other 2-cycle.
  // This will leave us with a twisted 3-cycle and a corner still twisted in place.
  for (const auto& corner1 : mispermutedCorners)
  {
    const auto& corner2 = cube[corner1];
    for (size_t candidateLastCornerIndex = 0; candidateLastCornerIndex < 4; ++candidateLastCornerIndex)
    {
      const auto& candidateLastCorner = mispermutedCorners[candidateLastCornerIndex];
      if (corner1 != candidateLastCorner && !CornerStructure::AreStickersOnSameCubie(corner2, candidateLastCorner))
      {
        for (const auto& corner3 : mispermutedCornerStickers[candidateLastCornerIndex])
        {
          FindAllInsertions(solutionSoFar, corner1, corner2, corner3, CubeGroup::AB4C_3cycle, partialSolutions);
        }
      }
    }
  }
  
  // Strategy 3: Cycle one corner from each 2-cycle and the corner that is twisted in place.
  // This will leave us with a 5-cycle.
  for (size_t corner1Index = 0; corner1Index < 4; ++corner1Index)
  {
    for (const auto& corner1 : mispermutedCornerStickers[corner1Index])
    {
      for (size_t corner2Index = 0; corner2Index < 4; ++corner2Index)
      {
        if (corner1Index != corner2Index && !CornerStructure::AreStickersOnSameCubie(corner1, cube[mispermutedCorners[corner2Index]]))
        {
          for (const auto& corner2 : mispermutedCornerStickers[corner2Index])
          {
            FindAllInsertions(solutionSoFar, corner1, corner2, misorientedCorner, CubeGroup::AB5C_5cycle, partialSolutions);
          }
        }
      }
    }
  }
  
  return partialSolutions;
}
