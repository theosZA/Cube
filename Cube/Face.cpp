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

Face GetNextFaceClockwise(Face a, Face b)
{
  switch (a)
  {
    case Face::Down:  
      switch (b)
      {
        case Face::Front: return Face::Right;
        case Face::Left:  return Face::Front;
        case Face::Back:  return Face::Left;
        case Face::Right: return Face::Back;
        default:          throw std::domain_error("Faces are not adjacent");
      }
    
    case Face::Up:
      switch (b)
      {
        case Face::Front: return Face::Left;
        case Face::Left:  return Face::Back;
        case Face::Back:  return Face::Right;
        case Face::Right: return Face::Front;
        default:          throw std::domain_error("Faces are not adjacent");
      }
    
    case Face::Front:
      switch (b)
      {
        case Face::Down:  return Face::Left;
        case Face::Up:    return Face::Right;
        case Face::Left:  return Face::Up;
        case Face::Right: return Face::Down;
        default:          throw std::domain_error("Faces are not adjacent");
      }

    case Face::Left:
      switch (b)
      {
        case Face::Down:  return Face::Back;
        case Face::Up:    return Face::Front;
        case Face::Front: return Face::Down;
        case Face::Back:  return Face::Up;
        default:          throw std::domain_error("Faces are not adjacent");
      }

    case Face::Back:
      switch (b)
      {
        case Face::Down:  return Face::Right;
        case Face::Up:    return Face::Left;
        case Face::Left:  return Face::Down;
        case Face::Right: return Face::Up;
        default:          throw std::domain_error("Faces are not adjacent");
      }

    case Face::Right:
      switch (b)
      {
        case Face::Down:  return Face::Front;
        case Face::Up:    return Face::Back;
        case Face::Front: return Face::Up;
        case Face::Back:  return Face::Down;
        default:          throw std::domain_error("Faces are not adjacent");
      }

    default:          throw std::domain_error("Invalid face");
  }
}

std::ostream& operator<<(std::ostream& out, Face face)
{
  out << GetFaceChar(face);
  return out;
}