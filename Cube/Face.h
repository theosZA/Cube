#pragma once

#include <iosfwd>
#include <sstream>
#include <stdexcept>

enum class Face : size_t
{
  Down = 0,
  Up = 1,
  Front = 2,
  Right = 3,
  Back = 4,
  Left = 5
};

// Returns the face opposite the given face.
Face GetOppositeFace(Face);
// Returns the face that follows in a clockwise order around a corner.
Face GetNextFaceClockwise(Face a, Face b);

inline Face GetFace(char c)
{
  switch (c)
  {
    case 'F': case 'f': return Face::Front;
    case 'R': case 'r': return Face::Right;
    case 'B': case 'b': return Face::Back;
    case 'L': case 'l': return Face::Left;
    case 'U': case 'u': return Face::Up;
    case 'D': case 'd': return Face::Down;
    default:
      std::ostringstream errorMessage;
      errorMessage << "Invalid face '" << c << "'";
      throw std::domain_error(errorMessage.str());
  }
}

inline char GetFaceChar(Face face)
{
  switch (face)
  {
    case Face::Down:  return 'D';
    case Face::Up:    return 'U';
    case Face::Front: return 'F';
    case Face::Right: return 'R';
    case Face::Back:  return 'B';
    case Face::Left:  return 'L';
    default:
      throw std::domain_error("Invalid face");
  }
}

std::ostream& operator<<(std::ostream&, Face);
