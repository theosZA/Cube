#include "Solver2x2x2.h"

#include <array>
#include <fstream>
#include <map>
#include <utility>

// We only include F, U and R moves since B, D and L are equivalent in a 2x2x2 by symmetry.
const std::array<CubeMove, 9> possibleMoves
{
  CubeMove{ Face::Right, 1 },
  CubeMove{ Face::Right, -1 },
  CubeMove{ Face::Right, 2 },
  CubeMove{ Face::Up, 1 },
  CubeMove{ Face::Up, -1 },
  CubeMove{ Face::Up, 2 },
  CubeMove{ Face::Front, 1 },
  CubeMove{ Face::Front, -1 },
  CubeMove{ Face::Front, 2 }
};

Solver2x2x2::Solver2x2x2(std::uint32_t maxMoves, const std::string& cacheFileName)
: graph(
    // Get key
    [](const Cube2x2x2& cube) 
    { 
      return cube.GetKey(); 
    },
    // Get adjacent vertices
    [](const Cube2x2x2& cube)
    {
      std::vector<std::pair<CubeMove, Cube2x2x2>> next;
      next.reserve(possibleMoves.size());
      for (const auto& move : possibleMoves)
      {
        auto nextCube = cube;
        nextCube += move;
        next.push_back(std::pair<CubeMove, Cube2x2x2>{ move, nextCube });
      }
      return next;
    })
{
  if (!ReadCacheFile(cacheFileName))
  {
    graph.Build(Cube2x2x2{}, maxMoves);
    WriteCacheFile(cacheFileName);
  }
}

std::vector<CubeMove> Solver2x2x2::Solve(Cube2x2x2 cube)
{
  std::map<Face, Face> faceMapping;
  auto downFace = cube[StickerPosition{ Face::Down, 2 }].face;
  auto backFace = cube[StickerPosition{ Face::Back, 3 }].face;
  faceMapping[downFace] = Face::Down;
  faceMapping[GetOppositeFace(downFace)] = Face::Up;
  faceMapping[backFace] = Face::Back;
  faceMapping[GetOppositeFace(backFace)] = Face::Front;
  faceMapping[GetNextFaceClockwise(downFace, backFace)] = Face::Left;
  faceMapping[GetOppositeFace(GetNextFaceClockwise(downFace, backFace))] = Face::Right;
  cube.RemapFaces(faceMapping[Face::Front], faceMapping[Face::Up]);

  return graph.FindShortestPathToRoot(cube);
}

void Solver2x2x2::WriteCacheFile(const std::string& cacheFileName)
{
  if (!cacheFileName.empty())
    graph.WriteToStream(std::ofstream(cacheFileName, std::ios::binary));
}

bool Solver2x2x2::ReadCacheFile(const std::string& cacheFileName)
{
  return !cacheFileName.empty() && graph.ReadFromStream(std::ifstream(cacheFileName, std::ios::binary));
}
