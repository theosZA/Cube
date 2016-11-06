#include "SolverCorners.h"

#include <algorithm>
#include <array>
#include <map>
#include <stdexcept>
#include <string>

#include "CornerStructure.h"
#include "Corner3Cycle.h"
#include "Cube3x3x3.h"

namespace SolverCorners {

static const std::array<StickerPosition, 8> allCorners
{
  StickerPosition{ Face::Down, 0 },
  StickerPosition{ Face::Down, 2 },
  StickerPosition{ Face::Down, 6 },
  StickerPosition{ Face::Down, 8 },
  StickerPosition{ Face::Up, 0 },
  StickerPosition{ Face::Up, 2 },
  StickerPosition{ Face::Up, 6 },
  StickerPosition{ Face::Up, 8 }
};

// Returns each corner which is not solved on the given cube.
std::vector<StickerPosition> FindWrongCorners(const Cube3x3x3& scrambledCube)
{
  std::vector<StickerPosition> wrongCorners;
  for (const auto& corner : allCorners)
  {
    if (scrambledCube[corner] != corner)
      wrongCorners.push_back(corner);
  }
  return wrongCorners;
}

// Returns each corner which is not solved on the give cube plus skeleton.
std::vector<StickerPosition> FindWrongCorners(Cube3x3x3 scrambledCube, const std::vector<CubeMove>& skeleton)
{
  scrambledCube += skeleton;
  return FindWrongCorners(scrambledCube);
}

// Solves the last 3 corners as at most 2 corner 3-cycles inserted into the skeleton.
// Must have exactly 3 wrong corners.
// Returns the new solution.
std::vector<CubeMove> SolveL3C(Cube3x3x3 scrambledCube, const std::vector<CubeMove>& skeleton)
{
  auto wrongCorners = FindWrongCorners(scrambledCube, skeleton);
  if (wrongCorners.size() != 3)
    throw std::domain_error(std::to_string(wrongCorners.size()) + " wrong corners when 3 were expected");

  Cube3x3x3 cube = scrambledCube;
  cube += skeleton;
  auto cycle = CornerStructure::GetCornerCycle(cube, wrongCorners[0]);
  if (cycle.size() == 3)
  {
    return Corner3Cycle::SolveCorner3Cycle(scrambledCube, skeleton);
  }

  // We are left with all 3 corners in the right permutation but wrong orientation.
  // There are two options to solve now, either start with a clockwise cycle or an anti-clockwise cycle.
  // We choose the one that gives us the shortest solution.
  std::array<std::array<StickerPosition, 3>, 2> cycles
  {
    std::array<StickerPosition, 3>{ cycle[0], cycle[1], cycle[2] },
    std::array<StickerPosition, 3>{ cycle[0], cycle[2], cycle[1] }
  };
  std::array<std::vector<CubeMove>, 2> options
  {
    SolveL3C(scrambledCube, Corner3Cycle::InsertCorner3CycleInSkeleton(skeleton, cycles[0])),
    SolveL3C(scrambledCube, Corner3Cycle::InsertCorner3CycleInSkeleton(skeleton, cycles[1]))
  };
  if (options[0].size() <= options[1].size())
    return options[0];
  else
    return options[1];
}

// Solves the last 4 corners as at most 2 corner 3-cycles inserted into the skeleton.
// Must have exactly 4 wrong corners.
// Returns the new solution.
std::vector<CubeMove> SolveL4C(Cube3x3x3 scrambledCube, const std::vector<CubeMove>& skeleton)
{
  auto wrongCorners = FindWrongCorners(scrambledCube, skeleton);
  if (wrongCorners.size() != 4)
    throw std::domain_error(std::to_string(wrongCorners.size()) + " wrong corners when 4 were expected");

  Cube3x3x3 cube = scrambledCube;
  cube += skeleton;

  // We want to find a corner 3-cycle that will leave us with a corner 3-cycle.
  // This means we solve exactly one corner. Go through each of our unsolved corners in turn to try them.
  bool haveSolution = false;
  std::vector<CubeMove> bestSolution;
  for (const auto& wrongCorner : wrongCorners)
  {
    auto nextCorner = cube[wrongCorner];
    if (!CornerStructure::AreStickersOnSameCubie(nextCorner, wrongCorner))
    {
      // Go through all possible third corners.
      for (const auto& finalCornerPosition : allCorners)
        if (!CornerStructure::AreStickersOnSameCubie(finalCornerPosition, wrongCorner) && !CornerStructure::AreStickersOnSameCubie(finalCornerPosition, nextCorner))
        {
          auto finalCornerPositions = std::array<StickerPosition, 3> {
            finalCornerPosition,
            CornerStructure::GetAdjacentCornerStickerPosition(finalCornerPosition, true),
            CornerStructure::GetAdjacentCornerStickerPosition(finalCornerPosition, false)
          };
          for (const auto& finalCorner : finalCornerPositions)
          {
            auto cycle = std::array<StickerPosition, 3>{ wrongCorner, nextCorner, finalCorner };
            // Check that this will lead to a corner 3-cycle.
            Cube3x3x3 newCube = cube;
            newCube += Corner3Cycle::SolveThreeCorners(cycle);
            auto newWrongCorners = FindWrongCorners(newCube);
            if (newWrongCorners.size() == 3)
            {
              auto newCycle = CornerStructure::GetCornerCycle(newCube, newWrongCorners[0]);
              if (newCycle.size() == 3 && !CornerStructure::AreStickersOnOneOfCubies(newCycle[0], newCycle.begin() + 1, newCycle.end()))
              {
                // This 3-cycle works. Get the best insert for it and then solve the last 3-cycle.
                auto solution = Corner3Cycle::SolveCorner3Cycle(scrambledCube, Corner3Cycle::InsertCorner3CycleInSkeleton(skeleton, cycle));
                if (!haveSolution || solution.size() < bestSolution.size())
                {
                  haveSolution = true;
                  bestSolution = solution;
                }
              }
            }
          }
        }
    }
  }
  if (!haveSolution)
    throw std::runtime_error("No solution found for L4C case");
  return bestSolution;
}

// Solves the last 2 corners as 2 corner 3-cycles inserted into the skeleton.
// Must have exactly 2 wrong corners.
// Returns the new solution.
std::vector<CubeMove> SolveL2C(Cube3x3x3 scrambledCube, const std::vector<CubeMove>& skeleton)
{
  auto wrongCorners = FindWrongCorners(scrambledCube, skeleton);
  if (wrongCorners.size() != 2)
    throw std::domain_error(std::to_string(wrongCorners.size()) + " wrong corners when 2 were expected");

  bool haveFirstCycle = false;
  std::vector<CubeMove> bestFirstCycleInsert;
  for (const auto& rightCorner : allCorners)
  {
    if (CornerStructure::AreStickersOnOneOfCubies(rightCorner, wrongCorners.begin(), wrongCorners.end()))
      continue;

    auto rightCornerStickers = std::array<StickerPosition, 3> { 
      rightCorner,
      CornerStructure::GetAdjacentCornerStickerPosition(rightCorner, true),
      CornerStructure::GetAdjacentCornerStickerPosition(rightCorner, false) 
    };
    for (const auto& rightCornerSticker : rightCornerStickers)
      for (size_t i = 0; i <= 1; ++i)
      {
        auto candidate = Corner3Cycle::InsertCorner3CycleInSkeleton(skeleton, std::array<StickerPosition, 3>{ wrongCorners[i], wrongCorners[1 - i], rightCornerSticker });
        if (!haveFirstCycle || candidate.size() < bestFirstCycleInsert.size())
        {
          haveFirstCycle = true;
          bestFirstCycleInsert = candidate;
        }
      }
  }

  return Corner3Cycle::SolveCorner3Cycle(scrambledCube, bestFirstCycleInsert);
}

// Solves the corners in as few corner 3-cycles as possible, all inserted into the skeleton.
// Returns the new solution.
std::vector<CubeMove> SolveCorners(const Cube3x3x3& scrambledCube, const std::vector<CubeMove>& skeleton)
{
  auto wrongCorners = FindWrongCorners(scrambledCube, skeleton);
  switch (wrongCorners.size())
  {
    case 0: return skeleton;
    case 2: return SolveL2C(scrambledCube, skeleton);
    case 3: return SolveL3C(scrambledCube, skeleton);
    case 4: return SolveL4C(scrambledCube, skeleton);
    default:;
  }

  // More than 4 corners.
  Cube3x3x3 cube = scrambledCube;
  cube += skeleton;
  bool haveSolution = false;
  std::vector<CubeMove> bestSolution;
  for (const auto& corner1 : wrongCorners)
  {
    auto corner2 = cube[corner1];
    if (!CornerStructure::AreStickersOnSameCubie(corner1, corner2))
      for (const auto& corner3Position : wrongCorners)
        if (!CornerStructure::AreStickersOnSameCubie(corner1, corner3Position) && !CornerStructure::AreStickersOnSameCubie(corner2, corner3Position))
        {
          auto corner3Positions = std::array<StickerPosition, 3> {
            corner3Position,
            CornerStructure::GetAdjacentCornerStickerPosition(corner3Position, true),
            CornerStructure::GetAdjacentCornerStickerPosition(corner3Position, false)
          };
          for (const auto& corner3 : corner3Positions)
          {
            auto cycle = std::array<StickerPosition, 3>{ corner1, corner2, corner3 };
            auto solution = SolveCorners(scrambledCube, Corner3Cycle::InsertCorner3CycleInSkeleton(skeleton, cycle));
            if (!haveSolution || solution.size() < bestSolution.size())
            {
              haveSolution = true;
              bestSolution = solution;
            }
          }
        }
  }
  if (!haveSolution)
    throw std::runtime_error("No solution found for corners");
  return bestSolution;
}

std::vector<CubeMove> SolveCorners(const std::vector<CubeMove>& scramble, const std::vector<CubeMove>& skeleton)
{
  Cube3x3x3 cube;
  cube += scramble;
  return SolveCorners(cube, skeleton);
}

} // SolverCorners