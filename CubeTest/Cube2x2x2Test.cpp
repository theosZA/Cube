#include "catch.hpp"

#include "Cube2x2x2.h"

TEST_CASE("2x2x2 - Ensure that the default position is correct", "[Cube2x2x2]")
{
  Cube2x2x2 cube;
  CHECK((cube[StickerPosition{ Face::Up, 0 }]) == (StickerPosition{ Face::Up, 0 }));
  CHECK((cube[StickerPosition{ Face::Up, 1 }]) == (StickerPosition{ Face::Up, 1 }));
  CHECK((cube[StickerPosition{ Face::Up, 2 }]) == (StickerPosition{ Face::Up, 2 }));
  CHECK((cube[StickerPosition{ Face::Up, 3 }]) == (StickerPosition{ Face::Up, 3 }));
  CHECK((cube[StickerPosition{ Face::Down, 0 }]) == (StickerPosition{ Face::Down, 0 }));
  CHECK((cube[StickerPosition{ Face::Down, 1 }]) == (StickerPosition{ Face::Down, 1 }));
  CHECK((cube[StickerPosition{ Face::Down, 2 }]) == (StickerPosition{ Face::Down, 2 }));
  CHECK((cube[StickerPosition{ Face::Down, 3 }]) == (StickerPosition{ Face::Down, 3 }));
  CHECK((cube[StickerPosition{ Face::Front, 0 }]) == (StickerPosition{ Face::Front, 0 }));
  CHECK((cube[StickerPosition{ Face::Front, 1 }]) == (StickerPosition{ Face::Front, 1 }));
  CHECK((cube[StickerPosition{ Face::Front, 2 }]) == (StickerPosition{ Face::Front, 2 }));
  CHECK((cube[StickerPosition{ Face::Front, 3 }]) == (StickerPosition{ Face::Front, 3 }));
  CHECK((cube[StickerPosition{ Face::Back, 0 }]) == (StickerPosition{ Face::Back, 0 }));
  CHECK((cube[StickerPosition{ Face::Back, 1 }]) == (StickerPosition{ Face::Back, 1 }));
  CHECK((cube[StickerPosition{ Face::Back, 2 }]) == (StickerPosition{ Face::Back, 2 }));
  CHECK((cube[StickerPosition{ Face::Back, 3 }]) == (StickerPosition{ Face::Back, 3 }));
  CHECK((cube[StickerPosition{ Face::Left, 0 }]) == (StickerPosition{ Face::Left, 0 }));
  CHECK((cube[StickerPosition{ Face::Left, 1 }]) == (StickerPosition{ Face::Left, 1 }));
  CHECK((cube[StickerPosition{ Face::Left, 2 }]) == (StickerPosition{ Face::Left, 2 }));
  CHECK((cube[StickerPosition{ Face::Left, 3 }]) == (StickerPosition{ Face::Left, 3 }));
  CHECK((cube[StickerPosition{ Face::Right, 0 }]) == (StickerPosition{ Face::Right, 0 }));
  CHECK((cube[StickerPosition{ Face::Right, 1 }]) == (StickerPosition{ Face::Right, 1 }));
  CHECK((cube[StickerPosition{ Face::Right, 2 }]) == (StickerPosition{ Face::Right, 2 }));
  CHECK((cube[StickerPosition{ Face::Right, 3 }]) == (StickerPosition{ Face::Right, 3 }));
}

TEST_CASE("2x2x2 - Ensure that we find the cubies in the correct place in the default position", "[Cube2x2x2]")
{
  Cube2x2x2 cube;
  CHECK(cube.Find(StickerPosition{ Face::Up, 0 }) == (StickerPosition{ Face::Up, 0 }));
  CHECK(cube.Find(StickerPosition{ Face::Up, 1 }) == (StickerPosition{ Face::Up, 1 }));
  CHECK(cube.Find(StickerPosition{ Face::Up, 2 }) == (StickerPosition{ Face::Up, 2 }));
  CHECK(cube.Find(StickerPosition{ Face::Up, 3 }) == (StickerPosition{ Face::Up, 3 }));
  CHECK(cube.Find(StickerPosition{ Face::Down, 0 }) == (StickerPosition{ Face::Down, 0 }));
  CHECK(cube.Find(StickerPosition{ Face::Down, 1 }) == (StickerPosition{ Face::Down, 1 }));
  CHECK(cube.Find(StickerPosition{ Face::Down, 2 }) == (StickerPosition{ Face::Down, 2 }));
  CHECK(cube.Find(StickerPosition{ Face::Down, 3 }) == (StickerPosition{ Face::Down, 3 }));
  CHECK(cube.Find(StickerPosition{ Face::Front, 0 }) == (StickerPosition{ Face::Front, 0 }));
  CHECK(cube.Find(StickerPosition{ Face::Front, 1 }) == (StickerPosition{ Face::Front, 1 }));
  CHECK(cube.Find(StickerPosition{ Face::Front, 2 }) == (StickerPosition{ Face::Front, 2 }));
  CHECK(cube.Find(StickerPosition{ Face::Front, 3 }) == (StickerPosition{ Face::Front, 3 }));
  CHECK(cube.Find(StickerPosition{ Face::Back, 0 }) == (StickerPosition{ Face::Back, 0 }));
  CHECK(cube.Find(StickerPosition{ Face::Back, 1 }) == (StickerPosition{ Face::Back, 1 }));
  CHECK(cube.Find(StickerPosition{ Face::Back, 2 }) == (StickerPosition{ Face::Back, 2 }));
  CHECK(cube.Find(StickerPosition{ Face::Back, 3 }) == (StickerPosition{ Face::Back, 3 }));
  CHECK(cube.Find(StickerPosition{ Face::Left, 0 }) == (StickerPosition{ Face::Left, 0 }));
  CHECK(cube.Find(StickerPosition{ Face::Left, 1 }) == (StickerPosition{ Face::Left, 1 }));
  CHECK(cube.Find(StickerPosition{ Face::Left, 2 }) == (StickerPosition{ Face::Left, 2 }));
  CHECK(cube.Find(StickerPosition{ Face::Left, 3 }) == (StickerPosition{ Face::Left, 3 }));
  CHECK(cube.Find(StickerPosition{ Face::Right, 0 }) == (StickerPosition{ Face::Right, 0 }));
  CHECK(cube.Find(StickerPosition{ Face::Right, 1 }) == (StickerPosition{ Face::Right, 1 }));
  CHECK(cube.Find(StickerPosition{ Face::Right, 2 }) == (StickerPosition{ Face::Right, 2 }));
  CHECK(cube.Find(StickerPosition{ Face::Right, 3 }) == (StickerPosition{ Face::Right, 3 }));
}

TEST_CASE("2x2x2 - Ensure that we can apply a move: U", "[Cube2x2x2]")
{
  Cube2x2x2 cube;
  cube += CubeMove { Face::Up, 1 };
  CHECK((cube[StickerPosition{ Face::Up, 0 }]) == (StickerPosition{ Face::Up, 2 }));
  CHECK((cube[StickerPosition{ Face::Up, 1 }]) == (StickerPosition{ Face::Up, 0 }));
  CHECK((cube[StickerPosition{ Face::Up, 2 }]) == (StickerPosition{ Face::Up, 3 }));
  CHECK((cube[StickerPosition{ Face::Up, 3 }]) == (StickerPosition{ Face::Up, 1 }));
  CHECK((cube[StickerPosition{ Face::Down, 0 }]) == (StickerPosition{ Face::Down, 0 }));
  CHECK((cube[StickerPosition{ Face::Down, 1 }]) == (StickerPosition{ Face::Down, 1 }));
  CHECK((cube[StickerPosition{ Face::Down, 2 }]) == (StickerPosition{ Face::Down, 2 }));
  CHECK((cube[StickerPosition{ Face::Down, 3 }]) == (StickerPosition{ Face::Down, 3 }));
}

TEST_CASE("2x2x2 - Ensure that we can apply a move: L", "[Cube2x2x2]")
{
  Cube2x2x2 cube;
  cube += CubeMove{ Face::Left, 1 };
  CHECK((cube[StickerPosition{ Face::Left, 0 }]) == (StickerPosition{ Face::Left, 2 }));
  CHECK((cube[StickerPosition{ Face::Left, 1 }]) == (StickerPosition{ Face::Left, 0 }));
  CHECK((cube[StickerPosition{ Face::Left, 2 }]) == (StickerPosition{ Face::Left, 3 }));
  CHECK((cube[StickerPosition{ Face::Left, 3 }]) == (StickerPosition{ Face::Left, 1 }));
  CHECK((cube[StickerPosition{ Face::Right, 0 }]) == (StickerPosition{ Face::Right, 0 }));
  CHECK((cube[StickerPosition{ Face::Right, 1 }]) == (StickerPosition{ Face::Right, 1 }));
  CHECK((cube[StickerPosition{ Face::Right, 2 }]) == (StickerPosition{ Face::Right, 2 }));
  CHECK((cube[StickerPosition{ Face::Right, 3 }]) == (StickerPosition{ Face::Right, 3 }));
}

TEST_CASE("2x2x2 - Ensure that we can apply a move: D2", "[Cube2x2x2]")
{
  Cube2x2x2 cube;
  cube += CubeMove{ Face::Down, 2 };
  CHECK((cube[StickerPosition{ Face::Down, 0 }]) == (StickerPosition{ Face::Down, 3 }));
  CHECK((cube[StickerPosition{ Face::Down, 1 }]) == (StickerPosition{ Face::Down, 2 }));
  CHECK((cube[StickerPosition{ Face::Down, 2 }]) == (StickerPosition{ Face::Down, 1 }));
  CHECK((cube[StickerPosition{ Face::Down, 3 }]) == (StickerPosition{ Face::Down, 0 }));
  CHECK((cube[StickerPosition{ Face::Up, 0 }]) == (StickerPosition{ Face::Up, 0 }));
  CHECK((cube[StickerPosition{ Face::Up, 1 }]) == (StickerPosition{ Face::Up, 1 }));
  CHECK((cube[StickerPosition{ Face::Up, 2 }]) == (StickerPosition{ Face::Up, 2 }));
  CHECK((cube[StickerPosition{ Face::Up, 3 }]) == (StickerPosition{ Face::Up, 3 }));
}

TEST_CASE("2x2x2 - Ensure that we can apply a move: B2", "[Cube2x2x2]")
{
  Cube2x2x2 cube;
  cube += CubeMove{ Face::Back, 2 };
  CHECK((cube[StickerPosition{ Face::Back, 0 }]) == (StickerPosition{ Face::Back, 3 }));
  CHECK((cube[StickerPosition{ Face::Back, 1 }]) == (StickerPosition{ Face::Back, 2 }));
  CHECK((cube[StickerPosition{ Face::Back, 2 }]) == (StickerPosition{ Face::Back, 1 }));
  CHECK((cube[StickerPosition{ Face::Back, 3 }]) == (StickerPosition{ Face::Back, 0 }));
  CHECK((cube[StickerPosition{ Face::Front, 0 }]) == (StickerPosition{ Face::Front, 0 }));
  CHECK((cube[StickerPosition{ Face::Front, 1 }]) == (StickerPosition{ Face::Front, 1 }));
  CHECK((cube[StickerPosition{ Face::Front, 2 }]) == (StickerPosition{ Face::Front, 2 }));
  CHECK((cube[StickerPosition{ Face::Front, 3 }]) == (StickerPosition{ Face::Front, 3 }));
}

TEST_CASE("2x2x2 - Ensure that we can apply a move: F'", "[Cube2x2x2]")
{
  Cube2x2x2 cube;
  cube += CubeMove{ Face::Front, -1 };
  CHECK((cube[StickerPosition{ Face::Front, 0 }]) == (StickerPosition{ Face::Front, 1 }));
  CHECK((cube[StickerPosition{ Face::Front, 1 }]) == (StickerPosition{ Face::Front, 3 }));
  CHECK((cube[StickerPosition{ Face::Front, 2 }]) == (StickerPosition{ Face::Front, 0 }));
  CHECK((cube[StickerPosition{ Face::Front, 3 }]) == (StickerPosition{ Face::Front, 2 }));
  CHECK((cube[StickerPosition{ Face::Back, 0 }]) == (StickerPosition{ Face::Back, 0 }));
  CHECK((cube[StickerPosition{ Face::Back, 1 }]) == (StickerPosition{ Face::Back, 1 }));
  CHECK((cube[StickerPosition{ Face::Back, 2 }]) == (StickerPosition{ Face::Back, 2 }));
  CHECK((cube[StickerPosition{ Face::Back, 3 }]) == (StickerPosition{ Face::Back, 3 }));
}

TEST_CASE("2x2x2 - Ensure that we can apply a move: R'", "[Cube2x2x2]")
{
  Cube2x2x2 cube;
  cube += CubeMove{ Face::Right, -1 };
  CHECK((cube[StickerPosition{ Face::Right, 0 }]) == (StickerPosition{ Face::Right, 1 }));
  CHECK((cube[StickerPosition{ Face::Right, 1 }]) == (StickerPosition{ Face::Right, 3 }));
  CHECK((cube[StickerPosition{ Face::Right, 2 }]) == (StickerPosition{ Face::Right, 0 }));
  CHECK((cube[StickerPosition{ Face::Right, 3 }]) == (StickerPosition{ Face::Right, 2 }));
  CHECK((cube[StickerPosition{ Face::Left, 0 }]) == (StickerPosition{ Face::Left, 0 }));
  CHECK((cube[StickerPosition{ Face::Left, 1 }]) == (StickerPosition{ Face::Left, 1 }));
  CHECK((cube[StickerPosition{ Face::Left, 2 }]) == (StickerPosition{ Face::Left, 2 }));
  CHECK((cube[StickerPosition{ Face::Left, 3 }]) == (StickerPosition{ Face::Left, 3 }));
}

TEST_CASE("2x2x2 - Ensure we can apply an empty move sequence", "[Cube2x2x2]")
{
  Cube2x2x2 cube;
  cube += std::vector<CubeMove>{};
  CHECK((cube[StickerPosition{ Face::Up, 0 }]) == (StickerPosition{ Face::Up, 0 }));
  CHECK((cube[StickerPosition{ Face::Up, 1 }]) == (StickerPosition{ Face::Up, 1 }));
  CHECK((cube[StickerPosition{ Face::Up, 2 }]) == (StickerPosition{ Face::Up, 2 }));
  CHECK((cube[StickerPosition{ Face::Up, 3 }]) == (StickerPosition{ Face::Up, 3 }));
  CHECK((cube[StickerPosition{ Face::Down, 0 }]) == (StickerPosition{ Face::Down, 0 }));
  CHECK((cube[StickerPosition{ Face::Down, 1 }]) == (StickerPosition{ Face::Down, 1 }));
  CHECK((cube[StickerPosition{ Face::Down, 2 }]) == (StickerPosition{ Face::Down, 2 }));
  CHECK((cube[StickerPosition{ Face::Down, 3 }]) == (StickerPosition{ Face::Down, 3 }));
  CHECK((cube[StickerPosition{ Face::Front, 0 }]) == (StickerPosition{ Face::Front, 0 }));
  CHECK((cube[StickerPosition{ Face::Front, 1 }]) == (StickerPosition{ Face::Front, 1 }));
  CHECK((cube[StickerPosition{ Face::Front, 2 }]) == (StickerPosition{ Face::Front, 2 }));
  CHECK((cube[StickerPosition{ Face::Front, 3 }]) == (StickerPosition{ Face::Front, 3 }));
  CHECK((cube[StickerPosition{ Face::Back, 0 }]) == (StickerPosition{ Face::Back, 0 }));
  CHECK((cube[StickerPosition{ Face::Back, 1 }]) == (StickerPosition{ Face::Back, 1 }));
  CHECK((cube[StickerPosition{ Face::Back, 2 }]) == (StickerPosition{ Face::Back, 2 }));
  CHECK((cube[StickerPosition{ Face::Back, 3 }]) == (StickerPosition{ Face::Back, 3 }));
  CHECK((cube[StickerPosition{ Face::Left, 0 }]) == (StickerPosition{ Face::Left, 0 }));
  CHECK((cube[StickerPosition{ Face::Left, 1 }]) == (StickerPosition{ Face::Left, 1 }));
  CHECK((cube[StickerPosition{ Face::Left, 2 }]) == (StickerPosition{ Face::Left, 2 }));
  CHECK((cube[StickerPosition{ Face::Left, 3 }]) == (StickerPosition{ Face::Left, 3 }));
  CHECK((cube[StickerPosition{ Face::Right, 0 }]) == (StickerPosition{ Face::Right, 0 }));
  CHECK((cube[StickerPosition{ Face::Right, 1 }]) == (StickerPosition{ Face::Right, 1 }));
  CHECK((cube[StickerPosition{ Face::Right, 2 }]) == (StickerPosition{ Face::Right, 2 }));
  CHECK((cube[StickerPosition{ Face::Right, 3 }]) == (StickerPosition{ Face::Right, 3 }));
}

TEST_CASE("2x2x2 - Ensure we can apply a move sequence", "[Cube2x2x2]")
{
  Cube2x2x2 cube;
  cube += std::vector<CubeMove>
  { 
    CubeMove{ Face::Left, 1 },
    CubeMove{ Face::Back, 2 },
    CubeMove{ Face::Right, -1 }
  };
  CHECK((cube[StickerPosition{ Face::Up, 0 }]) == (StickerPosition{ Face::Down, 3 }));
  CHECK((cube[StickerPosition{ Face::Up, 1 }]) == (StickerPosition{ Face::Down, 2 }));
  CHECK((cube[StickerPosition{ Face::Up, 2 }]) == (StickerPosition{ Face::Back, 1 }));
  CHECK((cube[StickerPosition{ Face::Up, 3 }]) == (StickerPosition{ Face::Down, 0 }));
  CHECK((cube[StickerPosition{ Face::Down, 0 }]) == (StickerPosition{ Face::Front, 0 }));
  CHECK((cube[StickerPosition{ Face::Down, 1 }]) == (StickerPosition{ Face::Front, 1 }));
  CHECK((cube[StickerPosition{ Face::Down, 2 }]) == (StickerPosition{ Face::Up, 1 }));
  CHECK((cube[StickerPosition{ Face::Down, 3 }]) == (StickerPosition{ Face::Front, 3 }));
  CHECK((cube[StickerPosition{ Face::Front, 0 }]) == (StickerPosition{ Face::Up, 0 }));
  CHECK((cube[StickerPosition{ Face::Front, 1 }]) == (StickerPosition{ Face::Front, 2 }));
  CHECK((cube[StickerPosition{ Face::Front, 2 }]) == (StickerPosition{ Face::Up, 2 }));
  CHECK((cube[StickerPosition{ Face::Front, 3 }]) == (StickerPosition{ Face::Up, 3 }));
  CHECK((cube[StickerPosition{ Face::Back, 0 }]) == (StickerPosition{ Face::Back, 3 }));
  CHECK((cube[StickerPosition{ Face::Back, 1 }]) == (StickerPosition{ Face::Back, 2 }));
  CHECK((cube[StickerPosition{ Face::Back, 2 }]) == (StickerPosition{ Face::Down, 1 }));
  CHECK((cube[StickerPosition{ Face::Back, 3 }]) == (StickerPosition{ Face::Back, 0 }));
  CHECK((cube[StickerPosition{ Face::Left, 0 }]) == (StickerPosition{ Face::Right, 3 }));
  CHECK((cube[StickerPosition{ Face::Left, 1 }]) == (StickerPosition{ Face::Left, 0 }));
  CHECK((cube[StickerPosition{ Face::Left, 2 }]) == (StickerPosition{ Face::Right, 1 }));
  CHECK((cube[StickerPosition{ Face::Left, 3 }]) == (StickerPosition{ Face::Left, 1 }));
  CHECK((cube[StickerPosition{ Face::Right, 0 }]) == (StickerPosition{ Face::Left, 3 }));
  CHECK((cube[StickerPosition{ Face::Right, 1 }]) == (StickerPosition{ Face::Left, 2 }));
  CHECK((cube[StickerPosition{ Face::Right, 2 }]) == (StickerPosition{ Face::Right, 0 }));
  CHECK((cube[StickerPosition{ Face::Right, 3 }]) == (StickerPosition{ Face::Right, 2 }));
}

TEST_CASE("2x2x2 - Ensure we can apply an empty move sequence string", "[Cube2x2x2]")
{
  Cube2x2x2 cube;
  cube += "";
  CHECK((cube[StickerPosition{ Face::Up, 0 }]) == (StickerPosition{ Face::Up, 0 }));
  CHECK((cube[StickerPosition{ Face::Up, 1 }]) == (StickerPosition{ Face::Up, 1 }));
  CHECK((cube[StickerPosition{ Face::Up, 2 }]) == (StickerPosition{ Face::Up, 2 }));
  CHECK((cube[StickerPosition{ Face::Up, 3 }]) == (StickerPosition{ Face::Up, 3 }));
  CHECK((cube[StickerPosition{ Face::Down, 0 }]) == (StickerPosition{ Face::Down, 0 }));
  CHECK((cube[StickerPosition{ Face::Down, 1 }]) == (StickerPosition{ Face::Down, 1 }));
  CHECK((cube[StickerPosition{ Face::Down, 2 }]) == (StickerPosition{ Face::Down, 2 }));
  CHECK((cube[StickerPosition{ Face::Down, 3 }]) == (StickerPosition{ Face::Down, 3 }));
  CHECK((cube[StickerPosition{ Face::Front, 0 }]) == (StickerPosition{ Face::Front, 0 }));
  CHECK((cube[StickerPosition{ Face::Front, 1 }]) == (StickerPosition{ Face::Front, 1 }));
  CHECK((cube[StickerPosition{ Face::Front, 2 }]) == (StickerPosition{ Face::Front, 2 }));
  CHECK((cube[StickerPosition{ Face::Front, 3 }]) == (StickerPosition{ Face::Front, 3 }));
  CHECK((cube[StickerPosition{ Face::Back, 0 }]) == (StickerPosition{ Face::Back, 0 }));
  CHECK((cube[StickerPosition{ Face::Back, 1 }]) == (StickerPosition{ Face::Back, 1 }));
  CHECK((cube[StickerPosition{ Face::Back, 2 }]) == (StickerPosition{ Face::Back, 2 }));
  CHECK((cube[StickerPosition{ Face::Back, 3 }]) == (StickerPosition{ Face::Back, 3 }));
  CHECK((cube[StickerPosition{ Face::Left, 0 }]) == (StickerPosition{ Face::Left, 0 }));
  CHECK((cube[StickerPosition{ Face::Left, 1 }]) == (StickerPosition{ Face::Left, 1 }));
  CHECK((cube[StickerPosition{ Face::Left, 2 }]) == (StickerPosition{ Face::Left, 2 }));
  CHECK((cube[StickerPosition{ Face::Left, 3 }]) == (StickerPosition{ Face::Left, 3 }));
  CHECK((cube[StickerPosition{ Face::Right, 0 }]) == (StickerPosition{ Face::Right, 0 }));
  CHECK((cube[StickerPosition{ Face::Right, 1 }]) == (StickerPosition{ Face::Right, 1 }));
  CHECK((cube[StickerPosition{ Face::Right, 2 }]) == (StickerPosition{ Face::Right, 2 }));
  CHECK((cube[StickerPosition{ Face::Right, 3 }]) == (StickerPosition{ Face::Right, 3 }));
}

TEST_CASE("2x2x2 - Ensure we can apply a move sequence string", "[Cube2x2x2]")
{
  Cube2x2x2 cube;
  cube += "L B2 R'";
  CHECK((cube[StickerPosition{ Face::Up, 0 }]) == (StickerPosition{ Face::Down, 3 }));
  CHECK((cube[StickerPosition{ Face::Up, 1 }]) == (StickerPosition{ Face::Down, 2 }));
  CHECK((cube[StickerPosition{ Face::Up, 2 }]) == (StickerPosition{ Face::Back, 1 }));
  CHECK((cube[StickerPosition{ Face::Up, 3 }]) == (StickerPosition{ Face::Down, 0 }));
  CHECK((cube[StickerPosition{ Face::Down, 0 }]) == (StickerPosition{ Face::Front, 0 }));
  CHECK((cube[StickerPosition{ Face::Down, 1 }]) == (StickerPosition{ Face::Front, 1 }));
  CHECK((cube[StickerPosition{ Face::Down, 2 }]) == (StickerPosition{ Face::Up, 1 }));
  CHECK((cube[StickerPosition{ Face::Down, 3 }]) == (StickerPosition{ Face::Front, 3 }));
  CHECK((cube[StickerPosition{ Face::Front, 0 }]) == (StickerPosition{ Face::Up, 0 }));
  CHECK((cube[StickerPosition{ Face::Front, 1 }]) == (StickerPosition{ Face::Front, 2 }));
  CHECK((cube[StickerPosition{ Face::Front, 2 }]) == (StickerPosition{ Face::Up, 2 }));
  CHECK((cube[StickerPosition{ Face::Front, 3 }]) == (StickerPosition{ Face::Up, 3 }));
  CHECK((cube[StickerPosition{ Face::Back, 0 }]) == (StickerPosition{ Face::Back, 3 }));
  CHECK((cube[StickerPosition{ Face::Back, 1 }]) == (StickerPosition{ Face::Back, 2 }));
  CHECK((cube[StickerPosition{ Face::Back, 2 }]) == (StickerPosition{ Face::Down, 1 }));
  CHECK((cube[StickerPosition{ Face::Back, 3 }]) == (StickerPosition{ Face::Back, 0 }));
  CHECK((cube[StickerPosition{ Face::Left, 0 }]) == (StickerPosition{ Face::Right, 3 }));
  CHECK((cube[StickerPosition{ Face::Left, 1 }]) == (StickerPosition{ Face::Left, 0 }));
  CHECK((cube[StickerPosition{ Face::Left, 2 }]) == (StickerPosition{ Face::Right, 1 }));
  CHECK((cube[StickerPosition{ Face::Left, 3 }]) == (StickerPosition{ Face::Left, 1 }));
  CHECK((cube[StickerPosition{ Face::Right, 0 }]) == (StickerPosition{ Face::Left, 3 }));
  CHECK((cube[StickerPosition{ Face::Right, 1 }]) == (StickerPosition{ Face::Left, 2 }));
  CHECK((cube[StickerPosition{ Face::Right, 2 }]) == (StickerPosition{ Face::Right, 0 }));
  CHECK((cube[StickerPosition{ Face::Right, 3 }]) == (StickerPosition{ Face::Right, 2 }));
}

TEST_CASE("2x2x2 - Ensure the default cube is solved", "[Cube2x2x2]")
{
  Cube2x2x2 cube;
  CHECK(cube.IsSolved());
}

TEST_CASE("2x2x2 - Ensure the rotated cubes are solved", "[Cube2x2x2]")
{
  Cube2x2x2 cubeX;
  cubeX += "R L'";
  CHECK(cubeX.IsSolved());
  Cube2x2x2 cubeX2;
  cubeX2 += "R2 L2";
  CHECK(cubeX2.IsSolved());
  Cube2x2x2 cubeXPrime;
  cubeXPrime += "R' L";
  CHECK(cubeXPrime.IsSolved());
  Cube2x2x2 cubeY;
  cubeY += "U D'";
  CHECK(cubeY.IsSolved());
  Cube2x2x2 cubeY2;
  cubeY2 += "U2 D2";
  CHECK(cubeY2.IsSolved());
  Cube2x2x2 cubeYPrime;
  cubeYPrime += "U' D";
  CHECK(cubeYPrime.IsSolved());
  Cube2x2x2 cubeZ;
  cubeZ += "F B'";
  CHECK(cubeZ.IsSolved());
  Cube2x2x2 cubeZ2;
  cubeZ2 += "F2 B2";
  CHECK(cubeZ2.IsSolved());
  Cube2x2x2 cubeZPrime;
  cubeZPrime += "F' B";
  CHECK(cubeZPrime.IsSolved());
}

TEST_CASE("2x2x2 - Ensure that scrambled cubes are not solved", "[Cube2x2x2]")
{
  Cube2x2x2 cube;
  CHECK(!(cube += "F").IsSolved());
  CHECK(!(cube += "U'").IsSolved());
  CHECK(!(cube += "R2").IsSolved());
  CHECK(!(cube += "B2").IsSolved());
  CHECK(!(cube += "L'").IsSolved());
  CHECK(!(cube += "D").IsSolved());
  CHECK((cube += "D' L B2 R2 U F'").IsSolved());
}

TEST_CASE("2x2x2 - Ensure that a default face remap on a solved cube leaves the cube unchanged", "[Cube2x2x2]")
{
  Cube2x2x2 cube;
  cube.RemapFaces(Face::Front, Face::Up);
  CHECK(cube.IsSolved());
  CHECK(cube.GetKey() == Cube2x2x2{}.GetKey());
}

TEST_CASE("2x2x2 - Ensure that a face remap on a solved cube leaves the cube solved", "[Cube2x2x2]")
{
  Cube2x2x2 cube;
  cube.RemapFaces(Face::Left, Face::Down);
  CHECK(cube.IsSolved());
  CHECK(cube.GetKey() != Cube2x2x2{}.GetKey());
}

TEST_CASE("2x2x2 - Ensure that a face remap can swap front and up faces", "[Cube2x2x2]")
{
  Cube2x2x2 cube;
  cube.RemapFaces(Face::Up, Face::Front);
  CHECK(cube.IsSolved());
  CHECK(cube.GetKey() != Cube2x2x2{}.GetKey());
  cube += "F2 B2 R L'";
  CHECK(cube.GetKey() == Cube2x2x2{}.GetKey());
}

TEST_CASE("2x2x2 - Ensure that a default face remap on a scrambled cube leaves the cube unchanged", "[Cube2x2x2]")
{
  Cube2x2x2 cube;
  cube += "F R2 U' B";
  Cube2x2x2 original = cube;
  cube.RemapFaces(Face::Front, Face::Up);
  CHECK(!cube.IsSolved());
  CHECK(cube.GetKey() == original.GetKey());
}

TEST_CASE("2x2x2 - Ensure that a face remap on a scrambled cube leaves the cube solvable with the inverse scramble", "[Cube2x2x2]")
{
  Cube2x2x2 cube;
  std::string scramble = "F R2 U' B";
  cube += scramble;
  Cube2x2x2 original = cube;
  cube.RemapFaces(Face::Left, Face::Down);
  CHECK(!cube.IsSolved());
  CHECK(cube.GetKey() != original.GetKey());
  cube += InvertMoveSequence(ParseMoveSequence(scramble));
  CHECK(cube.IsSolved());
  CHECK(cube.GetKey() != Cube2x2x2{}.GetKey());
}

TEST_CASE("2x2x2 - Ensure that two face remaps that are the opposite of each other leave the cube unchanged", "[Cube2x2x2]")
{
  Cube2x2x2 cube;
  cube += "F R2 U' B";
  Cube2x2x2 original = cube;
  cube.RemapFaces(Face::Up, Face::Front);
  cube.RemapFaces(Face::Up, Face::Front);
  CHECK(!cube.IsSolved());
  CHECK(cube.GetKey() == original.GetKey());
}

TEST_CASE("2x2x2 - Ensure that a face remap with two of the same face is not allowed", "[Cube2x2x2]")
{
  Cube2x2x2 cube;
  CHECK_THROWS(cube.RemapFaces(Face::Left, Face::Left));
}

TEST_CASE("2x2x2 - Ensure that a face remap with two opposite faces is not allowed", "[Cube2x2x2]")
{
  Cube2x2x2 cube;
  CHECK_THROWS(cube.RemapFaces(Face::Up, Face::Down));
}