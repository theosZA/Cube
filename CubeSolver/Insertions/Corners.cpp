#include "Corners.h"

#include <array>

namespace Corners {

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

std::vector<StickerPosition> FindWrongCorners(const Cube3x3x3& scrambledCube)
{
  std::vector<StickerPosition> wrongCorners;
  for (const auto& corner : allCorners)
  {
    if (scrambledCube[corner] != corner)
    {
      wrongCorners.push_back(corner);
    }
  }
  return wrongCorners;
}

// Returns the number of wrong corners in ABnC cases. All other cases (except solved) will throw an exception.
int GetNumberOfWrongCorners(CubeGroup cubeGroup)
{
  switch (cubeGroup)
  {
  case CubeGroup::Solved:
    return 0;

  case CubeGroup::AB5C_twisted:
  case CubeGroup::AB5C_2cycles:
  case CubeGroup::AB5C_3cycle:
  case CubeGroup::AB5C_5cycle:
    return 5;

  case CubeGroup::AB4C_twisted:
  case CubeGroup::AB4C_3cycle:
  case CubeGroup::AB4C_2cycles:
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

CubeGroup GetCubeGroupForWrongCorners(const Cube3x3x3& cube)
{
  // Get all incorrect corners (wrong orientation or position).
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

}
