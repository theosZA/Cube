#include "Solver2x2x2.h"

#include <algorithm>
#include <array>
#include <fstream>
#include <map>
#include <queue>
#include <stdexcept>
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

Solver2x2x2::Solver2x2x2(size_t maxMoves, const std::string& cacheFileName)
{
  if (ReadCacheFile(cacheFileName))
    return;

  struct Position
  {
    int positionKey;
    int movesToSolve;
  };
  std::queue<Position> positions;
  Cube2x2x2 startCube;
  movesToSolve.insert(std::pair<int, int>{ startCube.GetKey(), 0 });
  positions.push(Position{ startCube.GetKey(), 0 });

  // Generate all positions in the movesToSolve map that are reachable in maxMoves.
  while (!positions.empty() && positions.front().movesToSolve < maxMoves)
  {
    auto position = positions.front();
    positions.pop();
    // Generate all positions achievable from the current position. Only add the ones not
    // already in our map.
    for (const auto& move : possibleMoves)
    {
      Cube2x2x2 cube(position.positionKey);
      cube += move;
      auto newPosition = Position{ cube.GetKey(), position.movesToSolve + 1 };
      auto findIter = movesToSolve.find(newPosition.positionKey);
      if (findIter == movesToSolve.end())
      {
        movesToSolve.insert(std::pair<int, int>{ newPosition.positionKey, newPosition.movesToSolve });
        positions.push(newPosition);
      }
    }
  }

  WriteCacheFile(cacheFileName);
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

void Solver2x2x2::WriteCacheFile(const std::string& cacheFileName)
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

bool Solver2x2x2::ReadCacheFile(const std::string& cacheFileName)
{
  if (cacheFileName.empty())
    return false;

  std::ifstream cacheFile(cacheFileName, std::ios::binary);
  if (!cacheFile.good())
    return false;

  movesToSolve.clear();
  const size_t bufferSize = sizeof(std::pair<int, int>);
  char buffer[bufferSize];
  while (cacheFile.good() && !cacheFile.eof())
  {
    cacheFile.read(buffer, bufferSize);
    movesToSolve.insert(*reinterpret_cast<std::pair<int, int>*>(buffer));
  }
  return true;
}

int Solver2x2x2::SafeGetNumMovesToSolve(const Cube2x2x2& cube)
{
  auto findIter = movesToSolve.find(cube.GetKey());
  if (findIter == movesToSolve.end())
    return -1;
  return findIter->second;
}

int Solver2x2x2::GetNumMovesToSolve(const Cube2x2x2& cube)
{
  int movesRequired = SafeGetNumMovesToSolve(cube);
  if (movesRequired < 0)
    throw std::runtime_error("Cube position is not in the solver's memory");
  return movesRequired;
}

CubeMove Solver2x2x2::FindNextMove(const Cube2x2x2& cube)
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
