#include "Solver2x2x2Block.h"

#include <array>
#include <fstream>
#include <queue>
#include <stdexcept>

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

Solver2x2x2Block::Solver2x2x2Block(size_t maxMoves, const std::string& cacheFileName)
{
  if (ReadCacheFile(cacheFileName))
    return;

  struct Position
  {
    Cube3x3x3 cube;
    int movesToSolve;
  };
  std::queue<Position> positions;
  Cube3x3x3 startCube;
  movesToSolve.insert(std::pair<int, int>{ GetKeyValue(startCube), 0 });
  positions.push(Position{ std::move(startCube), 0 });

  // Generate all positions in the movesToSolve map that are reachable in maxMoves.
  while (!positions.empty() && positions.front().movesToSolve < maxMoves)
  {
    auto position = positions.front();
    positions.pop();
    // Generate all positions achievable from the current position. Only add the ones not
    // already in our map.
    for (const auto& move : possibleMoves)
    {
      auto cube = position.cube;
      cube += move;
      auto newPositionKey = GetKeyValue(cube);
      auto findIter = movesToSolve.find(newPositionKey);
      if (findIter == movesToSolve.end())
      {
        auto newMovesToSolve = position.movesToSolve + 1;
        movesToSolve.insert(std::pair<int, int>{ newPositionKey, newMovesToSolve });
        positions.push(Position{ std::move(cube), newMovesToSolve });
      }
    }
  }

  WriteCacheFile(cacheFileName);
}

std::vector<CubeMove> Solver2x2x2Block::Solve(Cube3x3x3 cube)
{
  int initialMovesRequired = GetNumMovesToSolve(cube);
  if (initialMovesRequired < 0)
    throw std::runtime_error("Cube position is not in the solver's memory");

  std::vector<CubeMove> solution;
  solution.reserve(initialMovesRequired);
  for (int i = 0; i < initialMovesRequired; ++i)
  {
    auto nextMove = FindNextMove(cube);
    cube += nextMove;
    solution.push_back(nextMove);
  }
  return solution;
}

void Solver2x2x2Block::WriteCacheFile(const std::string& cacheFileName)
{
  if (cacheFileName.empty())
    return;

  std::ofstream cacheFile(cacheFileName, std::ios::binary);
  for (const auto& positionPair : movesToSolve)
  {
    auto buffer = reinterpret_cast<const char*>(&positionPair);
    std::copy(buffer, buffer + sizeof(positionPair), std::ostream_iterator<char>(cacheFile));
  }
}

bool Solver2x2x2Block::ReadCacheFile(const std::string& cacheFileName)
{
  if (cacheFileName.empty())
    return false;

  std::ifstream cacheFile(cacheFileName, std::ios::binary);
  if (!cacheFile.good())
    return false;

  movesToSolve.clear();
  const size_t bufferSize = sizeof(std::pair<int, int>);
  char buffer[bufferSize];
  int count = 0;
  double sum = 0.0;
  while (cacheFile.good() && !cacheFile.eof())
  {
    cacheFile.read(buffer, bufferSize);
    ++count;
    sum += reinterpret_cast<std::pair<std::uint32_t, std::uint32_t>*>(buffer)->second;
    movesToSolve.insert(*reinterpret_cast<std::pair<std::uint32_t, std::uint32_t>*>(buffer));
  }
  auto avg = sum / count;
  return true;
}

int Solver2x2x2Block::SafeGetNumMovesToSolve(const Cube3x3x3& cube)
{
  auto findIter = movesToSolve.find(GetKeyValue(cube));
  if (findIter == movesToSolve.end())
    return -1;
  return findIter->second;
}

int Solver2x2x2Block::GetNumMovesToSolve(const Cube3x3x3& cube)
{
  int movesRequired = SafeGetNumMovesToSolve(cube);
  if (movesRequired < 0)
    throw std::runtime_error("Cube position is not in the solver's memory");
  return movesRequired;
}

CubeMove Solver2x2x2Block::FindNextMove(const Cube3x3x3& cube)
{
  int initialNumMovesRequired = GetNumMovesToSolve(cube);
  if (initialNumMovesRequired < 0)
    throw std::runtime_error("Cube position is not in the solver's memory");

  for (const auto& move : possibleMoves)
  {
    auto tryCube = cube;
    tryCube += move;
    int newNumMovesRequired = SafeGetNumMovesToSolve(tryCube);
    if (newNumMovesRequired == initialNumMovesRequired - 1)
      return move;
  }

  throw std::runtime_error("No move found which will advance the position");
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
