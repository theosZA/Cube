#include "SolverStep_Corner5Cycle.h"

#include <array>
#include <stdexcept>
#include <string>

#include "Insertions\Corner3Cycle.h"
#include "Insertions\Corners.h"

std::vector<PartialSolution> SolverStep_Corner5Cycle::Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar)
{
  Cube3x3x3 cube;
  cube += scramble;
  cube += solutionSoFar.GetMoves();
  auto wrongCorners = Corners::FindWrongCorners(cube);
  if (wrongCorners.size() != 5)
  {
    throw std::domain_error(std::to_string(wrongCorners.size()) + " wrong corners when expecting a corner 5-cycle");
  }

  std::vector<PartialSolution> partialSolutions;
  for (const auto& corner1 : wrongCorners)
  {
    auto corner2 = cube[corner1];
    if (CornerStructure::AreStickersOnSameCubie(corner1, corner2))
    {
      throw std::domain_error("Correctly permuted but incorrectly oriented corner when expecting a corner 5-cycle");
    }

    Solution solution;
    auto corner3 = cube[corner2];
    if (CornerStructure::AreStickersOnSameCubie(corner1, corner3))
    {
      throw std::domain_error("Corner 2-cycle found when expecting a corner 5-cycle");
    }

    auto cycle = std::array<StickerPosition, 3>{ corner1, corner2, corner3 };
    auto cycleSolution = Corner3Cycle::InsertCorner3CycleInSkeleton(solutionSoFar, cycle);
    partialSolutions.push_back(PartialSolution{ cycleSolution, CubeGroup::AB3C_3cycle });
  }
  return partialSolutions;
}
