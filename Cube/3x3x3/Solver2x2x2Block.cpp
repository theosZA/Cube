#include "Solver2x2x2Block.h"

#include <array>

#include "..\StickerPosition.h"

static const std::array<StickerPosition, 4> blockCubies
{
  StickerPosition{ Face::Front, 1 },
  StickerPosition{ Face::Right, 1 },
  StickerPosition{ Face::Front, 5 },
  StickerPosition{ Face::Front, 2 }
};

static const std::vector<CubeMove> possibleMoves
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
: solver(
    maxMoves,
    // Get key
    [](const Cube3x3x3& cube)
    {
        return Solver2x2x2Block::GetKeyValue(cube);
    },
    possibleMoves,
    cacheFileName)
{}

std::vector<CubeMove> Solver2x2x2Block::Solve(const Cube3x3x3& cube) const
{
  return solver.Solve(cube);
}

std::vector<CubeMove> Solver2x2x2Block::Solve(const std::vector<CubeMove>& scramble) const
{
  Cube3x3x3 cube;
  cube += scramble;
  return Solve(cube);
}

std::vector<CubeMove> Solver2x2x2Block::Solve(const std::vector<CubeMove>& scramble, Face cornerFace, Face cornerFaceClockwise) const
{
  std::array<Face, 6> forwardMapping;
  forwardMapping[static_cast<size_t>(Face::Front)] = cornerFace;
  forwardMapping[static_cast<size_t>(Face::Up)] = cornerFaceClockwise;
  forwardMapping[static_cast<size_t>(Face::Right)] = GetNextFaceClockwise(cornerFace, cornerFaceClockwise);
  forwardMapping[static_cast<size_t>(Face::Back)] = GetOppositeFace(cornerFace);
  forwardMapping[static_cast<size_t>(Face::Down)] = GetOppositeFace(cornerFaceClockwise);
  forwardMapping[static_cast<size_t>(Face::Left)] = GetOppositeFace(GetNextFaceClockwise(cornerFace, cornerFaceClockwise));

  std::vector<CubeMove> proxyScramble;
  for (const auto& move : scramble)
    proxyScramble.push_back(CubeMove{ forwardMapping[static_cast<size_t>(move.face)], move.quarterRotationsClockwise });
  auto proxySolution = Solve(proxyScramble);

  std::array<Face, 6> backwardMapping;
  for (size_t i = 0; i < 6; ++i)
    backwardMapping[static_cast<size_t>(forwardMapping[i])] = static_cast<Face>(i);

  std::vector<CubeMove> solution;
  for (const auto& move : proxySolution)
    solution.push_back(CubeMove{ backwardMapping[static_cast<size_t>(move.face)], move.quarterRotationsClockwise });
  return solution;
}

std::uint32_t Solver2x2x2Block::GetRequiredMoveCount(const Cube3x3x3& cube) const
{
  return solver.GetRequiredMoveCount(cube);
}

std::uint32_t Solver2x2x2Block::GetRequiredMoveCount(const std::vector<CubeMove>& scramble) const
{
  Cube3x3x3 cube;
  cube += scramble;
  return GetRequiredMoveCount(cube);
}

std::uint32_t Solver2x2x2Block::GetRequiredMoveCount(const std::vector<CubeMove>& scramble, Face cornerFace, Face cornerFaceClockwise) const
{
  std::array<Face, 6> forwardMapping;
  forwardMapping[static_cast<size_t>(Face::Front)] = cornerFace;
  forwardMapping[static_cast<size_t>(Face::Up)] = cornerFaceClockwise;
  forwardMapping[static_cast<size_t>(Face::Right)] = GetNextFaceClockwise(cornerFace, cornerFaceClockwise);
  forwardMapping[static_cast<size_t>(Face::Back)] = GetOppositeFace(cornerFace);
  forwardMapping[static_cast<size_t>(Face::Down)] = GetOppositeFace(cornerFaceClockwise);
  forwardMapping[static_cast<size_t>(Face::Left)] = GetOppositeFace(GetNextFaceClockwise(cornerFace, cornerFaceClockwise));

  std::vector<CubeMove> proxyScramble;
  for (const auto& move : scramble)
    proxyScramble.push_back(CubeMove{ forwardMapping[static_cast<size_t>(move.face)], move.quarterRotationsClockwise });
  return GetRequiredMoveCount(proxyScramble);
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
