#include "catch.hpp"

#include "Solver2x2x2.h"

TEST_CASE("Ensure that the solved cube can be solved", "[Solver2x2x2]")
{
  Solver2x2x2 solver(0);
  CHECK(solver.Solve(Cube2x2x2{}).empty());
}

TEST_CASE("Ensure that all single move scrambles can be solved in one move on the F/U/R faces", "[Solver2x2x2]")
{
  Solver2x2x2 solver(1);
  CHECK(solver.Solve((Cube2x2x2&)(Cube2x2x2{} += "F")) == (std::vector<CubeMove>{ CubeMove{ Face::Front, -1 } }));
  CHECK(solver.Solve((Cube2x2x2&)(Cube2x2x2{} += "F2")) == (std::vector<CubeMove>{ CubeMove{ Face::Front, 2 } }));
  CHECK(solver.Solve((Cube2x2x2&)(Cube2x2x2{} += "F'")) == (std::vector<CubeMove>{ CubeMove{ Face::Front, 1 } }));
  CHECK(solver.Solve((Cube2x2x2&)(Cube2x2x2{} += "U")) == (std::vector<CubeMove>{ CubeMove{ Face::Up, -1 } }));
  CHECK(solver.Solve((Cube2x2x2&)(Cube2x2x2{} += "U2")) == (std::vector<CubeMove>{ CubeMove{ Face::Up, 2 } }));
  CHECK(solver.Solve((Cube2x2x2&)(Cube2x2x2{} += "U'")) == (std::vector<CubeMove>{ CubeMove{ Face::Up, 1 } }));
  CHECK(solver.Solve((Cube2x2x2&)(Cube2x2x2{} += "R")) == (std::vector<CubeMove>{ CubeMove{ Face::Right, -1 } }));
  CHECK(solver.Solve((Cube2x2x2&)(Cube2x2x2{} += "R2")) == (std::vector<CubeMove>{ CubeMove{ Face::Right, 2 } }));
  CHECK(solver.Solve((Cube2x2x2&)(Cube2x2x2{} += "R'")) == (std::vector<CubeMove>{ CubeMove{ Face::Right, 1 } }));
  CHECK(solver.Solve((Cube2x2x2&)(Cube2x2x2{} += "B")) == (std::vector<CubeMove>{ CubeMove{ Face::Front, -1 } }));
  CHECK(solver.Solve((Cube2x2x2&)(Cube2x2x2{} += "B2")) == (std::vector<CubeMove>{ CubeMove{ Face::Front, 2 } }));
  CHECK(solver.Solve((Cube2x2x2&)(Cube2x2x2{} += "B'")) == (std::vector<CubeMove>{ CubeMove{ Face::Front, 1 } }));
  CHECK(solver.Solve((Cube2x2x2&)(Cube2x2x2{} += "D")) == (std::vector<CubeMove>{ CubeMove{ Face::Up, -1 } }));
  CHECK(solver.Solve((Cube2x2x2&)(Cube2x2x2{} += "D2")) == (std::vector<CubeMove>{ CubeMove{ Face::Up, 2 } }));
  CHECK(solver.Solve((Cube2x2x2&)(Cube2x2x2{} += "D'")) == (std::vector<CubeMove>{ CubeMove{ Face::Up, 1 } }));
  CHECK(solver.Solve((Cube2x2x2&)(Cube2x2x2{} += "L")) == (std::vector<CubeMove>{ CubeMove{ Face::Right, -1 } }));
  CHECK(solver.Solve((Cube2x2x2&)(Cube2x2x2{} += "L2")) == (std::vector<CubeMove>{ CubeMove{ Face::Right, 2 } }));
  CHECK(solver.Solve((Cube2x2x2&)(Cube2x2x2{} += "L'")) == (std::vector<CubeMove>{ CubeMove{ Face::Right, 1 } }));
}

TEST_CASE("Ensure we can solve a scramble of only F/U/R moves", "[Solver2x2x2]")
{
  Solver2x2x2 solver(5);
  Cube2x2x2 cube;
  cube += "F2 U' R F' U2";
  auto solution = solver.Solve(cube);
  CHECK((cube += solver.Solve(cube)).IsSolved());
}

TEST_CASE("Ensure we can solve a scramble up to 5 moves", "[Solver2x2x2]")
{
  Solver2x2x2 solver(5);
  Cube2x2x2 cube;
  cube += "L' U2 R2 B' D";
  auto solution = solver.Solve(cube);
  CHECK((cube += solver.Solve(cube)).IsSolved());
}