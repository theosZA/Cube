#include "Solver2x2x2.h"

#include <array>
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

Solver2x2x2::Solver2x2x2(size_t maxMoves)
{
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
}

std::vector<CubeMove> Solver2x2x2::Solve(Cube2x2x2 cube)
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
