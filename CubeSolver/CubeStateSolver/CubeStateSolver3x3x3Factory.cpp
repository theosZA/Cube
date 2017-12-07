#include "CubeStateSolver3x3x3Factory.h"

#include <stdexcept>

namespace CubeStateSolver3x3x3Factory
{
  const std::uint32_t maxMoves = 20;

  const std::vector<CubeMove> allMoves
  {
    CubeMove{ Face::Right, 1 },
    CubeMove{ Face::Right, -1 },
    CubeMove{ Face::Right, 2 },
    CubeMove{ Face::Up, 1 },
    CubeMove{ Face::Up, -1 },
    CubeMove{ Face::Up, 2 },
    CubeMove{ Face::Front, 1 },
    CubeMove{ Face::Front, -1 },
    CubeMove{ Face::Front, 2 },
    CubeMove{ Face::Left, 1 },
    CubeMove{ Face::Left, -1 },
    CubeMove{ Face::Left, 2 },
    CubeMove{ Face::Down, 1 },
    CubeMove{ Face::Down, -1 },
    CubeMove{ Face::Down, 2 },
    CubeMove{ Face::Back, 2 },
    CubeMove{ Face::Back, -1 },
    CubeMove{ Face::Back, 1 }
  };

  static const std::vector<CubeMove> movesRUF
  {
    CubeMove{ Face::Right, 1 },
    CubeMove{ Face::Right, -1 },
    CubeMove{ Face::Right, 2 },
    CubeMove{ Face::Up, 1 },
    CubeMove{ Face::Up, -1 },
    CubeMove{ Face::Up, 2 },
    CubeMove{ Face::Front, 2 },
    CubeMove{ Face::Front, -1 },
    CubeMove{ Face::Front, 1 }
  };

  static const std::vector<CubeMove> movesRU
  {
    CubeMove{ Face::Right, 1 },
    CubeMove{ Face::Right, -1 },
    CubeMove{ Face::Right, 2 },
    CubeMove{ Face::Up, 1 },
    CubeMove{ Face::Up, -1 },
    CubeMove{ Face::Up, 2 },
  };

  template <class T>
  std::uint32_t CalculateKeyValueForCubies(const Cube3x3x3& cube, const T& cubies)
  {
    // Each cubie:
    //  3 bits = face
    //  4 bits = index
    std::uint32_t key = 0;
    for (const auto& cubie : cubies)
    {
      key <<= 7;
      auto position = cube.Find(cubie);
      key += (static_cast<std::uint32_t>(position.face) << 4) + static_cast<std::uint32_t>(position.index);
    }
    return key;
  }

  template <class T>
  std::uint32_t CalculateKeyValueForOrientedCubies(const Cube3x3x3& cube, const T& cubies)
  {
    // For each cubie, calculate it's position within the collection.
    int bitsPerCubie = 1;
    while (1U << bitsPerCubie < static_cast<std::uint32_t>(cubies.size()))
    {
      ++bitsPerCubie;
    }
    std::uint32_t key = 0;
    for (const auto& cubie : cubies)
    {
      key <<= bitsPerCubie;
      auto position = cube.Find(cubie);
      auto arrayIndex = std::distance(cubies.begin(), std::find(cubies.begin(), cubies.end(), position));
      key += static_cast<std::uint32_t>(arrayIndex);
    }
    return key;
  }

  template <class T>
  std::function<std::uint32_t(const Cube3x3x3&)> GetSimpleKeyValueFunction(const T& cubies)
  {
    return [=](const Cube3x3x3& cube)
    {
      return CalculateKeyValueForCubies(cube, cubies);
    };
  }

  template <class T, class U>
  std::function<std::uint32_t(const Cube3x3x3&)> GetComplexKeyValueFunction(const T& cubies, const U& orientedCubies)
  {
    return [=](const Cube3x3x3& cube)
    {
      auto orientiedCubiesKeyValue = CalculateKeyValueForOrientedCubies(cube, orientedCubies);
      auto nonOrientedCubiesKeyValue = CalculateKeyValueForCubies(cube, cubies);
      return (orientiedCubiesKeyValue << (7 * cubies.size())) + nonOrientedCubiesKeyValue;
    };
  }

  std::function<std::uint32_t(const Cube3x3x3&)> GetKeyValueFunction(CubeGroup cubeGroupBegin, std::optional<CubeGroup> cubeGroupEnd)
  {
    switch (cubeGroupBegin)
    {
      case CubeGroup::Scrambled:  // Build 2x2x2 block
        if (!cubeGroupEnd.has_value())
        {
          throw std::domain_error("An end cube group is required to determine key value function known starting from scrambled");
        }
        if (*cubeGroupEnd != CubeGroup::Block2x2x2)
        {
           throw std::domain_error("No key value function known starting from scrambled and ending in cube group " + std::to_string(static_cast<int>(*cubeGroupEnd)));
        }
        return GetSimpleKeyValueFunction(std::array<StickerPosition, 4>
          {
            StickerPosition{ Face::Left, 3 },
            StickerPosition{ Face::Left, 6 },
            StickerPosition{ Face::Left, 7 },
            StickerPosition{ Face::Back, 7 }
          });

      case CubeGroup::Block2x2x2: // Build 2x2x3 block
        if (!cubeGroupEnd.has_value())
        {
          throw std::domain_error("An end cube group is required to determine key value function known starting from a 2x2x2 block");
        }
        if (*cubeGroupEnd != CubeGroup::Block2x2x3)
        {
          throw std::domain_error("No key value function known starting from a 2x2x2 block and ending in cube group " + std::to_string(static_cast<int>(*cubeGroupEnd)));
        }
        return GetSimpleKeyValueFunction(std::array<StickerPosition, 3>
          {
            StickerPosition{ Face::Front, 3 },
            StickerPosition{ Face::Front, 6 },
            StickerPosition{ Face::Front, 7 }
          });

      case CubeGroup::Block2x2x3_EO:  // Complete F2L-1 - 2 possible options of where to leave the last unsolved slot.
        if (!cubeGroupEnd.has_value())
        {
          throw std::domain_error("An end cube group is required to determine key value function known starting from an edge-oriented 2x2x3 block");
        }
        switch (*cubeGroupEnd)
        {
          case CubeGroup::F2L_BSlot_EO:
            return GetSimpleKeyValueFunction(std::array<StickerPosition, 3>
              {
                StickerPosition{ Face::Right, 3 },
                StickerPosition{ Face::Right, 6 },
                StickerPosition{ Face::Right, 7 }
              });

          case CubeGroup::F2L_FSlot_EO:
            return GetSimpleKeyValueFunction(std::array<StickerPosition, 3>
            {
              StickerPosition{ Face::Right, 5 },
              StickerPosition{ Face::Right, 7 },
              StickerPosition{ Face::Right, 8 }
            });

          default:  throw std::domain_error("No key value function known starting from an edge-oriented 2x2x3 block and ending in cube group " + std::to_string(static_cast<int>(*cubeGroupEnd)));
        }

      case CubeGroup::F2L_BSlot_EO: // Solve remaining edges.
        if (cubeGroupEnd.has_value())
        {
          throw std::domain_error("An end cube group can't be specified when starting from an edge-oriented F2L-1 block");
        }
        return GetComplexKeyValueFunction(
          std::array<StickerPosition, 1>
          {
            StickerPosition{ Face::Right, 6 }
          },
          std::array<StickerPosition, 7>
          {
              StickerPosition{ Face::Up, 1 },
              StickerPosition{ Face::Up, 3 },
              StickerPosition{ Face::Up, 5 },
              StickerPosition{ Face::Up, 7 },
              StickerPosition{ Face::Front, 5 },
              StickerPosition{ Face::Back, 3 },
              StickerPosition{ Face::Down, 5 }
          });

      case CubeGroup::F2L_FSlot_EO: // Solve remaining edges.
        if (cubeGroupEnd.has_value())
        {
          throw std::domain_error("An end cube group can't be specified when starting from an edge-oriented F2L-1 block");
        }
        return GetComplexKeyValueFunction(
          std::array<StickerPosition, 1>
          {
            StickerPosition{ Face::Right, 8 }
          },
          std::array<StickerPosition, 7>
          {
            StickerPosition{ Face::Up, 1 },
            StickerPosition{ Face::Up, 3 },
            StickerPosition{ Face::Up, 5 },
            StickerPosition{ Face::Up, 7 },
            StickerPosition{ Face::Front, 5 },
            StickerPosition{ Face::Back, 3 },
            StickerPosition{ Face::Down, 5 }
          });

      default:  throw std::domain_error("No key value function known starting from cube group " + std::to_string(static_cast<int>(cubeGroupBegin)));
    }
  }

  const std::vector<CubeMove>& GetAllowedMoves(CubeGroup cubeGroup)
  {
    switch (cubeGroup)
    {
      case CubeGroup::Scrambled:      return allMoves;
      case CubeGroup::Block2x2x2:     return movesRUF;  // Don't break existing 2x2x2 block.
      case CubeGroup::Block2x2x3_EO:
      case CubeGroup::F2L_BSlot_EO:
      case CubeGroup::F2L_FSlot_EO:   return movesRU;   // Don't break existing 2x2x3 block and keep edges oriented.

      default:  throw std::domain_error("No known allowed moves for cube group " + std::to_string(static_cast<int>(cubeGroup)));
    }
  }

  std::unique_ptr<CubeStateSolver<Cube3x3x3>> CreateSolver(const std::string& cacheFileName, CubeGroup cubeGroupBegin, std::optional<CubeGroup> cubeGroupEnd)
  {
    return std::make_unique<CubeStateSolver<Cube3x3x3>>(maxMoves, GetKeyValueFunction(cubeGroupBegin, cubeGroupEnd), GetAllowedMoves(cubeGroupBegin), cacheFileName);
  }

} // namespace CubeStateSolver3x3x3Factory