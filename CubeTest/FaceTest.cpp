#include "catch.hpp"

#include "Face.h"

TEST_CASE("Ensure the correct faces are opposite each other", "[Face]")
{
  CHECK(GetOppositeFace(Face::Front) == Face::Back);
  CHECK(GetOppositeFace(Face::Back) == Face::Front);
  CHECK(GetOppositeFace(Face::Up) == Face::Down);
  CHECK(GetOppositeFace(Face::Down) == Face::Up);
  CHECK(GetOppositeFace(Face::Left) == Face::Right);
  CHECK(GetOppositeFace(Face::Right) == Face::Left);
}

TEST_CASE("Ensure that characters convert to the correct faces", "[Face]")
{
  CHECK(GetFace('B') == Face::Back);
  CHECK(GetFace('F') == Face::Front);
  CHECK(GetFace('D') == Face::Down);
  CHECK(GetFace('U') == Face::Up);
  CHECK(GetFace('R') == Face::Right);
  CHECK(GetFace('L') == Face::Left);
  CHECK(GetFace('b') == Face::Back);
  CHECK(GetFace('f') == Face::Front);
  CHECK(GetFace('d') == Face::Down);
  CHECK(GetFace('u') == Face::Up);
  CHECK(GetFace('r') == Face::Right);
  CHECK(GetFace('l') == Face::Left);
}

TEST_CASE("Ensure that faces convert to the correct characters", "[Face]")
{
  CHECK(GetFaceChar(Face::Back) == 'B');
  CHECK(GetFaceChar(Face::Front) == 'F');
  CHECK(GetFaceChar(Face::Down) == 'D');
  CHECK(GetFaceChar(Face::Up) == 'U');
  CHECK(GetFaceChar(Face::Right) == 'R');
  CHECK(GetFaceChar(Face::Left) == 'L');
}
