#include "CubeMove.h"

#include <array>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>

bool CubeMove::operator==(const CubeMove& move) const
{
  return face == move.face && quarterRotationsClockwise == move.quarterRotationsClockwise;
}

std::ostream& operator<< (std::ostream& out, const CubeMove& move)
{
  out << move.face;
  switch (move.quarterRotationsClockwise)
  {
    case 2:
      out << '2';
      break;

    case -1:
      out << '\'';
      break;

    default:
      ; // clockwise rotation
  }
  return out;
}

CubeMove InvertMove(CubeMove move)
{
  switch (move.quarterRotationsClockwise % 4)
  {
    case 1:
    case -3:
      return CubeMove{ move.face, -1 };

    case -1:
    case 3:
      return CubeMove{ move.face, 1 };

    case 2:
    case -2:
      return CubeMove{ move.face, 2 };

    default:  // 0
      return CubeMove{ move.face, 0 };
  }
}

std::vector<CubeMove> InvertMoveSequence(const std::vector<CubeMove>& moveSequence)
{
  std::vector<CubeMove> invertedMoveSequence;
  invertedMoveSequence.reserve(moveSequence.size());
  for (auto moveIter = moveSequence.rbegin(); moveIter != moveSequence.rend(); ++moveIter)
  {
    invertedMoveSequence.push_back(InvertMove(*moveIter));
  }
  return invertedMoveSequence;
}

std::vector<CubeMove> ParseMoveSequence(const std::string& moves)
{
  std::vector<CubeMove> result;
  std::istringstream moveStream(moves);
  while (moveStream.rdbuf()->in_avail())
  {
    std::string move;
    moveStream >> move;
    int quarterRotationsClockwise = 0;
    switch (move.size())
    {
      case 0:
        throw std::domain_error("Empty move");

      case 1:
        quarterRotationsClockwise = 1;
        break;

      case 2:
        switch (move[1])
        {
          case '\'':
            quarterRotationsClockwise = -1;
            break;

          case '2':
            quarterRotationsClockwise = 2;
            break;

          default:
            std::ostringstream errorMessage;
            errorMessage << "Invalid move qualifier '" << move[1] << "'";
            throw std::domain_error(errorMessage.str());
        }
        break;

      default:
        throw std::domain_error("Move too large");
    }

    result.push_back(CubeMove{ GetFace(move[0]), quarterRotationsClockwise });
  }
  return result;
}

std::string MoveSequenceToText(const std::vector<CubeMove>& moves)
{
  std::ostringstream moveStream;
  for (size_t i = 0; i < moves.size(); ++i)
  {
    if (i != 0)
      moveStream << ' ';
    moveStream << moves[i];
  }
  return moveStream.str();
}

std::vector<CubeMove> SimplifyMoveSequence(const std::vector<CubeMove>& moves)
{
  std::vector<CubeMove> simplified;
  for (auto move : moves)
  {
    if (simplified.size() >= 2 && 
        move.face == GetOppositeFace(simplified.back().face) &&
        (simplified.end() - 1)->face == GetOppositeFace((simplified.end() - 2)->face))
    { // Re-order moves to allow us to simplify.
      std::swap(*(simplified.end() - 1), *(simplified.end() - 2));
    }
    if (!simplified.empty() && move.face == simplified.back().face)
    {
      auto& lastMove = simplified.back();
      lastMove.quarterRotationsClockwise += move.quarterRotationsClockwise;
      while (lastMove.quarterRotationsClockwise > 2)
      {
        lastMove.quarterRotationsClockwise -= 4;
      }
      while (lastMove.quarterRotationsClockwise < -1)
      {
        lastMove.quarterRotationsClockwise += 4;
      }
      if (lastMove.quarterRotationsClockwise == 0)
      {
        simplified.pop_back();
      }
    }
    else
    {
      simplified.push_back(move);
    }
  }

  return simplified;
}

std::vector<CubeMove> operator+(const std::vector<CubeMove>& a, const std::vector<CubeMove>& b)
{
  std::vector<CubeMove> sum = a;
  std::copy(b.begin(), b.end(), std::back_inserter(sum));
  return SimplifyMoveSequence(sum);
}

std::vector<CubeMove>& operator+=(std::vector<CubeMove>& a, const std::vector<CubeMove>& b)
{
  return a = a + b;
}

std::vector<CubeMove> Rotate(const std::vector<CubeMove>& moves, std::pair<Face, Face> oldFaces, std::pair<Face, Face> newFaces)
{
  auto oldFace3 = GetNextFaceClockwise(oldFaces.first, oldFaces.second);
  auto newFace3 = GetNextFaceClockwise(newFaces.first, newFaces.second);

  std::array<Face, 6> newFace;
  newFace[static_cast<size_t>(oldFaces.first)] = newFaces.first;
  newFace[static_cast<size_t>(oldFaces.second)] = newFaces.second;
  newFace[static_cast<size_t>(oldFace3)] = newFace3;
  newFace[static_cast<size_t>(GetOppositeFace(oldFaces.first))] = GetOppositeFace(newFaces.first);
  newFace[static_cast<size_t>(GetOppositeFace(oldFaces.second))] = GetOppositeFace(newFaces.second);
  newFace[static_cast<size_t>(GetOppositeFace(oldFace3))] = GetOppositeFace(newFace3);

  auto newMoves = moves;
  for (auto& move : newMoves)
    move.face = newFace[static_cast<size_t>(move.face)];
  return newMoves;
}
