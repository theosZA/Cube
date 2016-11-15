#include "SolverCorners.h"

#include <algorithm>
#include <array>
#include <iterator>
#include <map>
#include <set>
#include <stdexcept>
#include <string>

#include "CornerStructure.h"
#include "Corner3Cycle.h"
#include "Cube3x3x3.h"

namespace SolverCorners {

Solution SolveCorners(const Cube3x3x3& scrambledCube, const std::vector<CubeMove>& skeleton);

static const std::set<StickerPosition> allCorners
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

// Removes the first instance of the given corner from the set of positions (need not be the exact same cubie face).
void RemoveCornerFromSet(std::set<StickerPosition>& positions, StickerPosition corner)
{
  for (auto i = positions.begin(); i != positions.end(); ++i)
    if (CornerStructure::AreStickersOnSameCubie(*i, corner))
    {
      positions.erase(i);
      return;
    }
}

// Returns 6 corners - all the corners except 2 specified (which need not be the exact cubie faces
// as specified in allCorners).
std::set<StickerPosition> GetAllCornersExcept(StickerPosition a, StickerPosition b)
{
  std::set<StickerPosition> remainingCorners = allCorners;
  RemoveCornerFromSet(remainingCorners, a);
  RemoveCornerFromSet(remainingCorners, b);
  return remainingCorners;
}

// Returns each corner which is not solved on the given cube.
std::set<StickerPosition> FindWrongCorners(const Cube3x3x3& scrambledCube)
{
  std::set<StickerPosition> wrongCorners;
  for (const auto& corner : allCorners)
  {
    if (scrambledCube[corner] != corner)
      wrongCorners.insert(corner);
  }
  return wrongCorners;
}

// Returns each corner which is not solved on the give cube plus skeleton.
std::set<StickerPosition> FindWrongCorners(Cube3x3x3 scrambledCube, const std::vector<CubeMove>& skeleton)
{
  scrambledCube += skeleton;
  return FindWrongCorners(scrambledCube);
}

// Returns true if all the specified corners of the cube are in the corrent position (but potentially misoriented).
bool AreCornersCorrectlyPermuted(Cube3x3x3 cube, const std::set<StickerPosition>& corners)
{
  auto isMisorientedCorner =
      [&](const StickerPosition& corner)
      {
        return CornerStructure::AreStickersOnSameCubie(cube[corner], corner);
      };
  return (std::find_if_not(corners.begin(), corners.end(), isMisorientedCorner) == corners.end());
}

// Finds the best solution that starts with a corner 3-cycle: A -> B -> one of the remaining corners (in some orientation)
Solution SolveCornersWith3Cycle(const Cube3x3x3& scrambledCube, const std::vector<CubeMove>& skeleton,
                                StickerPosition cycleCornerA, StickerPosition cycleCornerB,
                                const std::set<StickerPosition>& remainingCorners)
{
  bool haveSolution = false;
  Solution bestSolution;

  for (const auto& orientedCycleCornerC : remainingCorners)
  {
    auto cycleCornerCCandidates = std::array<StickerPosition, 3> {
      orientedCycleCornerC,
      CornerStructure::GetAdjacentCornerStickerPosition(orientedCycleCornerC, true),
      CornerStructure::GetAdjacentCornerStickerPosition(orientedCycleCornerC, false)
    };
    for (const auto& cycleCornerC : cycleCornerCCandidates)
    {
      auto cycle = std::array<StickerPosition, 3>{ cycleCornerA, cycleCornerB, cycleCornerC };
      auto cycleSolution = Corner3Cycle::InsertCorner3CycleInSkeleton(skeleton, cycle);
      auto solution = SolveCorners(scrambledCube, cycleSolution.moves);
      if (!haveSolution || solution.moves.size() < bestSolution.moves.size())
      {
        haveSolution = true;
        bestSolution = solution;
        std::copy(cycleSolution.steps.begin(), cycleSolution.steps.end(), std::front_inserter(bestSolution.steps));
      }
    }
  }

  if (!haveSolution)
    throw std::runtime_error("No solution found for corners starting with 2 corners specified for a corner 3-cycle");
  return bestSolution;
}

// Finds the best solution that starts with a corner 3-cycle involving the given corner.
// A -> one of the remaining corners (in some orientation) -> one of the other remaining corners (in some orientation)
Solution SolveCornersWith3Cycle(const Cube3x3x3& scrambledCube, const std::vector<CubeMove>& skeleton,
                                StickerPosition cycleCorner, const std::set<StickerPosition>& remainingCorners)
{
  bool haveSolution = false;
  Solution bestSolution;

  for (const auto& orientedNextCycleCorner : remainingCorners)
  {
    auto nextCycleCornerCandidates = std::array<StickerPosition, 3> {
      orientedNextCycleCorner,
      CornerStructure::GetAdjacentCornerStickerPosition(orientedNextCycleCorner, true),
      CornerStructure::GetAdjacentCornerStickerPosition(orientedNextCycleCorner, false)
    };
    for (const auto& nextCycleCorner : nextCycleCornerCandidates)
    {
      std::set<StickerPosition> finalCycleCorners = remainingCorners;
      RemoveCornerFromSet(finalCycleCorners, nextCycleCorner);
      auto solution = SolveCornersWith3Cycle(scrambledCube, skeleton, cycleCorner, nextCycleCorner, finalCycleCorners);
      if (!haveSolution || solution.moves.size() < bestSolution.moves.size())
      {
        haveSolution = true;
        bestSolution = solution;
      }
    }
  }

  if (!haveSolution)
    throw std::runtime_error("No solution found for corners starting with first corner specified for a corner 3-cycle");
  return bestSolution;
}

// Finds the best solution consisting of exactly 2 corner 3-cycles, starting with a 3-cycle: A -> B -> one of the remaining corners (in some orientation)
Solution SolveCornersWithPairOf3Cycles(const Cube3x3x3& scrambledCube, const std::vector<CubeMove>& skeleton,
                                       StickerPosition cycleCornerA, StickerPosition cycleCornerB,
                                       const std::set<StickerPosition>& remainingCorners)
{
  Cube3x3x3 cube = scrambledCube;
  cube += skeleton;

  bool haveSolution = false;
  Solution bestSolution;

  for (const auto& orientedCycleCornerC : remainingCorners)
  {
    auto cycleCornerCCandidates = std::array<StickerPosition, 3> {
      orientedCycleCornerC,
        CornerStructure::GetAdjacentCornerStickerPosition(orientedCycleCornerC, true),
        CornerStructure::GetAdjacentCornerStickerPosition(orientedCycleCornerC, false)
    };
    for (const auto& cycleCornerC : cycleCornerCCandidates)
    {
      auto cycle = std::array<StickerPosition, 3>{ cycleCornerA, cycleCornerB, cycleCornerC };

      // Before trying to insert the cycle, check that it results in a corner 3-cycle.
      Cube3x3x3 newCube = cube;
      newCube += Corner3Cycle::SolveThreeCorners(cycle);
      auto newWrongCorners = FindWrongCorners(newCube);
      if (newWrongCorners.size() == 3 && !AreCornersCorrectlyPermuted(newCube, newWrongCorners))
      {
        // This 3-cycle works. Get the best insert for it and then solve the last 3-cycle.
        auto cycleSolution = Corner3Cycle::InsertCorner3CycleInSkeleton(skeleton, cycle);
        auto solution = Corner3Cycle::SolveCorner3Cycle(scrambledCube, cycleSolution.moves);
        if (!haveSolution || solution.moves.size() < bestSolution.moves.size())
        {
          haveSolution = true;
          bestSolution = solution;
          std::copy(cycleSolution.steps.begin(), cycleSolution.steps.end(), std::front_inserter(bestSolution.steps));
        }
      }
    }
  }

  if (!haveSolution)
    throw std::runtime_error("No solution found for corners starting with 2 corners specified for a corner 3-cycle");
  return bestSolution;
}

// Solves the last 4 corners as 2 corner 3-cycles inserted into the skeleton.
// Must have exactly 4 wrong corners not all of which are correctly permuted but misoriented.
// Returns the new solution.
Solution SolveL4C(Cube3x3x3 scrambledCube, const std::vector<CubeMove>& skeleton)
{
  auto wrongCorners = FindWrongCorners(scrambledCube, skeleton);
  if (wrongCorners.size() != 4)
    throw std::domain_error(std::to_string(wrongCorners.size()) + " wrong corners when 4 were expected");

  Cube3x3x3 cube = scrambledCube;
  cube += skeleton;

  bool haveSolution = false;
  Solution bestSolution;

  for (const auto& wrongCorner : wrongCorners)
  {
    auto nextCorner = cube[wrongCorner];
    if (!CornerStructure::AreStickersOnSameCubie(nextCorner, wrongCorner))
    {
      auto solution = SolveCornersWithPairOf3Cycles(scrambledCube, skeleton, wrongCorner, nextCorner, GetAllCornersExcept(wrongCorner, nextCorner));
      if (!haveSolution || solution.moves.size() < bestSolution.moves.size())
      {
        haveSolution = true;
        bestSolution = solution;
      }
    }
  }

  if (!haveSolution)
    throw std::runtime_error("No solution found for L4C case");
  return bestSolution;
}

// Solves the last 2 corners as 2 corner 3-cycles inserted into the skeleton.
// Must have exactly 2 wrong corners. They will be correctly permuted but misoriented.
// Returns the new solution.
Solution SolveL2C(Cube3x3x3 scrambledCube, const std::vector<CubeMove>& skeleton)
{
  auto wrongCorners = FindWrongCorners(scrambledCube, skeleton);
  if (wrongCorners.size() != 2)
    throw std::domain_error(std::to_string(wrongCorners.size()) + " wrong corners when 2 were expected");

  auto cycleCornerA = *wrongCorners.begin();
  auto i = wrongCorners.begin();
  auto orientedCycleCornerB = *(++i);
  auto cycleCornerBCandidates = std::array<StickerPosition, 3> {
      orientedCycleCornerB,
      CornerStructure::GetAdjacentCornerStickerPosition(orientedCycleCornerB, true),
      CornerStructure::GetAdjacentCornerStickerPosition(orientedCycleCornerB, false)
  };
  std::set<StickerPosition> remainingCorners = GetAllCornersExcept(cycleCornerA, orientedCycleCornerB);
  
  bool haveSolution = false;
  Solution bestSolution;

  for (const auto& cycleCornerB : cycleCornerBCandidates)
  {
    auto solution = SolveCornersWithPairOf3Cycles(scrambledCube, skeleton, cycleCornerA, cycleCornerB, remainingCorners);
    if (!haveSolution || solution.moves.size() < bestSolution.moves.size())
    {
      haveSolution = true;
      bestSolution = solution;
    }
  }

  if (!haveSolution)
    throw std::runtime_error("No solution found for L2C case");
  return bestSolution;
}

// Solves the corners in as few corner 3-cycles as possible, all inserted into the skeleton.
// Returns the new solution.
Solution SolveCorners(const Cube3x3x3& scrambledCube, const std::vector<CubeMove>& skeleton)
{
  auto wrongCorners = FindWrongCorners(scrambledCube, skeleton);
  if (wrongCorners.empty())
    return Solution{ skeleton, std::deque<Solution::Step>() };
  if (wrongCorners.size() == 2)
    return SolveL2C(scrambledCube, skeleton);

  Cube3x3x3 cube = scrambledCube;
  cube += skeleton;
  if (AreCornersCorrectlyPermuted(cube, wrongCorners))
  { // All corners are correctly positioned but wrongly oriented. We need to cycle 3 of these corners.
    bool haveSolution = false;
    Solution bestSolution;
    for (const auto& corner : wrongCorners)
    {
      std::set<StickerPosition> remainingCorners = wrongCorners;
      RemoveCornerFromSet(remainingCorners, corner);
      auto solution = SolveCornersWith3Cycle(scrambledCube, skeleton, corner, remainingCorners);
      if (!haveSolution || solution.moves.size() < bestSolution.moves.size())
      {
        haveSolution = true;
        bestSolution = solution;
      }
    }
    if (!haveSolution)
      throw std::runtime_error("No solution found for permuted but misoriented corners");
    return bestSolution;
  }

  if (wrongCorners.size() == 3)
    return Corner3Cycle::SolveCorner3Cycle(scrambledCube, skeleton);
  if (wrongCorners.size() == 4)
    return SolveL4C(scrambledCube, skeleton);

  // More than 4 wrong corners.
  // Try all 3-cycles among our wrong corners that fix at least one corner.
  bool haveSolution = false;
  Solution bestSolution;
  for (const auto& corner1 : wrongCorners)
  {
    auto corner2 = cube[corner1];
    if (!CornerStructure::AreStickersOnSameCubie(corner1, corner2))
    {
      Solution solution;
      auto corner3 = cube[corner2];
      if (CornerStructure::AreStickersOnSameCubie(corner1, corner3))
      { 
        std::set<StickerPosition> remainingCorners = wrongCorners;
        RemoveCornerFromSet(remainingCorners, corner1);
        RemoveCornerFromSet(remainingCorners, corner2);
        solution = SolveCornersWith3Cycle(scrambledCube, skeleton, corner1, corner2, remainingCorners);
      }
      else
      {
        auto cycle = std::array<StickerPosition, 3>{ corner1, corner2, corner3 };
        auto cycleSolution = Corner3Cycle::InsertCorner3CycleInSkeleton(skeleton, cycle);
        solution = SolveCorners(scrambledCube, cycleSolution.moves);
        std::copy(cycleSolution.steps.begin(), cycleSolution.steps.end(), std::front_inserter(solution.steps));
      }
      if (!haveSolution || solution.moves.size() < bestSolution.moves.size())
      {
        haveSolution = true;
        bestSolution = solution;
      }
    }
  }
  if (!haveSolution)
    throw std::runtime_error("No solution found for corners");
  return bestSolution;
}

Solution SolveCorners(const std::vector<CubeMove>& scramble, const std::vector<CubeMove>& skeleton)
{
  Cube3x3x3 cube;
  cube += scramble;
  return SolveCorners(cube, skeleton);
}

} // SolverCorners