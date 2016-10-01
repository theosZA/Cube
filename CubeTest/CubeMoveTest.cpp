#include "catch.hpp"

#include "CubeMove.h"

TEST_CASE("Ensure two identical cube moves are equal", "[CubeMove]")
{
  CHECK((CubeMove{ Face::Back, 2 }) == (CubeMove{ Face::Back, 2 }));
}

TEST_CASE("Ensure two different cube moves are not equal", "[CubeMove]")
{
  CHECK_FALSE((CubeMove{ Face::Back, 2 }) == (CubeMove{ Face::Front, 2 }));
  CHECK_FALSE((CubeMove{ Face::Back, 2 }) == (CubeMove{ Face::Back, 1 }));
}

TEST_CASE("Check the inverses of various cube moves", "[CubeMove]")
{
  CHECK(InvertMove(CubeMove{ Face::Front, 1 }) == (CubeMove{ Face::Front, -1}));
  CHECK(InvertMove(CubeMove{ Face::Up, 2 }) == (CubeMove{ Face::Up, 2 }));
  CHECK(InvertMove(CubeMove{ Face::Right, -1 }) == (CubeMove{ Face::Right, 1 }));
}

TEST_CASE("Check that an empty sequence can be inverted", "[CubeMove]")
{
  CHECK(InvertMoveSequence(std::vector<CubeMove>{}).empty());
}

TEST_CASE("Check that a single move in a move sequence can be inverted", "[CubeMove]")
{
  CHECK(InvertMoveSequence(std::vector<CubeMove>{ CubeMove{ Face::Front, 1 } }) == (std::vector<CubeMove>{ CubeMove{ Face::Front, -1 } }));
}

TEST_CASE("Check that a multiple move sequence can be inverted", "[CubeMove]")
{
  auto forward = std::vector<CubeMove> 
  {
    CubeMove{ Face::Front, 1 },
    CubeMove{ Face::Up, 2 },
    CubeMove{ Face::Right, -1 }
  };
  auto inverse = std::vector<CubeMove>
  {
    CubeMove{ Face::Right, 1 },
    CubeMove{ Face::Up, 2 },
    CubeMove{ Face::Front, -1 }
  };

  CHECK(InvertMoveSequence(forward) == inverse);
}

TEST_CASE("Check that an empty string move sequence can be parsed", "[CubeMove]")
{
  CHECK(ParseMoveSequence("").empty());
}

TEST_CASE("Check that a single move as a move sequence can be parsed", "[CubeMove]")
{
  CHECK(ParseMoveSequence("F") == (std::vector<CubeMove>{ CubeMove{ Face::Front, 1 } }));
  CHECK(ParseMoveSequence("F'") == (std::vector<CubeMove>{ CubeMove{ Face::Front, -1 } }));
  CHECK(ParseMoveSequence("F2") == (std::vector<CubeMove>{ CubeMove{ Face::Front, 2 } }));
  CHECK(ParseMoveSequence("B") == (std::vector<CubeMove>{ CubeMove{ Face::Back, 1 } }));
  CHECK(ParseMoveSequence("B'") == (std::vector<CubeMove>{ CubeMove{ Face::Back, -1 } }));
  CHECK(ParseMoveSequence("B2") == (std::vector<CubeMove>{ CubeMove{ Face::Back, 2 } }));
  CHECK(ParseMoveSequence("L") == (std::vector<CubeMove>{ CubeMove{ Face::Left, 1 } }));
  CHECK(ParseMoveSequence("L'") == (std::vector<CubeMove>{ CubeMove{ Face::Left, -1 } }));
  CHECK(ParseMoveSequence("L2") == (std::vector<CubeMove>{ CubeMove{ Face::Left, 2 } }));
  CHECK(ParseMoveSequence("R") == (std::vector<CubeMove>{ CubeMove{ Face::Right, 1 } }));
  CHECK(ParseMoveSequence("R'") == (std::vector<CubeMove>{ CubeMove{ Face::Right, -1 } }));
  CHECK(ParseMoveSequence("R2") == (std::vector<CubeMove>{ CubeMove{ Face::Right, 2 } }));
  CHECK(ParseMoveSequence("U") == (std::vector<CubeMove>{ CubeMove{ Face::Up, 1 } }));
  CHECK(ParseMoveSequence("U'") == (std::vector<CubeMove>{ CubeMove{ Face::Up, -1 } }));
  CHECK(ParseMoveSequence("U2") == (std::vector<CubeMove>{ CubeMove{ Face::Up, 2 } }));
  CHECK(ParseMoveSequence("D") == (std::vector<CubeMove>{ CubeMove{ Face::Down, 1 } }));
  CHECK(ParseMoveSequence("D'") == (std::vector<CubeMove>{ CubeMove{ Face::Down, -1 } }));
  CHECK(ParseMoveSequence("D2") == (std::vector<CubeMove>{ CubeMove{ Face::Down, 2 } }));
}

TEST_CASE("Check that multiple moves as a move sequence can be parsed", "[CubeMove]")
{
  CHECK(ParseMoveSequence("F U2 R'") == (std::vector<CubeMove>{ CubeMove{ Face::Front, 1 }, CubeMove{ Face::Up, 2 }, CubeMove{ Face::Right, -1 } }));
}

TEST_CASE("Check that an empty move sequence can be converted to text", "[CubeMove]")
{
  CHECK(MoveSequenceToText(std::vector<CubeMove>{}).empty());
}

TEST_CASE("Check that a single move as a move sequence can be converted to text", "[CubeMove]")
{
  CHECK(std::string{ "F" } == MoveSequenceToText(std::vector<CubeMove>{ CubeMove{ Face::Front, 1 } }));
  CHECK(std::string{ "F'" } == MoveSequenceToText(std::vector<CubeMove>{ CubeMove{ Face::Front, -1 } }));
  CHECK(std::string{ "F2" } == MoveSequenceToText(std::vector<CubeMove>{ CubeMove{ Face::Front, 2 } }));
  CHECK(std::string{ "B" } == MoveSequenceToText(std::vector<CubeMove>{ CubeMove{ Face::Back, 1 } }));
  CHECK(std::string{ "B'" } == MoveSequenceToText(std::vector<CubeMove>{ CubeMove{ Face::Back, -1 } }));
  CHECK(std::string{ "B2" } == MoveSequenceToText(std::vector<CubeMove>{ CubeMove{ Face::Back, 2 } }));
  CHECK(std::string{ "L" } == MoveSequenceToText(std::vector<CubeMove>{ CubeMove{ Face::Left, 1 } }));
  CHECK(std::string{ "L'" } == MoveSequenceToText(std::vector<CubeMove>{ CubeMove{ Face::Left, -1 } }));
  CHECK(std::string{ "L2" } == MoveSequenceToText(std::vector<CubeMove>{ CubeMove{ Face::Left, 2 } }));
  CHECK(std::string{ "R" } == MoveSequenceToText(std::vector<CubeMove>{ CubeMove{ Face::Right, 1 } }));
  CHECK(std::string{ "R'" } == MoveSequenceToText(std::vector<CubeMove>{ CubeMove{ Face::Right, -1 } }));
  CHECK(std::string{ "R2" } == MoveSequenceToText(std::vector<CubeMove>{ CubeMove{ Face::Right, 2 } }));
  CHECK(std::string{ "U" } == MoveSequenceToText(std::vector<CubeMove>{ CubeMove{ Face::Up, 1 } }));
  CHECK(std::string{ "U'" } == MoveSequenceToText(std::vector<CubeMove>{ CubeMove{ Face::Up, -1 } }));
  CHECK(std::string{ "U2" } == MoveSequenceToText(std::vector<CubeMove>{ CubeMove{ Face::Up, 2 } }));
  CHECK(std::string{ "D" } == MoveSequenceToText(std::vector<CubeMove>{ CubeMove{ Face::Down, 1 } }));
  CHECK(std::string{ "D'" } == MoveSequenceToText(std::vector<CubeMove>{ CubeMove{ Face::Down, -1 } }));
  CHECK(std::string{ "D2" } == MoveSequenceToText(std::vector<CubeMove>{ CubeMove{ Face::Down, 2 } }));
}

TEST_CASE("Check that multiple moves as a move sequence can be converted to text", "[CubeMove]")
{
  CHECK(std::string("F U2 R'") == MoveSequenceToText(std::vector<CubeMove>{ CubeMove{ Face::Front, 1 }, CubeMove{ Face::Up, 2 }, CubeMove{ Face::Right, -1 } }));
}

TEST_CASE("Check that an empty move sequence when simplified is still empty", "[CubeMove]")
{
  CHECK(SimplifyMoveSequence(std::vector<CubeMove>{}).empty());
}

TEST_CASE("Check that a single move as a move sequence when simplified is unchanged", "[CubeMove]")
{
  CHECK(SimplifyMoveSequence(std::vector<CubeMove>{ CubeMove{ Face::Back, 2 } }) == (std::vector<CubeMove>{ CubeMove{ Face::Back, 2 } }));
}

TEST_CASE("Check that multiple moves with no simplification possible when simplified is unchanged", "[CubeMove]")
{
  auto sequence = std::vector<CubeMove>
  {
    CubeMove{ Face::Front, 1 },
    CubeMove{ Face::Up, 2 },
    CubeMove{ Face::Right, -1 }
  };
  CHECK(SimplifyMoveSequence(sequence) == sequence);
}

TEST_CASE("Check that two consecutive moves on the same face are simplified", "[CubeMove]")
{
  auto sequence = std::vector<CubeMove>
  {
    CubeMove{ Face::Front, 1 },
    CubeMove{ Face::Front, 1 },
    CubeMove{ Face::Up, 1 },
    CubeMove{ Face::Up, 2 },
    CubeMove{ Face::Right, -1 },
    CubeMove{ Face::Right, -1 }
  };
  auto simplified = std::vector<CubeMove>
  {
    CubeMove{ Face::Front, 2 },
    CubeMove{ Face::Up, -1 },
    CubeMove{ Face::Right, 2 }
  };
  CHECK(SimplifyMoveSequence(sequence) == simplified);
}

TEST_CASE("Check that two consecutive moves on the same face which cancel each other are removed when simplified", "[CubeMove]")
{
  auto sequence = std::vector<CubeMove>
  {
    CubeMove{ Face::Front, 1 },
    CubeMove{ Face::Front, -1 },
    CubeMove{ Face::Up, 2 },
    CubeMove{ Face::Up, 2 },
    CubeMove{ Face::Right, -1 },
    CubeMove{ Face::Right, 1 }
  };
  CHECK(SimplifyMoveSequence(sequence).empty());
}

TEST_CASE("Check that more than two consecutive moves on the same face are simplified", "[CubeMove]")
{
  auto sequence = std::vector<CubeMove>
  {
    CubeMove{ Face::Front, 1 },
    CubeMove{ Face::Front, 2 },
    CubeMove{ Face::Front, -1 },
    CubeMove{ Face::Up, 2 },
    CubeMove{ Face::Up, -1 },
    CubeMove{ Face::Up, 2 },
    CubeMove{ Face::Up, -1 },
    CubeMove{ Face::Right, -1 },
    CubeMove{ Face::Right, 1 },
    CubeMove{ Face::Right, 1 }
  };
  auto simplified = std::vector<CubeMove>
  {
    CubeMove{ Face::Front, 2 },
    CubeMove{ Face::Up, 2 },
    CubeMove{ Face::Right, 1 }
  };
  CHECK(SimplifyMoveSequence(sequence) == simplified);
}

TEST_CASE("Check that parallel moves are considered for re-ordering when simplified", "[CubeMove]")
{
  auto sequence = std::vector<CubeMove>
  {
    CubeMove{ Face::Front, 1 },
    CubeMove{ Face::Left, -1 },
    CubeMove{ Face::Right, 2 },
    CubeMove{ Face::Left, 2 },
    CubeMove{ Face::Up, -1 },
    CubeMove{ Face::Down, -1 },
    CubeMove{ Face::Up, -1 },
    CubeMove{ Face::Down, 1 }
  };
  auto simplifiedLeft1st = std::vector<CubeMove>
  {
    CubeMove{ Face::Front, 1 },
    CubeMove{ Face::Left, 1 },
    CubeMove{ Face::Right, 2 },
    CubeMove{ Face::Up, 2 }
  };
  auto simplifiedRight1st = std::vector<CubeMove>
  {
    CubeMove{ Face::Front, 1 },
    CubeMove{ Face::Right, 2 },
    CubeMove{ Face::Left, 1 },
    CubeMove{ Face::Up, 2 }
  };
  CHECK((SimplifyMoveSequence(sequence) == simplifiedLeft1st || SimplifyMoveSequence(sequence) == simplifiedRight1st));
}

