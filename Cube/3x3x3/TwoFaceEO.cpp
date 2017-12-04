#include "TwoFaceEO.h"

#include <array>
#include <fstream>
#include <queue>
#include <stdexcept>
#include <unordered_set>

#include "..\BinaryStream.h"
#include "CubeStateSolver3x3x3Factory.h"

TwoFaceEO::TwoFaceEO(const std::string& cacheFileName, const std::string& cacheFileName2x2x2)
{
  if (!ReadCacheFile(cacheFileName))
  {
    GenerateSolutions(8, cacheFileName2x2x2);
    WriteCacheFile(cacheFileName);
  }
}

std::vector<CubeMove> TwoFaceEO::Solve(const Cube3x3x3& cube) const
{
  auto findIter = solutions.find(GetKeyValue(cube));
  if (findIter == solutions.end())
    throw std::runtime_error("No solution found for EO");
  return findIter->second;
}

std::vector<CubeMove> TwoFaceEO::Solve(const std::vector<CubeMove>& scramble) const
{
  Cube3x3x3 cube;
  cube += scramble;
  return Solve(cube);
}

void TwoFaceEO::GenerateSolutions(std::uint8_t maxMoves, const std::string& cacheFileName2x2x2)
{
  // Breadth-first search of move sequence space until all positions have solutions.
  
  // Add all positions that need solutions.
  std::unordered_set<std::uint8_t> positions;
  for (std::uint8_t key = 0; key <= 0x7f; ++key)
  {
    // Bit-level hack to get the bit parity from http://www.graphics.stanford.edu/~seander/bithacks.html#ParityParallel
    auto bits = key;
    bits ^= bits >> 4;
    bits &= 0xf;
    auto bitParity = (0x6996 >> bits) & 1;
    if (bitParity == 0)
      positions.insert(key);
  }

  // We need a block solver to tell us when our move sequences have grown so long
  // that we can't possible reach a 2-face case within the maximum moves specified.
  auto solver2x2x2 = CubeStateSolver3x3x3Factory::CreateSolver(cacheFileName2x2x2, CubeGroup::Scrambled, CubeGroup::Block2x2x2);

  // Now go through the move sequences.
  std::queue<std::vector<CubeMove>> scrambles;
  scrambles.push(std::vector<CubeMove>{});
  while (!positions.empty() && !scrambles.empty())
  {
    auto scramble = scrambles.front();
    scrambles.pop();
    Cube3x3x3 cube;
    cube += scramble;
    if (IsTwoFaceCase(cube))
    {
      auto key = GetKeyValue(cube);
      if (positions.find(key) != positions.end())
      { // We have found a scramble that reachs a position for which we don't yet have a solution.
        // A solution is just the inverse of the scramble.
        solutions.emplace(key, InvertMoveSequence(scramble));
        positions.erase(key);
      }
    }
    // Generate all successors to consider.
    for (size_t faceValue = 0; faceValue < 6; ++faceValue)
    {
      auto face = static_cast<Face>(faceValue);
      if (scramble.empty() || scramble.back().face != face)
      {
        for (int quarterRotationsClockwise = 1; quarterRotationsClockwise <= 3; ++quarterRotationsClockwise)
        {
          auto newScramble = scramble;
          newScramble.push_back(CubeMove{ face, quarterRotationsClockwise < 3 ? quarterRotationsClockwise : -1 });
          // Check that we can reach a 2-face case within the moves allowed.
          if (newScramble.size() + solver2x2x2->GetRequiredMoveCount(newScramble) <= maxMoves &&
              newScramble.size() + solver2x2x2->GetRequiredMoveCount(Rotate(newScramble, std::make_pair(Face::Front, Face::Down), std::make_pair(Face::Back, Face::Left))) <= maxMoves)
          {
            scrambles.push(std::move(newScramble));
          }
        }
      }
    }
  }
}

void TwoFaceEO::WriteCacheFile(const std::string& cacheFileName) const
{
  if (cacheFileName.empty())
    return;

  std::ofstream out(cacheFileName, std::ios::binary);
  for (const auto& solutionPair : solutions)
  {
    auto key = solutionPair.first;
    const auto& solution = solutionPair.second;

    BinaryStream::Write(out, key);
    BinaryStream::Write(out, static_cast<std::uint8_t>(solution.size()));
    for (const auto& move : solution)
    {
      BinaryStream::Write(out, static_cast<std::uint8_t>(move.face));
      BinaryStream::Write(out, static_cast<std::int8_t>(move.quarterRotationsClockwise));
    }
  }
}

bool TwoFaceEO::ReadCacheFile(const std::string& cacheFileName)
{
  if (cacheFileName.empty())
    return false;

  std::ifstream in(cacheFileName, std::ios::binary);
  if (!in.good())
    return false;

  while (in.good() && !in.eof())
  {
    auto key = BinaryStream::Read<std::uint8_t>(in);
    auto size = BinaryStream::Read<std::uint8_t>(in);
    std::vector<CubeMove> solution(size);
    for (std::uint8_t i = 0; i < size; ++i)
    {
      solution[i].face = static_cast<Face>(BinaryStream::Read<std::uint8_t>(in));
      solution[i].quarterRotationsClockwise = BinaryStream::Read<std::int8_t>(in);
    }
    solutions[key] = solution;
  }
  return true;
}

std::uint8_t TwoFaceEO::GetKeyValue(const Cube3x3x3& cube)
{
  static const std::array<StickerPosition, 4> upEdges
  { 
    StickerPosition{ Face::Up, 1 },
    StickerPosition{ Face::Up, 3 },
    StickerPosition{ Face::Up, 5 },
    StickerPosition{ Face::Up, 7 }
  };
  static const std::array<StickerPosition, 3> rightEdges
  {
    // Right-1 is missing because it's included in otherFaceOfUpEdges
    StickerPosition{ Face::Right, 3 },
    StickerPosition{ Face::Right, 5 },
    StickerPosition{ Face::Right, 7 }
  };

  static const std::array<StickerPosition, 4> otherFaceOfUpEdges
  {
    StickerPosition{ Face::Back, 1 },
    StickerPosition{ Face::Left, 1 },
    StickerPosition{ Face::Right, 1 },
    StickerPosition{ Face::Front, 1 }
  };
  static const std::array<StickerPosition, 3> otherFaceOfRightEdges
  {
    // Up-5 is missing becuase it's included in upEdges
    StickerPosition{ Face::Front, 5 },
    StickerPosition{ Face::Back, 3 },
    StickerPosition{ Face::Down, 5 }
  };

  // 7 edges, each:
  //  1 bit = 0 if oriented, 1 if misoriented
  std::uint8_t keyValue = 0;
  for (size_t i = 0; i < 4; ++i)
  {
    keyValue <<= 1;
    if (cube[upEdges[i]].face == Face::Right || cube[otherFaceOfUpEdges[i]].face == Face::Up)
      ++keyValue;
  }
  for (size_t i = 0; i < 3; ++i)
  {
    keyValue <<= 1;
    if (cube[rightEdges[i]].face == Face::Up || cube[otherFaceOfRightEdges[i]].face == Face::Right)
      ++keyValue;
  }
  return keyValue;
}

bool TwoFaceEO::IsTwoFaceCase(const Cube3x3x3& cube)
{
  static const std::array<StickerPosition, 7> cubiesToCheck
  {
    StickerPosition{ Face::Left, 3 },
    StickerPosition{ Face::Left, 5 },
    StickerPosition{ Face::Left, 6 },
    StickerPosition{ Face::Left, 7 },
    StickerPosition{ Face::Left, 8 },
    StickerPosition{ Face::Front, 7 },
    StickerPosition{ Face::Back, 7 }
  };
  for (const auto& cubie : cubiesToCheck)
    if (cube[cubie] != cubie)
      return false;
  return true;
}
