#include "Solver2x2x2Block.h"

#include <array>
#include <fstream>

#include "..\StickerPosition.h"

static const std::array<StickerPosition, 4> blockCubies
{
  StickerPosition{ Face::Front, 1 },
  StickerPosition{ Face::Right, 1 },
  StickerPosition{ Face::Front, 5 },
  StickerPosition{ Face::Front, 2 }
};

static const std::array<CubeMove, 18> possibleMoves
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

Solver2x2x2Block::Solver2x2x2Block(std::uint32_t maxMoves, const std::string& cacheFileName)
: graph(
    // Get key
    [](const Cube3x3x3& cube)
    {
       return Solver2x2x2Block::GetKeyValue(cube);
    },
    // Get adjacent vertices
    [](const Cube3x3x3& cube)
    {
      std::vector<std::pair<CubeMove, Cube3x3x3>> next;
      next.reserve(possibleMoves.size());
      for (const auto& move : possibleMoves)
      {
        auto nextCube = cube;
        nextCube += move;
        next.push_back(std::pair<CubeMove, Cube3x3x3>{ move, nextCube });
      }
      return next;
    })
{
  if (!ReadCacheFile(cacheFileName))
  {
    graph.Build(Cube3x3x3{}, maxMoves);
    WriteCacheFile(cacheFileName);
  }
}

std::vector<CubeMove> Solver2x2x2Block::Solve(const Cube3x3x3& cube)
{
  return graph.FindShortestPathToRoot(cube);
}

void Solver2x2x2Block::WriteCacheFile(const std::string& cacheFileName)
{
  if (!cacheFileName.empty())
    graph.WriteToStream(std::ofstream(cacheFileName, std::ios::binary));
}

bool Solver2x2x2Block::ReadCacheFile(const std::string& cacheFileName)
{
  return !cacheFileName.empty() && graph.ReadFromStream(std::ifstream(cacheFileName, std::ios::binary));
}

std::uint32_t Solver2x2x2Block::GetKeyValue(const Cube3x3x3& cube)
{
  // 3 edges + corner, each:
  //  3 bits = face
  //  4 bits = index
  std::uint32_t key = 0;
  for (const auto& cubie : blockCubies)
  {
    key <<= 7;
    auto position = cube.Find(cubie);
    key += (static_cast<std::uint32_t>(position.face) << 4) + static_cast<std::uint32_t>(position.index);
  }
  return key;
}
