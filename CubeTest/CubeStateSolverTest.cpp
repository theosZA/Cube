#include "catch.hpp"

#include "Graph\CubeStateSolver.h"

struct MockCube
{
  MockCube& operator+=(CubeMove newMove)
  {
    moves.push_back(newMove);
    return *this;
  }

  std::vector<CubeMove> moves;
};

std::uint32_t GetKey(const MockCube& cube)
{
  return static_cast<std::uint32_t>(std::hash<std::string>()(MoveSequenceToText(SimplifyMoveSequence(cube.moves))));
}

const std::vector<CubeMove> defaultMoves 
{
  CubeMove{ Face::Front, 1 },
  CubeMove{ Face::Front, -1 },
  CubeMove{ Face::Right, 1 },
  CubeMove{ Face::Right, -1 }
};

TEST_CASE("Cube state solver - Ensure we can solve the default cube on an empty graph", "[CubeStateSolver]")
{
  CubeStateSolver<MockCube> solver(0, GetKey, defaultMoves);
  CHECK(solver.Solve(MockCube{}) == std::vector<CubeMove>{});
}

TEST_CASE("Cube state solver - Ensure we can solve the default cube on a populated graph", "[CubeStateSolver]")
{
  CubeStateSolver<MockCube> solver(6, GetKey, defaultMoves);
  CHECK(solver.Solve(MockCube{}) == std::vector<CubeMove>{});
}

TEST_CASE("Cube state solver - Ensure we can find all length 1 solutions", "[CubeStateSolver]")
{
  CubeStateSolver<MockCube> solver(6, GetKey, defaultMoves);
  for (const auto& move : defaultMoves)
  {
    MockCube cube;
    cube += move;
    CHECK(solver.Solve(cube) == (std::vector<CubeMove>{ CubeMove{ move.face, -move.quarterRotationsClockwise } }));
  }
}

TEST_CASE("Cube state solver - Ensure we can find a length 3 solution", "[CubeStateSolver]")
{
  CubeStateSolver<MockCube> solver(6, GetKey, defaultMoves);
  MockCube cube;
  cube += CubeMove{ Face::Front, 1 };
  cube += CubeMove{ Face::Right, 1 };
  cube += CubeMove{ Face::Front, -1 };
  CHECK(solver.Solve(cube) == (std::vector<CubeMove>{ CubeMove{ Face::Front, 1 }, CubeMove{ Face::Right, -1 }, CubeMove{ Face::Front, -1 } }));
}

TEST_CASE("Cube state solver - Ensure we can find a maximum length solution", "[CubeStateSolver]")
{
  CubeStateSolver<MockCube> solver(3, GetKey, defaultMoves);
  MockCube cube;
  cube += CubeMove{ Face::Front, 1 };
  cube += CubeMove{ Face::Right, 1 };
  cube += CubeMove{ Face::Front, -1 };
  CHECK(solver.Solve(cube) == (std::vector<CubeMove>{ CubeMove{ Face::Front, 1 }, CubeMove{ Face::Right, -1 }, CubeMove{ Face::Front, -1 } }));
}

TEST_CASE("Cube state solver - Ensure we can't find a solution longer than maximum number of moves", "[CubeStateSolver]")
{
  CubeStateSolver<MockCube> solver(2, GetKey, defaultMoves);
  MockCube cube;
  cube += CubeMove{ Face::Front, 1 };
  cube += CubeMove{ Face::Right, 1 };
  cube += CubeMove{ Face::Front, -1 };
  CHECK_THROWS(solver.Solve(cube));
}
