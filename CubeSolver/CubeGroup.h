#pragma once

#include <algorithm>
#include <array>
#include <set>
#include <string>
#include <stdexcept>

#include "Cube\3x3x3\CornerStructure.h"

// A descriptive element describing the general (non-mathematical) group that the cube belongs to.
// This can be used to determine what solver steps can be applied to it. For groups that have
// rotations, reflections, etc., there is exactly one canonical group.
enum CubeGroup
{
  Solved,
  Scrambled,
  Block2x2x2,     // block positioned at BDL
  Block2x2x3,     // block position at BDL-FDL, i.e. with U and R faces free
  Block2x2x3_EO,  // block positioned at BDL-FDL and edges oriented with respect to F and B faces
  F2L_FSlot_EO,   // F2L-1, free slot positioned at FR-FDR, U face free, and edges oriented with respect to F and B faces.
  F2L_BSlot_EO,   // F2L-1, free slot positioned at BR-BDR, U face free, and edges oriented with respect to F and B faces.
  AB5C_twisted,   // all 5 corners twisted in place
  AB5C_2cycles,   // 4 of the 5 corners form two distinct 2-cycle while the 5th corner is twisted in place
  AB5C_3cycle,    // 3 of the 5 corners form a 3-cycle while the other 2 corners are twisted in place
  AB5C_4cycle,    // 4 of the 5 corners form a 4-cycle while the 5th corner is twisted in place
  AB5C_5cycle,    // all 5 corners form a 5-cycle
  AB4C_twisted,   // all 4 corners twisted in place
  AB4C_3cycle,    // 3 of the 4 corners form a 3-cycle while the 4th corner is twisted in place
  AB4C_2cycles,   // the corners form two distinct 2-cycles
  AB4C_4cycle,    // all 4 corners form a 4-cycle
  AB3C_twisted,   // all 3 corners twisted in place
  AB3C_3cycle,    // all 3 corners form a 3-cycle
  AB2C_twisted    // both corners twisted in place
};

// Returns a reasonable lower bound on the number of moves that would be required to solve most cubes in the given group.
inline int EstimateMovesRequired(CubeGroup cubeGroup)
{
  switch (cubeGroup)
  {
    case Solved:        return 0;

    // 1 corner cycle insertion
    case AB3C_3cycle:   return 2;

    // 2 corner cycle insertions
    case AB2C_twisted:
    case AB3C_twisted:
    case AB4C_4cycle:
    case AB4C_2cycles:
    case AB4C_3cycle:
    case AB5C_5cycle:   return 4;

    // 3 corner cycle insertions
    case AB4C_twisted:
    case AB5C_4cycle:
    case AB5C_3cycle:
    case AB5C_2cycles:  return 6;

    // 4 corner cycle incertions
    case AB5C_twisted:  return 8;

    // Petrus block building
    case F2L_FSlot_EO:  
    case F2L_BSlot_EO:  return 6;   // 2 moves to permute remaining edges
    case Block2x2x3_EO: return 9;   // 3 moves to build another block with restricted movement
    case Block2x2x3:    return 11;  // 2 moves to orient edges
    case Block2x2x2:    return 13;  // 2 moves to extend to a 2x2x3
    case Scrambled:     return 15;  // 2 moves to build a 2x2x2

    default:
      throw std::domain_error("Unexpected cube group " + std::to_string(static_cast<int>(cubeGroup)));
  }
}

// Returns the number of wrong corners in ABnC cases. All other cases (except solved) will throw an exception.
inline int GetNumberOfWrongCorners(CubeGroup cubeGroup)
{
  switch (cubeGroup)
  {
    case CubeGroup::Solved: 
      return 0;

    case CubeGroup::AB5C_twisted:
    case CubeGroup::AB5C_2cycles:
    case CubeGroup::AB5C_3cycle:
    case CubeGroup::AB5C_4cycle:
    case CubeGroup::AB5C_5cycle:
      return 5;

    case CubeGroup::AB4C_twisted:
    case CubeGroup::AB4C_3cycle:
    case CubeGroup::AB4C_2cycles:
    case CubeGroup::AB4C_4cycle:
      return 4;

    case CubeGroup::AB3C_twisted:
    case CubeGroup::AB3C_3cycle:
      return 3;

    case CubeGroup::AB2C_twisted:
      return 2;

    default:
      throw std::domain_error("Cube group  " + std::to_string(static_cast<int>(cubeGroup)) + " is not a group with only corners unsolved");
  }
}

inline CubeGroup GetCubeGroupForSkeleton(const Cube3x3x3& cube)
{
  // Get all incorrect corners (wrong orientation or position).
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
  std::set<StickerPosition> wrongCorners;
  for (const auto& corner : allCorners)
  {
    if (cube[corner] != corner)
    {
      wrongCorners.insert(corner);
    }
  }
  auto numWrongCorners = wrongCorners.size();

  // Now determine the length of the (permutation) cycles of these wrong corners.
  // (1 for corners in the correct position but wrong orientation.)
  std::multiset<int> cycleLengths;
  while (!wrongCorners.empty())
  {
    auto startingCorner = *wrongCorners.begin();
    wrongCorners.erase(startingCorner);
    int cycleLength = 1;
    auto currentCorner = cube[startingCorner];
    while (!CornerStructure::AreStickersOnSameCubie(startingCorner, currentCorner))
    {
      auto findIter = std::find_if(wrongCorners.begin(), wrongCorners.end(),
        [&currentCorner](const StickerPosition& a)
        {
          return CornerStructure::AreStickersOnSameCubie(a, currentCorner);
        });
      wrongCorners.erase(findIter);
      currentCorner = cube[currentCorner];
      ++cycleLength;
    }
    cycleLengths.insert(cycleLength);
  }

  // Now we can determine the matching cube group.
  switch (numWrongCorners)
  {
    case 0:   return CubeGroup::Solved;
    
    case 2:   return CubeGroup::AB2C_twisted;

    case 3:
      switch (cycleLengths.size())
      {
        case 1:   return CubeGroup::AB3C_3cycle;
        case 3:   return CubeGroup::AB3C_twisted;
        default:  throw std::runtime_error("Unhandled cycle count for 3 wrong corners:" + std::to_string(cycleLengths.size()));
      }

    case 4:
      switch (cycleLengths.size())
      {
        case 1: return CubeGroup::AB4C_4cycle;
        case 2: 
          switch (*cycleLengths.begin())
          {
            case 1:   return CubeGroup::AB4C_3cycle;
            case 2:   return CubeGroup::AB4C_2cycles;
            default:  throw std::runtime_error("Unhandled smallest cycle length for 2 cycles on 4 wrong corners:" + std::to_string(*cycleLengths.begin()));
          }
        case 4: return CubeGroup::AB4C_twisted;
        default:  throw std::runtime_error("Unhandled cycle count for 4 wrong corners:" + std::to_string(cycleLengths.size()));
      }

    case 5:
      switch (cycleLengths.size())
      {
        case 1:   return CubeGroup::AB5C_5cycle;
        case 2:   return CubeGroup::AB5C_4cycle;
        case 3:
          switch (*cycleLengths.rbegin())
          {
            case 2:   return CubeGroup::AB5C_2cycles;
            case 3:   return CubeGroup::AB5C_3cycle;
            default:  throw std::runtime_error("Unhandled largest cycle length for 3 cycles on 5 wrong corners:" + std::to_string(*cycleLengths.rbegin()));
          }
        case 5:   return CubeGroup::AB5C_twisted;
        default:  throw std::runtime_error("Unhandled cycle count for 5 wrong corners:" + std::to_string(cycleLengths.size()));
      }

    default: throw std::runtime_error("Unhandled number of wrong corners: " + std::to_string(numWrongCorners));
  }
}