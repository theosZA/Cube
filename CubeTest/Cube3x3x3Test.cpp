#include "catch.hpp"

#include "3x3x3\Cube3x3x3.h"

TEST_CASE("3x3x3 - Ensure that the default position is correct", "[Cube3x3x3]")
{
  Cube3x3x3 cube;
  for (size_t face = 0; face < 6; ++face)
    for (size_t index = 0; index < 9; ++index)
      CHECK((cube[StickerPosition{ static_cast<Face>(face), index }]) == (StickerPosition{ static_cast<Face>(face), index }));
}

TEST_CASE("3x3x3 - Ensure that we find the cubies in the correct place in the default position", "[Cube3x3x3]")
{
  Cube3x3x3 cube;
  for (size_t face = 0; face < 6; ++face)
    for (size_t index = 0; index < 9; ++index)
      CHECK(cube.Find(StickerPosition{ static_cast<Face>(face), index }) == (StickerPosition{ static_cast<Face>(face), index }));
}

TEST_CASE("3x3x3 - Ensure that we can apply a move: U", "[Cube3x3x3]")
{
  Cube3x3x3 cube;
  cube += CubeMove{ Face::Up, 1 };
  CHECK((cube[StickerPosition{ Face::Up, 0 }]) == (StickerPosition{ Face::Up, 6 }));
  CHECK((cube[StickerPosition{ Face::Up, 1 }]) == (StickerPosition{ Face::Up, 3 }));
  CHECK((cube[StickerPosition{ Face::Up, 2 }]) == (StickerPosition{ Face::Up, 0 }));
  CHECK((cube[StickerPosition{ Face::Up, 3 }]) == (StickerPosition{ Face::Up, 7 }));
  CHECK((cube[StickerPosition{ Face::Up, 4 }]) == (StickerPosition{ Face::Up, 4 }));
  CHECK((cube[StickerPosition{ Face::Up, 5 }]) == (StickerPosition{ Face::Up, 1 }));
  CHECK((cube[StickerPosition{ Face::Up, 6 }]) == (StickerPosition{ Face::Up, 8 }));
  CHECK((cube[StickerPosition{ Face::Up, 7 }]) == (StickerPosition{ Face::Up, 5 }));
  CHECK((cube[StickerPosition{ Face::Up, 8 }]) == (StickerPosition{ Face::Up, 2 }));
  CHECK((cube[StickerPosition{ Face::Down, 0 }]) == (StickerPosition{ Face::Down, 0 }));
  CHECK((cube[StickerPosition{ Face::Down, 1 }]) == (StickerPosition{ Face::Down, 1 }));
  CHECK((cube[StickerPosition{ Face::Down, 2 }]) == (StickerPosition{ Face::Down, 2 }));
  CHECK((cube[StickerPosition{ Face::Down, 3 }]) == (StickerPosition{ Face::Down, 3 }));
  CHECK((cube[StickerPosition{ Face::Down, 4 }]) == (StickerPosition{ Face::Down, 4 }));
  CHECK((cube[StickerPosition{ Face::Down, 5 }]) == (StickerPosition{ Face::Down, 5 }));
  CHECK((cube[StickerPosition{ Face::Down, 6 }]) == (StickerPosition{ Face::Down, 6 }));
  CHECK((cube[StickerPosition{ Face::Down, 7 }]) == (StickerPosition{ Face::Down, 7 }));
  CHECK((cube[StickerPosition{ Face::Down, 8 }]) == (StickerPosition{ Face::Down, 8 }));
  CHECK((cube[StickerPosition{ Face::Front, 3 }]) == (StickerPosition{ Face::Front, 3 }));
  CHECK((cube[StickerPosition{ Face::Front, 4 }]) == (StickerPosition{ Face::Front, 4 }));
  CHECK((cube[StickerPosition{ Face::Front, 5 }]) == (StickerPosition{ Face::Front, 5 }));
  CHECK((cube[StickerPosition{ Face::Back, 3 }]) == (StickerPosition{ Face::Back, 3 }));
  CHECK((cube[StickerPosition{ Face::Back, 4 }]) == (StickerPosition{ Face::Back, 4 }));
  CHECK((cube[StickerPosition{ Face::Back, 5 }]) == (StickerPosition{ Face::Back, 5 }));
}

TEST_CASE("3x3x3 - Ensure that we can apply a move: L", "[Cube3x3x3]")
{
  Cube3x3x3 cube;
  cube += CubeMove{ Face::Left, 1 };
  CHECK((cube[StickerPosition{ Face::Left, 0 }]) == (StickerPosition{ Face::Left, 6 }));
  CHECK((cube[StickerPosition{ Face::Left, 1 }]) == (StickerPosition{ Face::Left, 3 }));
  CHECK((cube[StickerPosition{ Face::Left, 2 }]) == (StickerPosition{ Face::Left, 0 }));
  CHECK((cube[StickerPosition{ Face::Left, 3 }]) == (StickerPosition{ Face::Left, 7 }));
  CHECK((cube[StickerPosition{ Face::Left, 4 }]) == (StickerPosition{ Face::Left, 4 }));
  CHECK((cube[StickerPosition{ Face::Left, 5 }]) == (StickerPosition{ Face::Left, 1 }));
  CHECK((cube[StickerPosition{ Face::Left, 6 }]) == (StickerPosition{ Face::Left, 8 }));
  CHECK((cube[StickerPosition{ Face::Left, 7 }]) == (StickerPosition{ Face::Left, 5 }));
  CHECK((cube[StickerPosition{ Face::Left, 8 }]) == (StickerPosition{ Face::Left, 2 }));
  CHECK((cube[StickerPosition{ Face::Right, 0 }]) == (StickerPosition{ Face::Right, 0 }));
  CHECK((cube[StickerPosition{ Face::Right, 1 }]) == (StickerPosition{ Face::Right, 1 }));
  CHECK((cube[StickerPosition{ Face::Right, 2 }]) == (StickerPosition{ Face::Right, 2 }));
  CHECK((cube[StickerPosition{ Face::Right, 3 }]) == (StickerPosition{ Face::Right, 3 }));
  CHECK((cube[StickerPosition{ Face::Right, 4 }]) == (StickerPosition{ Face::Right, 4 }));
  CHECK((cube[StickerPosition{ Face::Right, 5 }]) == (StickerPosition{ Face::Right, 5 }));
  CHECK((cube[StickerPosition{ Face::Right, 6 }]) == (StickerPosition{ Face::Right, 6 }));
  CHECK((cube[StickerPosition{ Face::Right, 7 }]) == (StickerPosition{ Face::Right, 7 }));
  CHECK((cube[StickerPosition{ Face::Right, 8 }]) == (StickerPosition{ Face::Right, 8 }));
  CHECK((cube[StickerPosition{ Face::Front, 1 }]) == (StickerPosition{ Face::Front, 1 }));
  CHECK((cube[StickerPosition{ Face::Front, 4 }]) == (StickerPosition{ Face::Front, 4 }));
  CHECK((cube[StickerPosition{ Face::Front, 7 }]) == (StickerPosition{ Face::Front, 7 }));
  CHECK((cube[StickerPosition{ Face::Back, 1 }]) == (StickerPosition{ Face::Back, 1 }));
  CHECK((cube[StickerPosition{ Face::Back, 4 }]) == (StickerPosition{ Face::Back, 4 }));
  CHECK((cube[StickerPosition{ Face::Back, 7 }]) == (StickerPosition{ Face::Back, 7 }));
}

TEST_CASE("3x3x3 - Ensure that we can apply a move: D2", "[Cube3x3x3]")
{
  Cube3x3x3 cube;
  cube += CubeMove{ Face::Down, 2 };
  CHECK((cube[StickerPosition{ Face::Down, 0 }]) == (StickerPosition{ Face::Down, 8 }));
  CHECK((cube[StickerPosition{ Face::Down, 1 }]) == (StickerPosition{ Face::Down, 7 }));
  CHECK((cube[StickerPosition{ Face::Down, 2 }]) == (StickerPosition{ Face::Down, 6 }));
  CHECK((cube[StickerPosition{ Face::Down, 3 }]) == (StickerPosition{ Face::Down, 5 }));
  CHECK((cube[StickerPosition{ Face::Down, 4 }]) == (StickerPosition{ Face::Down, 4 }));
  CHECK((cube[StickerPosition{ Face::Down, 5 }]) == (StickerPosition{ Face::Down, 3 }));
  CHECK((cube[StickerPosition{ Face::Down, 6 }]) == (StickerPosition{ Face::Down, 2 }));
  CHECK((cube[StickerPosition{ Face::Down, 7 }]) == (StickerPosition{ Face::Down, 1 }));
  CHECK((cube[StickerPosition{ Face::Down, 8 }]) == (StickerPosition{ Face::Down, 0 }));
  CHECK((cube[StickerPosition{ Face::Up, 0 }]) == (StickerPosition{ Face::Up, 0 }));
  CHECK((cube[StickerPosition{ Face::Up, 1 }]) == (StickerPosition{ Face::Up, 1 }));
  CHECK((cube[StickerPosition{ Face::Up, 2 }]) == (StickerPosition{ Face::Up, 2 }));
  CHECK((cube[StickerPosition{ Face::Up, 3 }]) == (StickerPosition{ Face::Up, 3 }));
  CHECK((cube[StickerPosition{ Face::Up, 4 }]) == (StickerPosition{ Face::Up, 4 }));
  CHECK((cube[StickerPosition{ Face::Up, 5 }]) == (StickerPosition{ Face::Up, 5 }));
  CHECK((cube[StickerPosition{ Face::Up, 6 }]) == (StickerPosition{ Face::Up, 6 }));
  CHECK((cube[StickerPosition{ Face::Up, 7 }]) == (StickerPosition{ Face::Up, 7 }));
  CHECK((cube[StickerPosition{ Face::Up, 8 }]) == (StickerPosition{ Face::Up, 8 }));
  CHECK((cube[StickerPosition{ Face::Front, 3 }]) == (StickerPosition{ Face::Front, 3 }));
  CHECK((cube[StickerPosition{ Face::Front, 4 }]) == (StickerPosition{ Face::Front, 4 }));
  CHECK((cube[StickerPosition{ Face::Front, 5 }]) == (StickerPosition{ Face::Front, 5 }));
  CHECK((cube[StickerPosition{ Face::Back, 3 }]) == (StickerPosition{ Face::Back, 3 }));
  CHECK((cube[StickerPosition{ Face::Back, 4 }]) == (StickerPosition{ Face::Back, 4 }));
  CHECK((cube[StickerPosition{ Face::Back, 5 }]) == (StickerPosition{ Face::Back, 5 }));
}

TEST_CASE("3x3x3 - Ensure that we can apply a move: B2", "[Cube3x3x3]")
{
  Cube3x3x3 cube;
  cube += CubeMove{ Face::Back, 2 };
  CHECK((cube[StickerPosition{ Face::Back, 0 }]) == (StickerPosition{ Face::Back, 8 }));
  CHECK((cube[StickerPosition{ Face::Back, 1 }]) == (StickerPosition{ Face::Back, 7 }));
  CHECK((cube[StickerPosition{ Face::Back, 2 }]) == (StickerPosition{ Face::Back, 6 }));
  CHECK((cube[StickerPosition{ Face::Back, 3 }]) == (StickerPosition{ Face::Back, 5 }));
  CHECK((cube[StickerPosition{ Face::Back, 4 }]) == (StickerPosition{ Face::Back, 4 }));
  CHECK((cube[StickerPosition{ Face::Back, 5 }]) == (StickerPosition{ Face::Back, 3 }));
  CHECK((cube[StickerPosition{ Face::Back, 6 }]) == (StickerPosition{ Face::Back, 2 }));
  CHECK((cube[StickerPosition{ Face::Back, 7 }]) == (StickerPosition{ Face::Back, 1 }));
  CHECK((cube[StickerPosition{ Face::Back, 8 }]) == (StickerPosition{ Face::Back, 0 }));
  CHECK((cube[StickerPosition{ Face::Front, 0 }]) == (StickerPosition{ Face::Front, 0 }));
  CHECK((cube[StickerPosition{ Face::Front, 1 }]) == (StickerPosition{ Face::Front, 1 }));
  CHECK((cube[StickerPosition{ Face::Front, 2 }]) == (StickerPosition{ Face::Front, 2 }));
  CHECK((cube[StickerPosition{ Face::Front, 3 }]) == (StickerPosition{ Face::Front, 3 }));
  CHECK((cube[StickerPosition{ Face::Front, 4 }]) == (StickerPosition{ Face::Front, 4 }));
  CHECK((cube[StickerPosition{ Face::Front, 5 }]) == (StickerPosition{ Face::Front, 5 }));
  CHECK((cube[StickerPosition{ Face::Front, 6 }]) == (StickerPosition{ Face::Front, 6 }));
  CHECK((cube[StickerPosition{ Face::Front, 7 }]) == (StickerPosition{ Face::Front, 7 }));
  CHECK((cube[StickerPosition{ Face::Front, 8 }]) == (StickerPosition{ Face::Front, 8 }));
  CHECK((cube[StickerPosition{ Face::Up, 3 }]) == (StickerPosition{ Face::Up, 3 }));
  CHECK((cube[StickerPosition{ Face::Up, 4 }]) == (StickerPosition{ Face::Up, 4 }));
  CHECK((cube[StickerPosition{ Face::Up, 5 }]) == (StickerPosition{ Face::Up, 5 }));
  CHECK((cube[StickerPosition{ Face::Down, 3 }]) == (StickerPosition{ Face::Down, 3 }));
  CHECK((cube[StickerPosition{ Face::Down, 4 }]) == (StickerPosition{ Face::Down, 4 }));
  CHECK((cube[StickerPosition{ Face::Down, 5 }]) == (StickerPosition{ Face::Down, 5 }));
}

TEST_CASE("3x3x3 - Ensure that we can apply a move: F'", "[Cube3x3x3]")
{
  Cube3x3x3 cube;
  cube += CubeMove{ Face::Front, -1 };
  CHECK((cube[StickerPosition{ Face::Front, 0 }]) == (StickerPosition{ Face::Front, 2 }));
  CHECK((cube[StickerPosition{ Face::Front, 1 }]) == (StickerPosition{ Face::Front, 5 }));
  CHECK((cube[StickerPosition{ Face::Front, 2 }]) == (StickerPosition{ Face::Front, 8 }));
  CHECK((cube[StickerPosition{ Face::Front, 3 }]) == (StickerPosition{ Face::Front, 1 }));
  CHECK((cube[StickerPosition{ Face::Front, 4 }]) == (StickerPosition{ Face::Front, 4 }));
  CHECK((cube[StickerPosition{ Face::Front, 5 }]) == (StickerPosition{ Face::Front, 7 }));
  CHECK((cube[StickerPosition{ Face::Front, 6 }]) == (StickerPosition{ Face::Front, 0 }));
  CHECK((cube[StickerPosition{ Face::Front, 7 }]) == (StickerPosition{ Face::Front, 3 }));
  CHECK((cube[StickerPosition{ Face::Front, 8 }]) == (StickerPosition{ Face::Front, 6 }));
  CHECK((cube[StickerPosition{ Face::Back, 0 }]) == (StickerPosition{ Face::Back, 0 }));
  CHECK((cube[StickerPosition{ Face::Back, 1 }]) == (StickerPosition{ Face::Back, 1 }));
  CHECK((cube[StickerPosition{ Face::Back, 2 }]) == (StickerPosition{ Face::Back, 2 }));
  CHECK((cube[StickerPosition{ Face::Back, 3 }]) == (StickerPosition{ Face::Back, 3 }));
  CHECK((cube[StickerPosition{ Face::Back, 4 }]) == (StickerPosition{ Face::Back, 4 }));
  CHECK((cube[StickerPosition{ Face::Back, 5 }]) == (StickerPosition{ Face::Back, 5 }));
  CHECK((cube[StickerPosition{ Face::Back, 6 }]) == (StickerPosition{ Face::Back, 6 }));
  CHECK((cube[StickerPosition{ Face::Back, 7 }]) == (StickerPosition{ Face::Back, 7 }));
  CHECK((cube[StickerPosition{ Face::Back, 8 }]) == (StickerPosition{ Face::Back, 8 }));
  CHECK((cube[StickerPosition{ Face::Up, 3 }]) == (StickerPosition{ Face::Up, 3 }));
  CHECK((cube[StickerPosition{ Face::Up, 4 }]) == (StickerPosition{ Face::Up, 4 }));
  CHECK((cube[StickerPosition{ Face::Up, 5 }]) == (StickerPosition{ Face::Up, 5 }));
  CHECK((cube[StickerPosition{ Face::Down, 3 }]) == (StickerPosition{ Face::Down, 3 }));
  CHECK((cube[StickerPosition{ Face::Down, 4 }]) == (StickerPosition{ Face::Down, 4 }));
  CHECK((cube[StickerPosition{ Face::Down, 5 }]) == (StickerPosition{ Face::Down, 5 }));
}

TEST_CASE("3x3x3 - Ensure that we can apply a move: R'", "[Cube3x3x3]")
{
  Cube3x3x3 cube;
  cube += CubeMove{ Face::Right, -1 };
  CHECK((cube[StickerPosition{ Face::Right, 0 }]) == (StickerPosition{ Face::Right, 2 }));
  CHECK((cube[StickerPosition{ Face::Right, 1 }]) == (StickerPosition{ Face::Right, 5 }));
  CHECK((cube[StickerPosition{ Face::Right, 2 }]) == (StickerPosition{ Face::Right, 8 }));
  CHECK((cube[StickerPosition{ Face::Right, 3 }]) == (StickerPosition{ Face::Right, 1 }));
  CHECK((cube[StickerPosition{ Face::Right, 4 }]) == (StickerPosition{ Face::Right, 4 }));
  CHECK((cube[StickerPosition{ Face::Right, 5 }]) == (StickerPosition{ Face::Right, 7 }));
  CHECK((cube[StickerPosition{ Face::Right, 6 }]) == (StickerPosition{ Face::Right, 0 }));
  CHECK((cube[StickerPosition{ Face::Right, 7 }]) == (StickerPosition{ Face::Right, 3 }));
  CHECK((cube[StickerPosition{ Face::Right, 8 }]) == (StickerPosition{ Face::Right, 6 }));
  CHECK((cube[StickerPosition{ Face::Left, 0 }]) == (StickerPosition{ Face::Left, 0 }));
  CHECK((cube[StickerPosition{ Face::Left, 1 }]) == (StickerPosition{ Face::Left, 1 }));
  CHECK((cube[StickerPosition{ Face::Left, 2 }]) == (StickerPosition{ Face::Left, 2 }));
  CHECK((cube[StickerPosition{ Face::Left, 3 }]) == (StickerPosition{ Face::Left, 3 }));
  CHECK((cube[StickerPosition{ Face::Left, 4 }]) == (StickerPosition{ Face::Left, 4 }));
  CHECK((cube[StickerPosition{ Face::Left, 5 }]) == (StickerPosition{ Face::Left, 5 }));
  CHECK((cube[StickerPosition{ Face::Left, 6 }]) == (StickerPosition{ Face::Left, 6 }));
  CHECK((cube[StickerPosition{ Face::Left, 7 }]) == (StickerPosition{ Face::Left, 7 }));
  CHECK((cube[StickerPosition{ Face::Left, 8 }]) == (StickerPosition{ Face::Left, 8 }));
  CHECK((cube[StickerPosition{ Face::Up, 1 }]) == (StickerPosition{ Face::Up, 1 }));
  CHECK((cube[StickerPosition{ Face::Up, 4 }]) == (StickerPosition{ Face::Up, 4 }));
  CHECK((cube[StickerPosition{ Face::Up, 7 }]) == (StickerPosition{ Face::Up, 7 }));
  CHECK((cube[StickerPosition{ Face::Down, 1 }]) == (StickerPosition{ Face::Down, 1 }));
  CHECK((cube[StickerPosition{ Face::Down, 4 }]) == (StickerPosition{ Face::Down, 4 }));
  CHECK((cube[StickerPosition{ Face::Down, 7 }]) == (StickerPosition{ Face::Down, 7 }));
}

TEST_CASE("3x3x3 - Ensure each move and its inverse return to the default cube", "[Cube3x3x3]")
{
  for (size_t rotatingFace = 0; rotatingFace < 6; ++rotatingFace)
  {
    Cube3x3x3 cube;
    cube += CubeMove{ static_cast<Face>(rotatingFace), 1};
    cube += CubeMove{ static_cast<Face>(rotatingFace), -1 };
    for (size_t face = 0; face < 6; ++face)
      for (size_t index = 0; index < 9; ++index)
        CHECK((cube[StickerPosition{ static_cast<Face>(face), index }]) == (StickerPosition{ static_cast<Face>(face), index }));
  }
}

TEST_CASE("3x3x3 - Ensure we can apply an empty move sequence", "[Cube3x3x3]")
{
  Cube3x3x3 cube;
  cube += std::vector<CubeMove>{};
  for (size_t face = 0; face < 6; ++face)
    for (size_t index = 0; index < 9; ++index)
      CHECK((cube[StickerPosition{ static_cast<Face>(face), index }]) == (StickerPosition{ static_cast<Face>(face), index }));
}

TEST_CASE("3x3x3 - Ensure we can apply a move sequence", "[Cube3x3x3]")
{
  Cube3x3x3 cube;
  cube += std::vector<CubeMove>
  {
    CubeMove{ Face::Left, 1 },
    CubeMove{ Face::Back, 2 },
    CubeMove{ Face::Right, -1 }
  };
  CHECK((cube[StickerPosition{ Face::Up, 0 }]) == (StickerPosition{ Face::Down, 8 }));
  CHECK((cube[StickerPosition{ Face::Up, 1 }]) == (StickerPosition{ Face::Down, 7 }));
  CHECK((cube[StickerPosition{ Face::Up, 2 }]) == (StickerPosition{ Face::Down, 6 }));
  CHECK((cube[StickerPosition{ Face::Up, 3 }]) == (StickerPosition{ Face::Back, 5 }));
  CHECK((cube[StickerPosition{ Face::Up, 4 }]) == (StickerPosition{ Face::Up, 4 }));
  CHECK((cube[StickerPosition{ Face::Up, 5 }]) == (StickerPosition{ Face::Down, 3 }));
  CHECK((cube[StickerPosition{ Face::Up, 6 }]) == (StickerPosition{ Face::Back, 2 }));
  CHECK((cube[StickerPosition{ Face::Up, 7 }]) == (StickerPosition{ Face::Up, 7 }));
  CHECK((cube[StickerPosition{ Face::Up, 8 }]) == (StickerPosition{ Face::Down, 0 }));
  CHECK((cube[StickerPosition{ Face::Down, 0 }]) == (StickerPosition{ Face::Front, 0 }));
  CHECK((cube[StickerPosition{ Face::Down, 1 }]) == (StickerPosition{ Face::Down, 1 }));
  CHECK((cube[StickerPosition{ Face::Down, 2 }]) == (StickerPosition{ Face::Front, 2 }));
  CHECK((cube[StickerPosition{ Face::Down, 3 }]) == (StickerPosition{ Face::Front, 3 }));
  CHECK((cube[StickerPosition{ Face::Down, 4 }]) == (StickerPosition{ Face::Down, 4 }));
  CHECK((cube[StickerPosition{ Face::Down, 5 }]) == (StickerPosition{ Face::Front, 5 }));
  CHECK((cube[StickerPosition{ Face::Down, 6 }]) == (StickerPosition{ Face::Up, 2 }));
  CHECK((cube[StickerPosition{ Face::Down, 7 }]) == (StickerPosition{ Face::Up, 1 }));
  CHECK((cube[StickerPosition{ Face::Down, 8 }]) == (StickerPosition{ Face::Front, 8 }));
  CHECK((cube[StickerPosition{ Face::Front, 3 }]) == (StickerPosition{ Face::Up, 3 }));
  CHECK((cube[StickerPosition{ Face::Front, 4 }]) == (StickerPosition{ Face::Front, 4 }));
  CHECK((cube[StickerPosition{ Face::Front, 5 }]) == (StickerPosition{ Face::Up, 5 }));
  CHECK((cube[StickerPosition{ Face::Back, 3 }]) == (StickerPosition{ Face::Down, 5 }));
  CHECK((cube[StickerPosition{ Face::Back, 4 }]) == (StickerPosition{ Face::Back, 4 }));
  CHECK((cube[StickerPosition{ Face::Back, 5 }]) == (StickerPosition{ Face::Back, 3 }));
  CHECK((cube[StickerPosition{ Face::Left, 4 }]) == (StickerPosition{ Face::Left, 4 }));
  CHECK((cube[StickerPosition{ Face::Right, 4 }]) == (StickerPosition{ Face::Right, 4 }));
}

TEST_CASE("3x3x3 - Ensure we can apply an empty move sequence string", "[Cube3x3x3]")
{
  Cube3x3x3 cube;
  cube += "";
  for (size_t face = 0; face < 6; ++face)
    for (size_t index = 0; index < 9; ++index)
      CHECK((cube[StickerPosition{ static_cast<Face>(face), index }]) == (StickerPosition{ static_cast<Face>(face), index }));
}

TEST_CASE("3x3x3 - Ensure we can apply a move sequence string", "[Cube3x3x3]")
{
  Cube3x3x3 cube;
  cube += "L B2 R'";
  CHECK((cube[StickerPosition{ Face::Up, 0 }]) == (StickerPosition{ Face::Down, 8 }));
  CHECK((cube[StickerPosition{ Face::Up, 1 }]) == (StickerPosition{ Face::Down, 7 }));
  CHECK((cube[StickerPosition{ Face::Up, 2 }]) == (StickerPosition{ Face::Down, 6 }));
  CHECK((cube[StickerPosition{ Face::Up, 3 }]) == (StickerPosition{ Face::Back, 5 }));
  CHECK((cube[StickerPosition{ Face::Up, 4 }]) == (StickerPosition{ Face::Up, 4 }));
  CHECK((cube[StickerPosition{ Face::Up, 5 }]) == (StickerPosition{ Face::Down, 3 }));
  CHECK((cube[StickerPosition{ Face::Up, 6 }]) == (StickerPosition{ Face::Back, 2 }));
  CHECK((cube[StickerPosition{ Face::Up, 7 }]) == (StickerPosition{ Face::Up, 7 }));
  CHECK((cube[StickerPosition{ Face::Up, 8 }]) == (StickerPosition{ Face::Down, 0 }));
  CHECK((cube[StickerPosition{ Face::Down, 0 }]) == (StickerPosition{ Face::Front, 0 }));
  CHECK((cube[StickerPosition{ Face::Down, 1 }]) == (StickerPosition{ Face::Down, 1 }));
  CHECK((cube[StickerPosition{ Face::Down, 2 }]) == (StickerPosition{ Face::Front, 2 }));
  CHECK((cube[StickerPosition{ Face::Down, 3 }]) == (StickerPosition{ Face::Front, 3 }));
  CHECK((cube[StickerPosition{ Face::Down, 4 }]) == (StickerPosition{ Face::Down, 4 }));
  CHECK((cube[StickerPosition{ Face::Down, 5 }]) == (StickerPosition{ Face::Front, 5 }));
  CHECK((cube[StickerPosition{ Face::Down, 6 }]) == (StickerPosition{ Face::Up, 2 }));
  CHECK((cube[StickerPosition{ Face::Down, 7 }]) == (StickerPosition{ Face::Up, 1 }));
  CHECK((cube[StickerPosition{ Face::Down, 8 }]) == (StickerPosition{ Face::Front, 8 }));
  CHECK((cube[StickerPosition{ Face::Front, 3 }]) == (StickerPosition{ Face::Up, 3 }));
  CHECK((cube[StickerPosition{ Face::Front, 4 }]) == (StickerPosition{ Face::Front, 4 }));
  CHECK((cube[StickerPosition{ Face::Front, 5 }]) == (StickerPosition{ Face::Up, 5 }));
  CHECK((cube[StickerPosition{ Face::Back, 3 }]) == (StickerPosition{ Face::Down, 5 }));
  CHECK((cube[StickerPosition{ Face::Back, 4 }]) == (StickerPosition{ Face::Back, 4 }));
  CHECK((cube[StickerPosition{ Face::Back, 5 }]) == (StickerPosition{ Face::Back, 3 }));
  CHECK((cube[StickerPosition{ Face::Left, 4 }]) == (StickerPosition{ Face::Left, 4 }));
  CHECK((cube[StickerPosition{ Face::Right, 4 }]) == (StickerPosition{ Face::Right, 4 }));
}

TEST_CASE("3x3x3 - Ensure the default cube is solved", "[Cube3x3x3]")
{
  Cube3x3x3 cube;
  CHECK(cube.IsSolved());
}

TEST_CASE("3x3x3 - Ensure that scrambled cubes are not solved", "[Cube3x3x3]")
{
  Cube3x3x3 cube;
  CHECK(!(cube += "F").IsSolved());
  CHECK(!(cube += "U'").IsSolved());
  CHECK(!(cube += "R2").IsSolved());
  CHECK(!(cube += "B2").IsSolved());
  CHECK(!(cube += "L'").IsSolved());
  CHECK(!(cube += "D").IsSolved());
  CHECK((cube += "D' L B2 R2 U F'").IsSolved());
}
