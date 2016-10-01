#include "MoveSequence.h"

const std::array<CubeMove, 18> MoveSequence::possibleMoves =
{
  CubeMove{ Face::Right, 1 },
  CubeMove{ Face::Right, -1 },
  CubeMove{ Face::Up, 1 },
  CubeMove{ Face::Up, -1 },
  CubeMove{ Face::Front, 1 },
  CubeMove{ Face::Front, -1 },
  CubeMove{ Face::Right, 2 },
  CubeMove{ Face::Up, 2 },
  CubeMove{ Face::Front, 2 },
  CubeMove{ Face::Left, 1 },
  CubeMove{ Face::Left, -1 },
  CubeMove{ Face::Left, 2 },
  CubeMove{ Face::Down, 1 },
  CubeMove{ Face::Down, -1 },
  CubeMove{ Face::Down, 2 },
  CubeMove{ Face::Back, 2 },
  CubeMove{ Face::Back, 1 },
  CubeMove{ Face::Back, -1 }
};

void MoveSequence::PushMove(CubeMove move)
{
  size_t index = std::distance(possibleMoves.begin(), std::find(possibleMoves.begin(), possibleMoves.end(), move));
  PushMove(index);
}

void MoveSequence::PopMove()
{
  if (moves.empty())
    return;

  cube.ApplyMove(InvertMove(possibleMoves[moves.back()]));
  moves.pop_back();
}

void MoveSequence::Clear()
{
  while (!moves.empty())
    PopMove();
}

void MoveSequence::GoToFirst(size_t length)
{
  Clear();
  for (size_t i = 0; i < length; ++i)
  { // Alternating R U
    PushMove(i % 2 == 0 ? 0 : 2);
  }
}

bool MoveSequence::GoToNext()
{
  if (moves.empty())
    return false;

  // Undo the current last move.
  auto moveIndex = moves.back();
  PopMove();

  // Increase to next move index. Can't repeat the face of the previous move.
  ++moveIndex;
  while (!moves.empty() &&
         moveIndex < possibleMoves.size() &&
         possibleMoves[moves.back()].face == possibleMoves[moveIndex].face)
  {
    ++moveIndex;
  }

  // If we run out of possible moves then we need our prior move in the move sequence to increase,
  // while we reset this move.
  if (moveIndex >= possibleMoves.size())
  {
    if (!GoToNext()) // recursive call will increase the prior move in the move sequence
      return false;  // or die trying
    moveIndex = 0;
    while (!moves.empty() &&
           possibleMoves[moves.back()].face == possibleMoves[moveIndex].face)
    {
      ++moveIndex;
    }
  }

  // We are happy that this is now our next move in order.
  PushMove(moveIndex);
  return true;
}

void MoveSequence::PushMove(size_t moveIndex)
{
  moves.push_back(moveIndex);
  cube.ApplyMove(possibleMoves[moveIndex]);
}
