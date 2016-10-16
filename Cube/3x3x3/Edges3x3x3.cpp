#include "Edges3x3x3.h"

#include <array>

// The state is encoded as follows:
// 4 bits: cubie in position UF (0=UF, 1=UL, 2=UB, 3=UR, 4=DF, 5=DL, 6=DB, 7=DR, 8=LF, 9=LB, 10=RB, 11=RF)
// 4 bits: cubie in position UL
// 4 bits: cubie in position UB
// 4 bits: cubie in position UR
// 4 bits: cubie in position DF
// 4 bits: cubie in position DL
// 4 bits: cubie in position DB
// 4 bits: cubie in position DR
// 4 bits: cubie in position LF
// 4 bits: cubie in position LB
// 4 bits: cubie in position RB
// 4 bits: cubie in position RF
// 1 bit: orientation of cubie in position UF
// 1 bit: orientation of cubie in position UL
// 1 bit: orientation of cubie in position UB
// 1 bit: orientation of cubie in position UR
// 1 bit: orientation of cubie in position DF
// 1 bit: orientation of cubie in position DL
// 1 bit: orientation of cubie in position DB
// 1 bit: orientation of cubie in position DR
// 1 bit: orientation of cubie in position LF
// 1 bit: orientation of cubie in position LB
// 1 bit: orientation of cubie in position RB
// 1 bit: orientation of cubie in position RF

static const std::array<std::array<Face, 2>, 12> edgeFace
{
  std::array<Face, 2>{ Face::Up, Face::Front },
  std::array<Face, 2>{ Face::Up, Face::Left },
  std::array<Face, 2>{ Face::Up, Face::Back },
  std::array<Face, 2>{ Face::Up, Face::Right },
  std::array<Face, 2>{ Face::Down, Face::Front },
  std::array<Face, 2>{ Face::Down, Face::Left },
  std::array<Face, 2>{ Face::Down, Face::Back },
  std::array<Face, 2>{ Face::Down, Face::Right },
  std::array<Face, 2>{ Face::Left, Face::Front },
  std::array<Face, 2>{ Face::Left, Face::Back },
  std::array<Face, 2>{ Face::Right, Face::Back },
  std::array<Face, 2>{ Face::Right, Face::Front }
};

static const std::array<std::array<size_t, 2>, 12> cubieIndex
{
  std::array<size_t, 2>{ 7, 1 },
  std::array<size_t, 2>{ 3, 1 },
  std::array<size_t, 2>{ 1, 1 },
  std::array<size_t, 2>{ 5, 1 },
  std::array<size_t, 2>{ 1, 7 },
  std::array<size_t, 2>{ 3, 7 },
  std::array<size_t, 2>{ 7, 7 },
  std::array<size_t, 2>{ 5, 7 },
  std::array<size_t, 2>{ 5, 3 },
  std::array<size_t, 2>{ 3, 5 },
  std::array<size_t, 2>{ 5, 3 },
  std::array<size_t, 2>{ 3, 5 }
};

std::uint32_t GetEdgePositionIndex(Face face, size_t index)
{
  switch (face)
  {
    case Face::Down:
      switch (index)
      {
        case 1: return 4;
        case 3: return 5;
        case 5: return 7;
        case 7: return 6;
        default:
          throw std::domain_error("Invalid edge index " + std::to_string(index));
      }

    case Face::Up:
      switch (index)
      {
        case 1: return 2;
        case 3: return 1;
        case 5: return 3;
        case 7: return 0;
        default:
          throw std::domain_error("Invalid edge index " + std::to_string(index));
      }

    case Face::Front:
      switch (index)
      {
        case 1: return 0;
        case 3: return 8;
        case 5: return 11;
        case 7: return 4;
        default:
          throw std::domain_error("Invalid edge index " + std::to_string(index));
      }

    case Face::Left:
      switch (index)
      {
        case 1: return 1;
        case 3: return 9;
        case 5: return 8;
        case 7: return 5;
        default:
          throw std::domain_error("Invalid edge index " + std::to_string(index));
      }

    case Face::Back:
      switch (index)
      {
        case 1: return 2;
        case 3: return 10;
        case 5: return 9;
        case 7: return 6;
        default:
          throw std::domain_error("Invalid edge index " + std::to_string(index));
      }

    case Face::Right:
      switch (index)
      {
        case 1: return 3;
        case 3: return 11;
        case 5: return 10;
        case 7: return 7;
        default:
          throw std::domain_error("Invalid edge index " + std::to_string(index));
      }

    default:
      throw std::domain_error("Invalid face " + std::to_string(static_cast<int>(face)));
  }
}

Edges3x3x3::Edges3x3x3()
: state(0xBA9876543210)
{}

void Edges3x3x3::ApplyMove(Face face, int quarterRotationsClockwise)
{
  // Edge cubies on each face in clockwise order.
  static const std::array<std::array<std::uint32_t, 4>, 6> positionsOnFace
  {
    std::array<std::uint32_t, 4>{ 4, 7, 6, 5 }, // down
    std::array<std::uint32_t, 4>{ 0, 1, 2, 3 }, // up
    std::array<std::uint32_t, 4>{ 0, 11, 4, 8 }, // front
    std::array<std::uint32_t, 4>{ 3, 10, 7, 11 }, // right
    std::array<std::uint32_t, 4>{ 2, 9, 6, 10 }, // back
    std::array<std::uint32_t, 4>{ 1, 8, 5, 9 }  // left
  };
  auto& positions = positionsOnFace[static_cast<size_t>(face)];

  bool flip = ((face == Face::Left || face == Face::Right) && quarterRotationsClockwise != 2);

  std::array<std::uint64_t, 4> cubies;
  std::array<bool, 4> orientations;
  for (size_t sourceIndex = 0; sourceIndex < 4; ++sourceIndex)
  {
    cubies[sourceIndex] = GetCubie(positions[sourceIndex]);
    orientations[sourceIndex] = GetOrientation(positions[sourceIndex]);
  }
  for (size_t destIndex = 0; destIndex < 4; ++destIndex)
  {
    auto sourceIndex = (destIndex + 4 - quarterRotationsClockwise) % 4;
    SetCubie(positions[destIndex], cubies[sourceIndex]);
    SetOrientation(positions[destIndex], orientations[sourceIndex] ^ flip);
  }
}

std::pair<Face, size_t> Edges3x3x3::Get(Face face, size_t index) const
{
  auto position = GetEdgePositionIndex(face, index);
  auto cubie = GetCubie(position);
  auto orientation = GetOrientation(position);
  auto orientationIndex = (orientation ^ (edgeFace[position][1] == face)) ? 1 : 0;
  
  return std::pair<Face, size_t>{ edgeFace[cubie][orientationIndex], cubieIndex[cubie][orientationIndex] };
}

std::uint64_t Edges3x3x3::GetCubie(std::uint32_t positionIndex) const
{
  return 0xF & (state >> (positionIndex * 4));
}

bool Edges3x3x3::GetOrientation(std::uint32_t positionIndex) const
{
  return (0x1 & (state >> (48 + positionIndex))) == 1;
}

void Edges3x3x3::SetCubie(std::uint32_t positionIndex, std::uint64_t cubie)
{
  state |= (0xFllu << (positionIndex * 4));
  state ^= (0xFllu << (positionIndex * 4));
  state |= (cubie << (positionIndex * 4));
}

void Edges3x3x3::SetOrientation(std::uint32_t positionIndex, bool orientation)
{
  state |= (0x1llu << (48 + positionIndex));
  if (!orientation)
    state ^= (0x1llu << (48 + positionIndex));
}
