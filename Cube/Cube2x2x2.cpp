#include "Cube2x2x2.h"

#include <array>

// The state is encoded as follows:
// 3 bits: cubie in position DLF (0=DLF, 1=DFR, 2=DBL, 3=DRB, 4=ULB, 5=UBR, 6=UFL, 7=URF)
// 3 bits: cubie in position DFR
// 3 bits: cubie in position DBL
// 3 bits: cubie in position DRB
// 3 bits: cubie in position ULB
// 3 bits: cubie in position UBR
// 1 bit: cubie in position UFL (0 for first remaining; 1 for second remaining)
// implied: cubie in position URF
// 12 bits: rotations broken down in base 3
// 1 trit: orientation of cubie in position DLF (0 = none, 1 = clockwise, 2 = anti-clockwise)
// 1 trit: orientation of cubie in position DFR
// 1 trit: orientation of cubie in position DBL
// 1 trit: orientation of cubie in position DRB
// 1 trit: orientation of cubie in position ULB
// 1 trit: orientation of cubie in position UBR
// 1 trit: orientation of cubie in position UFL
// impled: orientation of cubie in position URF

static const std::array<std::array<Face, 3>, 8> cubieFace
{
  std::array<Face, 3>{ Face::Down, Face::Left,   Face::Front },
  std::array<Face, 3>{ Face::Down, Face::Front,  Face::Right },
  std::array<Face, 3>{ Face::Down, Face::Back,   Face::Left },
  std::array<Face, 3>{ Face::Down, Face::Right,  Face::Back },
  std::array<Face, 3>{ Face::Up,   Face::Left,   Face::Back },
  std::array<Face, 3>{ Face::Up,   Face::Back,   Face::Right },
  std::array<Face, 3>{ Face::Up,   Face::Front,  Face::Left },
  std::array<Face, 3>{ Face::Up,   Face::Right,  Face::Front }
};

static const std::array<std::array<size_t, 3>, 8> cubieIndex
{
  std::array<size_t, 3>{ 0, 3, 2 },
  std::array<size_t, 3>{ 1, 3, 2 },
  std::array<size_t, 3>{ 2, 3, 2 },
  std::array<size_t, 3>{ 3, 3, 2 },
  std::array<size_t, 3>{ 0, 0, 1 },
  std::array<size_t, 3>{ 1, 0, 1 },
  std::array<size_t, 3>{ 2, 0, 1 },
  std::array<size_t, 3>{ 3, 0, 1 }
};

static const std::array<int, 8> powersOf3 { 1, 3, 9, 27, 81, 243, 729, 2187 };

template <class I> 
I IntegerSquareRoot(I value)
{
  I i = 0;
  while (i * i < value)
    ++i;
  return i;
}

std::uint32_t GetPositionIndex(Face face, size_t index)
{
  switch (face)
  {
    case Face::Down:
      switch (index)
      {
        case 0: return 0;
        case 1: return 1;
        case 2: return 2;
        case 3: return 3;
        default:
          throw std::domain_error("Invalid index " + std::to_string(index));
      }

    case Face::Up:
      switch (index)
      {
        case 0: return 4;
        case 1: return 5;
        case 2: return 6;
        case 3: return 7;
        default:
          throw std::domain_error("Invalid index " + std::to_string(index));
      }

    case Face::Front:
      switch (index)
      {
        case 0: return 6;
        case 1: return 7;
        case 2: return 0;
        case 3: return 1;
        default:
          throw std::domain_error("Invalid index " + std::to_string(index));
      }

    case Face::Left:
      switch (index)
      {
        case 0: return 4;
        case 1: return 6;
        case 2: return 2;
        case 3: return 0;
        default:
          throw std::domain_error("Invalid index " + std::to_string(index));
      }

    case Face::Back:
      switch (index)
      {
        case 0: return 5;
        case 1: return 4;
        case 2: return 3;
        case 3: return 2;
        default:
          throw std::domain_error("Invalid index " + std::to_string(index));
      }

    case Face::Right:
      switch (index)
      {
        case 0: return 7;
        case 1: return 5;
        case 2: return 1;
        case 3: return 3;
        default:
          throw std::domain_error("Invalid index " + std::to_string(index));
      }

    default:
      throw std::domain_error("Invalid face " + std::to_string(static_cast<int>(face)));
  }
}

std::uint32_t GetOrientationOfPosition(Face face, size_t index)
{
  switch (face)
  {
    case Face::Down:
      switch (index)
      {
        case 0: return 0;
        case 1: return 0;
        case 2: return 0;
        case 3: return 0;
        default:
          throw std::domain_error("Invalid index " + std::to_string(index));
      }

    case Face::Up:
      switch (index)
      {
        case 0: return 0;
        case 1: return 0;
        case 2: return 0;
        case 3: return 0;
        default:
          throw std::domain_error("Invalid index " + std::to_string(index));
      }

    case Face::Front:
      switch (index)
      {
        case 0: return 1;
        case 1: return 2;
        case 2: return 2;
        case 3: return 1;
        default:
          throw std::domain_error("Invalid index " + std::to_string(index));
      }

    case Face::Left:
      switch (index)
      {
        case 0: return 1;
        case 1: return 2;
        case 2: return 2;
        case 3: return 1;
        default:
          throw std::domain_error("Invalid index " + std::to_string(index));
      }

    case Face::Back:
      switch (index)
      {
        case 0: return 1;
        case 1: return 2;
        case 2: return 2;
        case 3: return 1;
        default:
          throw std::domain_error("Invalid index " + std::to_string(index));
      }

    case Face::Right:
      switch (index)
      {
        case 0: return 1;
        case 1: return 2;
        case 2: return 2;
        case 3: return 1;
        default:
          throw std::domain_error("Invalid index " + std::to_string(index));
      }

    default:
      throw std::domain_error("Invalid face " + std::to_string(static_cast<int>(face)));
  }
}

// No orientation
// Cubie bits are:
// 0 101 100 011 010 001 000
Cube2x2x2::Cube2x2x2()
: state(181896)
{}

Cube2x2x2::Cube2x2x2(int key)
: state(static_cast<std::uint32_t>(key))
{}

// Returns a unique key specifying the position.
int Cube2x2x2::GetKey() const
{
  return static_cast<int>(state);
}

size_t Cube2x2x2::GetSize() const
{
  return 2;
}

void Cube2x2x2::ApplyMove(Face face, int quarterRotationsClockwise)
{
  // Cubies on each face in clockwise order. Cubies 6 and 7 must be last in order because
  // the calculations for the cubies in those positions must be inferred from the others.
  static const std::array<std::array<std::uint32_t, 4>, 6> positionsOnFace
  {
    std::array<std::uint32_t, 4>{ 0, 1, 3, 2 }, // down
    std::array<std::uint32_t, 4>{ 4, 5, 7, 6 }, // up
    std::array<std::uint32_t, 4>{ 1, 0, 6, 7 }, // front
    std::array<std::uint32_t, 4>{ 5, 3, 1, 7 }, // right
    std::array<std::uint32_t, 4>{ 2, 3, 5, 4 }, // back
    std::array<std::uint32_t, 4>{ 0, 2, 4, 6 }  // left
  };
  auto& positions = positionsOnFace[static_cast<size_t>(face)];

  std::array<std::uint32_t, 4> cubies;
  std::array<std::uint32_t, 4> orientations;
  for (size_t i = 0; i < 4; ++i)
  {
    cubies[i] = GetCubie(positions[i]);
    orientations[i] = GetOrientation(positions[i]);
  }
  for (size_t destIndex = 0; destIndex < 4; ++destIndex)
  {
    auto sourceIndex = (destIndex + 4 - quarterRotationsClockwise) % 4;
    SetCubie(positions[destIndex], cubies[sourceIndex]);
    auto sourcePosition = positions[sourceIndex];
    int orientationChange = 0;
    if (quarterRotationsClockwise != 2)
    {
      auto& faces = cubieFace[sourcePosition];
      orientationChange = faces[0] == face ? 0
                        : faces[1] == face ? 2
                        : 1;
    }
    SetOrientation(positions[destIndex], (orientations[sourceIndex] + orientationChange) % 3);
  }
}

std::pair<Face, size_t> Cube2x2x2::Get(Face face, size_t index) const
{
  auto position = GetPositionIndex(face, index);
  auto cubie = GetCubie(position);
  auto orientation = (GetOrientation(position) + GetOrientationOfPosition(face, index)) % 3;
  return std::make_pair(cubieFace[cubie][orientation], cubieIndex[cubie][orientation]);
}

std::uint32_t Cube2x2x2::GetCubie(std::uint32_t positionIndex) const
{
  if (positionIndex < 6)
    return (state >> (positionIndex * 3)) & 7;

  // Check which cubies are in earlier positions.
  std::uint32_t sumCubies = 0;
  std::uint32_t sumSquaresCubies = 0;
  for (int i = 0; i < 6; ++i)
  {
    auto cubie = (state >> (i * 3)) & 7;
    sumCubies += cubie;
    sumSquaresCubies += (cubie * cubie);
  }
  auto remainingSum = (0+1+2+3+4+5+6+7) - sumCubies;
  auto remainingSumSquares = (0+1+4+9+16+25+36+49) - sumSquaresCubies;
  // S = x + y
  // Q = x^2 + y^2
  // Q = x^2 + (S-x)^2
  // Q = x^2 + S^2 - 2Sx + x^2
  // 2x^2 - 2Sx + (S^2 - Q) = 0
  // x = {2S +- SQRT(4S^2 - 8[S^2 - Q])} / 4
  // x = {S +- SQRT(2Q - S^2)} / 2
  auto rootDelta = IntegerSquareRoot(2 * remainingSumSquares - remainingSum * remainingSum);
  std::array<std::uint32_t, 2> remainingCubies = { (remainingSum + rootDelta) / 2, (remainingSum - rootDelta) / 2 };
  return remainingCubies[(positionIndex == 6) ^ ((state >> 18) & 1)];
}

std::uint32_t Cube2x2x2::GetOrientation(std::uint32_t positionIndex) const
{
  auto orientationState = state >> 19;
  if (positionIndex < 7)
    // Can get orientation directly.
    return orientationState / powersOf3[positionIndex] % 3;

  // For the last cubie we have to infer the orientation from all the other orientations.
  auto cumulativeOrientation = 0;
  for (auto i = 0; i < 7; ++i)
  {
    cumulativeOrientation += (orientationState % 3);
    orientationState /= 3;
  }
  return (3 - (cumulativeOrientation % 3)) % 3;
}

void Cube2x2x2::SetCubie(std::uint32_t positionIndex, std::uint32_t cubie)
{
  switch (positionIndex)
  {
    case 0:
      state = (state & 0xfffffff8) | cubie;
      return;

    case 1:
      state = (state & 0xffffffc7) | (cubie << 3);
      return;

    case 2:
      state = (state & 0xfffffe3f) | (cubie << 6);
      return;

    case 3:
      state = (state & 0xfffff1ff) | (cubie << 9);
      return;

    case 4:
      state = (state & 0xffff8fff) | (cubie << 12);
      return;

    case 5:
      state = (state & 0xfffc7fff) | (cubie << 15);
      return;

    case 6:
      {
        // Check which cubies are in earlier positions.
        std::uint32_t sumCubies = 0;
        for (int i = 0; i < 6; ++i)
        {
          auto cubie = (state >> (i * 3)) & 7;
          sumCubies += cubie;
        }
        auto remainingSum = (0 + 1 + 2 + 3 + 4 + 5 + 6 + 7) - sumCubies;
        auto cubieIn7 = remainingSum - cubie;
        state = (state & 0xfffbffff) | (cubie < cubieIn7 ? 0 : (1 << 18));
      }
      return;

    default:
      {
        // The cubie in position 7 is not stored in state; check if we need to change cubie 6 though.
        auto current6 = GetCubie(6);
        auto current7 = GetCubie(7);
        if (cubie <= current6 && current6 < current7)
        {
          state = (state & 0xfffbffff) | (1 << 18);
        }
        else if (current7 < current6 && current6 <= cubie)
        {
          state = (state & 0xfffbffff);
        }
      }
      return;
  }
}

void Cube2x2x2::SetOrientation(std::uint32_t positionIndex, std::uint32_t orientation)
{
  if (positionIndex == 7)
    // The cubie in position 7 does not store its orientation in state
    return;

  auto oldOrientationState = state >> 19;
  auto earlierOrientations = oldOrientationState % powersOf3[positionIndex];
  auto laterOrientations = oldOrientationState / powersOf3[positionIndex + 1];
  auto newOrientationState = (laterOrientations * 3 + orientation) * powersOf3[positionIndex] + earlierOrientations;
  state = (state & 0x0007ffff) | (newOrientationState << 19);
}