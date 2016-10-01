#include "Face.h"

#include <iostream>

Face GetOppositeFace(Face face)
{
  switch (face)
  {
    case Face::Down:  return Face::Up;
    case Face::Up:    return Face::Down;
    case Face::Front: return Face::Back;
    case Face::Left:  return Face::Right;
    case Face::Back:  return Face::Front;
    case Face::Right: return Face::Left;
    default:          throw std::domain_error("Invalid face");
  }
}

std::ostream& operator<<(std::ostream& out, Face face)
{
  out << GetFaceChar(face);
  return out;
}