#include "Corner3Cycle.h"

#include <map>
#include <string>
#include <stdexcept>

#include "Cube\3x3x3\CornerStructure.h"

#include "Solution\SolutionStep.h"

namespace Corner3Cycle {

static const std::array<StickerPosition, 8> allCorners
{
  StickerPosition{ Face::Down, 0 },
  StickerPosition{ Face::Down, 2 },
  StickerPosition{ Face::Down, 6 },
  StickerPosition{ Face::Down, 8 },
  StickerPosition{ Face::Up, 0 },
  StickerPosition{ Face::Up, 2 },
  StickerPosition{ Face::Up, 6 },
  StickerPosition{ Face::Up, 8 }
};

// Returns the first corner 3-cycle if one is present on the cube.
// Throws an exception if there are no corner 3-cycles.
std::array<StickerPosition, 3> FindCorner3Cycle(const Cube3x3x3& cube)
{
  for (const auto& startCorner : allCorners)
  {
    auto cycle = CornerStructure::GetCornerCycle(cube, startCorner);
    if (cycle.size() == 3 && !CornerStructure::AreStickersOnOneOfCubies(cycle[0], cycle.begin() + 1, cycle.end()))
    {
      return std::array<StickerPosition, 3>{ cycle[0], cycle[1], cycle[2] };
    }
  }
  throw std::domain_error("No corner 3-cycle in given cube");
}

std::vector<CubeMove> SolveThreeCorners(const std::array<StickerPosition, 3>& cornerStickers)
{
  std::vector<std::array<Face, 3>> corners;
  for (auto sticker : cornerStickers)
  {
    corners.push_back(CornerStructure::StickerPositionToCorner(sticker));
  }

  // We do a face remapping such that our first corner is at URB.
  std::map<Face, Face> algorithmToReal;
  algorithmToReal[Face::Up] = corners[0][0];
  algorithmToReal[Face::Back] = corners[0][1];
  algorithmToReal[Face::Right] = corners[0][2];
  algorithmToReal[Face::Down] = GetOppositeFace(corners[0][0]);
  algorithmToReal[Face::Front] = GetOppositeFace(corners[0][1]);
  algorithmToReal[Face::Left] = GetOppositeFace(corners[0][2]);
  std::map<Face, Face> realToAlgorithm;
  for (auto& facePair : algorithmToReal)
  {
    realToAlgorithm[facePair.second] = facePair.first;
  }

  // We translate all three corners into the algorithm faces and represent this
  // by a text description of the corner 3-cycle, e.g. (URB UBL ULF)
  std::ostringstream cycleStream;
  cycleStream << '(';
  for (size_t i = 0; i < corners.size(); ++i)
  {
    if (i != 0)
      cycleStream << ' ';
    cycleStream << GetFaceChar(realToAlgorithm[corners[i][0]])
                << GetFaceChar(realToAlgorithm[corners[i][2]])
                << GetFaceChar(realToAlgorithm[corners[i][1]]);
  }
  cycleStream << ')';
  auto cycle = cycleStream.str();

  // We can now look up the solution by this cycle.
  static const std::map<std::string, std::string> cycleAlgorithms
  { // Source of all algorithms: http://www.speedcubing.com/chris/bhcorners.html
    { "(URB UBL ULF)", "L F' L B2 L' F L B2 L2" }, // 	A9
    { "(URB UBL UFR)", "R2 B2 R F R' B2 R F' R" }, // 	A9
    { "(URB UBL FUL)", "L F R' F' L' F R F'" }, // 	Direct Insert
    { "(URB UBL FRU)", "R B L B' R' B L' B'" }, // 	Direct Insert
    { "(URB UBL FLD)", "U' B D' B' U B D B'" }, // 	Drop and Catch
    { "(URB UBL FDR)", "U' L' D2 L U L' D2 L" }, // 	Drop and Catch
    { "(URB UBL RUF)", "B U2 B D B' U2 B D' B2" }, // 	A9
    { "(URB UBL RFD)", "U' B D2 B' U B D2 B'" }, // 	Drop and Catch
    { "(URB UBL RDB)", "U' L' D L U L' D' L" }, // 	Drop and Catch
    { "(URB UBL BRD)", "R' B L B' R B L' B'" }, // 	Direct Insert
    { "(URB UBL BDL)", "U' B D B' U B D' B'" }, // 	Direct Insert
    { "(URB UBL LFU)", "B2 D' B U2 B' D B U2 B" }, // 	A9
    { "(URB UBL LBD)", "U' L' D' L U L' D L" }, // 	Direct Insert
    { "(URB UBL LDF)", "B' D2 B U B' D2 B U'" }, // 	Drop and Catch
    { "(URB UBL DFL)", "L2 D R2 D' L2 D R2 D'" }, // 	Direct Insert
    { "(URB UBL DRF)", "R2 B L B' R2 B L' B'" }, // 	Direct Insert
    { "(URB UBL DLB)", "D L2 D R2 D' L2 D R2 D2" }, // 	A9
    { "(URB UBL DBR)", "D2 L2 D R2 D' L2 D R2 D" }, // 	A9
    { "(URB ULF UBL)", "L2 B2 L' F' L B2 L' F L'" }, // 	A9
    { "(URB ULF UFR)", "R B' R F2 R' B R F2 R2" }, // 	A9
    { "(URB ULF FRU)", "L2 B2 L' F2 L B2 L' F2 L'" }, // 	A9
    { "(URB ULF FLD)", "U2 F' D' F U2 F' D F" }, // 	Direct Insert
    { "(URB ULF FDR)", "B' D B U2 B' D' B U2" }, // 	Drop and Catch
    { "(URB ULF RUF)", "B' R2 B' L2 B R2 B' L2 B2" }, // 	A9
    { "(URB ULF RFD)", "U2 L D' L' U2 L D L'" }, // 	Drop and Catch
    { "(URB ULF RDB)", "U2 F' D2 F U2 F' D2 F" }, // 	Drop and Catch
    { "(URB ULF BLU)", "R B2 R F2 R' B2 R F2 R2" }, // 	A9
    { "(URB ULF BRD)", "U2 L D2 L' U2 L D2 L'" }, // 	Drop and Catch
    { "(URB ULF BDL)", "U2 F' D F U2 F' D' F" }, // 	Drop and Catch
    { "(URB ULF LUB)", "F2 R2 F L2 F' R2 F L2 F" }, // 	A9
    { "(URB ULF LBD)", "R D' R' U2 R D R' U2" }, // 	Drop and Catch
    { "(URB ULF LDF)", "U2 L D L' U2 L D' L'" }, // 	Direct Insert
    { "(URB ULF DFL)", "R U2 R' U' L2 U R U' L2 U' R'" }, // 	Columns
    { "(URB ULF DRF)", "U F2 U' F2 U' R2 U F2 U F2 U' R2" }, // 	Per Special
    { "(URB ULF DLB)", "U2 F U2 F' L2 F' L2 F L2 B L2 B'" }, // 	Per Special
    { "(URB ULF DBR)", "R B R2 B L' B' R2 B L B2 R'" }, // 	Columns
    { "(URB UFR UBL)", "R' F R' B2 R F' R' B2 R2" }, // 	A9
    { "(URB UFR ULF)", "R2 F2 R' B' R F2 R' B R'" }, // 	A9
    { "(URB UFR FUL)", "R2 D R' U2 R D' R' U2 R'" }, // 	A9
    { "(URB UFR FLD)", "R D2 R' U' R D2 R' U" }, // 	Drop and Catch
    { "(URB UFR FDR)", "U F D F' U' F D' F'" }, // 	Direct Insert
    { "(URB UFR RFD)", "U R' D' R U' R' D R" }, // 	Direct Insert
    { "(URB UFR RDB)", "R D R' U' R D' R' U" }, // 	Direct Insert
    { "(URB UFR BLU)", "R' U2 R' D' R U2 R' D R2" }, // 	A9
    { "(URB UFR BRD)", "U F D' F' U' F D F'" }, // 	Drop and Catch
    { "(URB UFR BDL)", "U R' D2 R U' R' D2 R" }, // 	Drop and Catch
    { "(URB UFR LUB)", "L F' L' B L F L' B'" }, // 	Direct Insert
    { "(URB UFR LFU)", "R B R' F R B' R' F'" }, // 	Direct Insert
    { "(URB UFR LBD)", "U F D2 F' U' F D2 F'" }, // 	Drop and Catch
    { "(URB UFR LDF)", "U R' D R U' R' D' R" }, // 	Drop and Catch
    { "(URB UFR DFL)", "R B R' F2 R B' R' F2" }, // 	Direct Insert
    { "(URB UFR DRF)", "D' F2 D' B2 D F2 D' B2 D2" }, // 	A9
    { "(URB UFR DLB)", "D F2 D' B2 D F2 D' B2" }, // 	Toss Up
    { "(URB UFR DBR)", "D2 F2 D' B2 D F2 D' B2 D'" }, // 	A9
    { "(URB FUL UBL)", "F R' F' L F R F' L'" }, // 	Drop and Catch
    { "(URB FUL UFR)", "R U2 R D R' U2 R D' R2" }, // 	A9
    { "(URB FUL FRU)", "U L U' R' U L' U' R" }, // 	Direct Insert
    { "(URB FUL FLD)", "F' L B2 L' F L B2 L'" }, // 	Drop and Catch
    { "(URB FUL FDR)", "F2 D B' D' F2 D B D'" }, // 	Drop and Catch
    { "(URB FUL RUF)", "F2 D' F U2 F' D F U2 F" }, // 	A9
    { "(URB FUL RFD)", "U R' B2 R F R' B2 R F' U'" }, // 	Orthogonals
    { "(URB FUL RDB)", "D' F2 D B' D' F2 D B" }, // 	Toss Up
    { "(URB FUL BLU)", "F R' U2 R F' R' F U2 F' R" }, // 	Cyclic Shift
    { "(URB FUL BRD)", "B' L2 B R B' L2 B R'" }, // 	Drop and Catch
    { "(URB FUL BDL)", "F R' F' L2 F R F' L2" }, // 	Drop and Catch
    { "(URB FUL LUB)", "L F L' B L F' L' B'" }, // 	Direct Insert
    { "(URB FUL LBD)", "U L' B R2 B' L B R2 B' U'" }, // 	Orthogonals
    { "(URB FUL LDF)", "D F2 D B' D' F2 D B D2" }, // 	A9
    { "(URB FUL DFL)", "F R' F' L' F R F' L" }, // 	Drop and Catch
    { "(URB FUL DRF)", "D' L D R2 D' L' D R2" }, // 	Drop and Catch
    { "(URB FUL DLB)", "B2 R' F R B2 R' F' R" }, // 	Drop and Catch
    { "(URB FUL DBR)", "D2 L D R2 D' L' D R2 D" }, // 	A9
    { "(URB FRU UBL)", "R' F' L F R F' L' F" }, // 	Drop and Catch
    { "(URB FRU ULF)", "L F2 L B2 L' F2 L B2 L2" }, // 	A9
    { "(URB FRU FUL)", "U' B' U F U' B U F'" }, // 	Direct Insert
    { "(URB FRU FLD)", "U' B' U F2 U' B U F2" }, // 	Direct Insert
    { "(URB FRU FDR)", "U' B' U F' U' B U F" }, // 	Direct Insert
    { "(URB FRU RFD)", "U2 L' U R2 U' L U R2 U" }, // 	A9
    { "(URB FRU RDB)", "D' F D B' D' F' D B" }, // 	Drop and Catch
    { "(URB FRU BLU)", "L2 F2 L B2 L' F2 L B2 L" }, // 	A9
    { "(URB FRU BRD)", "U F' U B2 U' F U B2 U2" }, // 	A9
    { "(URB FRU BDL)", "U' L U R U' L' U R'" }, // 	Drop and Catch
    { "(URB FRU LUB)", "U' L' U R U' L U R'" }, // 	Direct Insert
    { "(URB FRU LFU)", "R' F' L' F R F' L F" }, // 	Direct Insert
    { "(URB FRU LBD)", "R' U L2 U' R U L2 U'" }, // 	Drop and Catch
    { "(URB FRU LDF)", "R' U L' U' R U L U'" }, // 	Drop and Catch
    { "(URB FRU DFL)", "U' L2 U R U' L2 U R'" }, // 	Drop and Catch
    { "(URB FRU DRF)", "U2 F2 U B U' F2 U B' U" }, // 	A9
    { "(URB FRU DLB)", "L' F2 L B2 L' F2 L B2" }, // 	Toss Up
    { "(URB FRU DBR)", "U R2 U L U' R2 U L' U2" }, // 	A9
    { "(URB FLD UBL)", "U R D2 R' U' R D2 R'" }, // 	Drop and Catch
    { "(URB FLD ULF)", "U2 R D2 R' U2 R D2 R'" }, // 	Drop and Catch
    { "(URB FLD UFR)", "U' R D2 R' U R D2 R'" }, // 	Drop and Catch
    { "(URB FLD FUL)", "L B2 L' F' L B2 L' F" }, // 	Drop and Catch
    { "(URB FLD FRU)", "F2 U' B' U F2 U' B U" }, // 	Direct Insert
    { "(URB FLD FDR)", "F' D B' D' F D B D'" }, // 	Drop and Catch
    { "(URB FLD RUF)", "B' R2 B' L B R2 B' L' B2" }, // 	A9
    { "(URB FLD RFD)", "D F' U F D' F' U' F" }, // 	Toss Up
    { "(URB FLD RDB)", "B R' F2 R B' R' F2 R" }, // 	Drop and Catch
    { "(URB FLD BLU)", "U B2 D' B' U2 B D B' U2 B' U'" }, // 	Columns
    { "(URB FLD BRD)", "D2 B U B' D2 B U' B'" }, // 	Direct Insert
    { "(URB FLD BDL)", "F2 R' F' L2 F R F' L2 F'" }, // 	A9
    { "(URB FLD LUB)", "U' F U B U' F' U B'" }, // 	Drop and Catch
    { "(URB FLD LFU)", "D2 R D L2 D' R' D L2 D" }, // 	A9
    { "(URB FLD LBD)", "L' U2 L D L' U2 L D'" }, // 	Toss Up
    { "(URB FLD DRF)", "U L' U F2 U' L U L' F2 L U2" }, // 	Columns
    { "(URB FLD DLB)", "D F D' B2 D F' D' B2" }, // 	Direct Insert
    { "(URB FLD DBR)", "U R2 U L' U' R2 U L U2" }, // 	A9
    { "(URB FDR UBL)", "U B' D B U' B' D' B" }, // 	Drop and Catch
    { "(URB FDR ULF)", "U2 B' D B U2 B' D' B" }, // 	Drop and Catch
    { "(URB FDR UFR)", "U' B' D B U B' D' B" }, // 	Drop and Catch
    { "(URB FDR FUL)", "D B' D' F2 D B D' F2" }, // 	Drop and Catch
    { "(URB FDR FRU)", "F' U' B' U F U' B U" }, // 	Direct Insert
    { "(URB FDR FLD)", "F L B2 L' F' L B2 L'" }, // 	Drop and Catch
    { "(URB FDR RUF)", "D B' R2 B D' B' D R2 D' B" }, // 	Cyclic Shift
    { "(URB FDR RDB)", "R F R' B' R F' R' B" }, // 	Direct Insert
    { "(URB FDR BLU)", "L2 F L B2 L' F' L B2 L" }, // 	A9
    { "(URB FDR BRD)", "B R2 B L B' R2 B L' B2" }, // 	A9
    { "(URB FDR BDL)", "D2 L U' L' D2 L U L'" }, // 	Toss Up
    { "(URB FDR LUB)", "U' F2 U B U' F2 U B'" }, // 	Drop and Catch
    { "(URB FDR LFU)", "U R' F L2 F' R F L2 F' U'" }, // 	Orthogonals
    { "(URB FDR LBD)", "U F' L2 F R F' L2 F R' U'" }, // 	Orthogonals
    { "(URB FDR LDF)", "D' F U2 F' D F U2 F'" }, // 	Toss Up
    { "(URB FDR DFL)", "L D2 L U' L' D2 L U L2" }, // 	A9
    { "(URB FDR DLB)", "L' F L B2 L' F' L B2" }, // 	Drop and Catch
    { "(URB FDR DBR)", "D2 L' D R2 D' L D R2 D" }, // 	A9
    { "(URB RUF UBL)", "B2 D B' U2 B D' B' U2 B'" }, // 	A9
    { "(URB RUF ULF)", "B2 L2 B R2 B' L2 B R2 B" }, // 	A9
    { "(URB RUF FUL)", "F' U2 F' D' F U2 F' D F2" }, // 	A9
    { "(URB RUF FLD)", "B2 L B R2 B' L' B R2 B" }, // 	A9
    { "(URB RUF FDR)", "B' D R2 D' B D B' R2 B D'" }, // 	Cyclic Shift
    { "(URB RUF RFD)", "F R2 F L F' R2 F L' F2" }, // 	A9
    { "(URB RUF RDB)", "F' U2 F' D2 F U2 F' D2 F2" }, // 	A9
    { "(URB RUF BLU)", "L F' U2 F L' F' L U2 L' F" }, // 	Cyclic Shift
    { "(URB RUF BRD)", "B2 L' B R2 B' L B R2 B" }, // 	A9
    { "(URB RUF BDL)", "F' U2 F' D F U2 F' D' F2" }, // 	A9
    { "(URB RUF LUB)", "F R2 F L2 F' R2 F L2 F2" }, // 	A9
    { "(URB RUF LFU)", "B L' U2 L B' L' B U2 B' L" }, // 	Cyclic Shift
    { "(URB RUF LBD)", "F R2 F L' F' R2 F L F2" }, // 	A9
    { "(URB RUF LDF)", "U2 B' U2 B' D2 B U2 B' D2 B2 U2" }, // 	Columns
    { "(URB RUF DFL)", "B2 D' B' U2 B D B' U2 B'" }, // 	A9
    { "(URB RUF DRF)", "B2 D2 B' U2 B D2 B' U2 B'" }, // 	A9
    { "(URB RUF DLB)", "U R2 D' R' U2 R D R' U2 R' U'" }, // 	Columns
    { "(URB RUF DBR)", "D' F R2 F' D F D' R2 D F'" }, // 	Cyclic Shift
    { "(URB RFD UBL)", "B D2 B' U' B D2 B' U" }, // 	Drop and Catch
    { "(URB RFD ULF)", "L D' L' U2 L D L' U2" }, // 	Drop and Catch
    { "(URB RFD UFR)", "R' D' R U R' D R U'" }, // 	Direct Insert
    { "(URB RFD FUL)", "U F R' B2 R F' R' B2 R U'" }, // 	Orthogonals
    { "(URB RFD FRU)", "U' R2 U' L' U R2 U' L U2" }, // 	A9
    { "(URB RFD FLD)", "F' U F D F' U' F D'" }, // 	Toss Up
    { "(URB RFD RUF)", "F2 L F' R2 F L' F' R2 F'" }, // 	A9
    { "(URB RFD RDB)", "D' F' D B' D' F D B" }, // 	Direct Insert
    { "(URB RFD BLU)", "L2 D' L' U2 L D L' U2 L'" }, // 	A9
    { "(URB RFD BRD)", "F' U F D' F' U' F D" }, // 	Toss Up
    { "(URB RFD BDL)", "U R B2 R' F' R B2 R' F U'" }, // 	Orthogonals
    { "(URB RFD LUB)", "L F2 L' B L F2 L' B'" }, // 	Drop and Catch
    { "(URB RFD LFU)", "F2 L' B L F2 L' B' L" }, // 	Drop and Catch
    { "(URB RFD LBD)", "F' U F D2 F' U' F D2" }, // 	Toss Up
    { "(URB RFD LDF)", "L' F2 L' B L F2 L' B' L2" }, // 	A9
    { "(URB RFD DFL)", "F D' B2 D F' D' B2 D" }, // 	Drop and Catch
    { "(URB RFD DLB)", "B2 U F' U' B2 U F U'" }, // 	Drop and Catch
    { "(URB RFD DBR)", "F' U R2 U' F U F' R2 F U'" }, // 	Cyclic Shift
    { "(URB RDB UBL)", "U R D R' U' R D' R'" }, // 	Direct Insert
    { "(URB RDB ULF)", "U2 R D R' U2 R D' R'" }, // 	Direct Insert
    { "(URB RDB UFR)", "U' R D R' U R D' R'" }, // 	Direct Insert
    { "(URB RDB FUL)", "R' F R B R' F' R B'" }, // 	Drop and Catch
    { "(URB RDB FRU)", "U F U' B U F' U' B'" }, // 	Direct Insert
    { "(URB RDB FLD)", "R' F2 R B R' F2 R B'" }, // 	Drop and Catch
    { "(URB RDB FDR)", "R' U' R D R' U R D'" }, // 	Direct Insert
    { "(URB RDB RUF)", "F2 D2 F U2 F' D2 F U2 F" }, // 	A9
    { "(URB RDB RFD)", "U F' U' B U F U' B'" }, // 	Toss Up
    { "(URB RDB BLU)", "R B2 R F R' B2 R F' R2" }, // 	A9
    { "(URB RDB BDL)", "R' U' R D' R' U R D" }, // 	Direct Insert
    { "(URB RDB LUB)", "U2 R2 U L U' R2 U L' U" }, // 	A9
    { "(URB RDB LFU)", "B' R F2 R' B R F2 R'" }, // 	Drop and Catch
    { "(URB RDB LBD)", "R2 F' R B2 R' F R B2 R" }, // 	A9
    { "(URB RDB LDF)", "R' U' R D2 R' U R D2" }, // 	Direct Insert
    { "(URB RDB DFL)", "U F2 U' B U F2 U' B'" }, // 	Drop and Catch
    { "(URB RDB DRF)", "F D2 F U2 F' D2 F U2 F2" }, // 	A9
    { "(URB RDB DLB)", "R2 D2 R U R' D2 R U' R" }, // 	A9
    { "(URB BLU ULF)", "R2 F2 R' B2 R F2 R' B2 R'" }, // 	A9
    { "(URB BLU UFR)", "R2 D' R U2 R' D R U2 R" }, // 	A9
    { "(URB BLU FUL)", "R' F U2 F' R F R' U2 R F'" }, // 	Cyclic Shift
    { "(URB BLU FRU)", "L' B2 L' F2 L B2 L' F2 L2" }, // 	A9
    { "(URB BLU FLD)", "U B U2 B D' B' U2 B D B2 U'" }, // 	Columns
    { "(URB BLU FDR)", "L' B2 L' F L B2 L' F' L2" }, // 	A9
    { "(URB BLU RUF)", "F' L U2 L' F L F' U2 F L'" }, // 	Cyclic Shift
    { "(URB BLU RFD)", "L U2 L D' L' U2 L D L2" }, // 	A9
    { "(URB BLU RDB)", "R2 F R' B2 R F' R' B2 R'" }, // 	A9
    { "(URB BLU BRD)", "L U2 L D2 L' U2 L D2 L2" }, // 	A9
    { "(URB BLU BDL)", "L' B2 L' F' L B2 L' F L2" }, // 	A9
    { "(URB BLU LFU)", "L U2 L D L' U2 L D' L2" }, // 	A9
    { "(URB BLU LBD)", "R D' B2 D R' D' R B2 R' D" }, // 	Cyclic Shift
    { "(URB BLU LDF)", "R2 F' R' B2 R F R' B2 R'" }, // 	A9
    { "(URB BLU DFL)", "R2 D R U2 R' D' R U2 R" }, // 	A9
    { "(URB BLU DRF)", "U2 L2 D2 L U2 L' D2 L U2 L U2" }, // 	Columns
    { "(URB BLU DLB)", "R2 D2 R U2 R' D2 R U2 R" }, // 	A9
    { "(URB BLU DBR)", "D L' B2 L D' L' D B2 D' L" }, // 	Cyclic Shift
    { "(URB BRD UBL)", "U B' D' B U' B' D B" }, // 	Direct Insert
    { "(URB BRD ULF)", "U2 B' D' B U2 B' D B" }, // 	Direct Insert
    { "(URB BRD UFR)", "U' B' D' B U B' D B" }, // 	Direct Insert
    { "(URB BRD FUL)", "R B' L2 B R' B' L2 B" }, // 	Drop and Catch
    { "(URB BRD FRU)", "U2 B2 U' F' U B2 U' F U'" }, // 	A9
    { "(URB BRD FLD)", "D2 L' U2 L D2 L' U2 L" }, // 	Toss Up
    { "(URB BRD FDR)", "B2 L B' R2 B L' B' R2 B'" }, // 	A9
    { "(URB BRD RUF)", "B' R2 B' L' B R2 B' L B2" }, // 	A9
    { "(URB BRD RFD)", "D' F' U F D F' U' F" }, // 	Toss Up
    { "(URB BRD BLU)", "L2 D2 L' U2 L D2 L' U2 L'" }, // 	A9
    { "(URB BRD BDL)", "U' L U R' U' L' U R" }, // 	Drop and Catch
    { "(URB BRD LUB)", "U' L' U R' U' L U R" }, // 	Direct Insert
    { "(URB BRD LFU)", "R D L2 D' R' D L2 D'" }, // 	Drop and Catch
    { "(URB BRD LBD)", "R B' L' B R' B' L B" }, // 	Direct Insert
    { "(URB BRD LDF)", "B L2 B' R' B L2 B' R" }, // 	Drop and Catch
    { "(URB BRD DFL)", "U' L2 U R' U' L2 U R" }, // 	Drop and Catch
    { "(URB BRD DRF)", "D' F D' B2 D F' D' B2 D2" }, // 	A9
    { "(URB BRD DLB)", "L' D2 L' U2 L D2 L' U2 L2" }, // 	A9
    { "(URB BDL UBL)", "L B' R' B L' B' R B" }, // 	Direct Insert
    { "(URB BDL ULF)", "F' D F U2 F' D' F U2" }, // 	Drop and Catch
    { "(URB BDL UFR)", "R' D2 R U R' D2 R U'" }, // 	Drop and Catch
    { "(URB BDL FUL)", "L2 F R' F' L2 F R F'" }, // 	Drop and Catch
    { "(URB BDL FRU)", "R U' L U R' U' L' U" }, // 	Drop and Catch
    { "(URB BDL FLD)", "F L2 F R' F' L2 F R F2" }, // 	A9
    { "(URB BDL FDR)", "L U' L' D2 L U L' D2" }, // 	Toss Up
    { "(URB BDL RUF)", "F2 D F U2 F' D' F U2 F" }, // 	A9
    { "(URB BDL RFD)", "U F' R B2 R' F R B2 R' U'" }, // 	Orthogonals
    { "(URB BDL RDB)", "D' R' U' R D R' U R" }, // 	Direct Insert
    { "(URB BDL BLU)", "L2 F' L B2 L' F L B2 L" }, // 	A9
    { "(URB BDL BRD)", "R' U' L U R U' L' U" }, // 	Drop and Catch
    { "(URB BDL LUB)", "U B2 U F U' B2 U F' U2" }, // 	A9
    { "(URB BDL LFU)", "U L' F2 L B L' F2 L B' U'" }, // 	Orthogonals
    { "(URB BDL LDF)", "D F U2 F' D' F U2 F'" }, // 	Toss Up
    { "(URB BDL DFL)", "L' D R2 D' L D R2 D'" }, // 	Drop and Catch
    { "(URB BDL DRF)", "R2 U' L U R2 U' L' U" }, // 	Drop and Catch
    { "(URB BDL DBR)", "L U' B2 U L' U' L B2 L' U" }, // 	Cyclic Shift
    { "(URB LUB ULF)", "F' L2 F' R2 F L2 F' R2 F2" }, // 	A9
    { "(URB LUB UFR)", "R' F' R B' R' F R B" }, // 	Direct Insert
    { "(URB LUB FUL)", "R' F R B' R' F' R B" }, // 	Toss Up
    { "(URB LUB FRU)", "U F U' B' U F' U' B" }, // 	Direct Insert
    { "(URB LUB FLD)", "R' F2 R B' R' F2 R B" }, // 	Drop and Catch
    { "(URB LUB FDR)", "B U' F2 U B' U' F2 U" }, // 	Drop and Catch
    { "(URB LUB RUF)", "F2 L2 F' R2 F L2 F' R2 F'" }, // 	A9
    { "(URB LUB RFD)", "U F' U' B' U F U' B" }, // 	Drop and Catch
    { "(URB LUB RDB)", "U' L U' R2 U L' U' R2 U2" }, // 	A9
    { "(URB LUB BRD)", "R' U' L' U R U' L U" }, // 	Direct Insert
    { "(URB LUB BDL)", "U2 F U' B2 U F' U' B2 U'" }, // 	A9
    { "(URB LUB LFU)", "U R U' L' U R' U' L" }, // 	Direct Insert
    { "(URB LUB LBD)", "U R U' L U R' U' L'" }, // 	Direct Insert
    { "(URB LUB LDF)", "U R U' L2 U R' U' L2" }, // 	Direct Insert
    { "(URB LUB DFL)", "U F2 U' B' U F2 U' B" }, // 	Drop and Catch
    { "(URB LUB DRF)", "R2 U' L' U R2 U' L U" }, // 	Direct Insert
    { "(URB LUB DLB)", "U2 L2 U' R' U L2 U' R U'" }, // 	A9
    { "(URB LUB DBR)", "U' B2 U' F' U B2 U' F U2" }, // 	A9
    { "(URB LFU UBL)", "B' U2 B' D' B U2 B' D B2" }, // 	A9
    { "(URB LFU UFR)", "F R B R' F' R B' R'" }, // 	Direct Insert
    { "(URB LFU FRU)", "R B L' B' R' B L B'" }, // 	Drop and Catch
    { "(URB LFU FLD)", "D' L2 D' R D L2 D' R' D2" }, // 	A9
    { "(URB LFU FDR)", "U F L2 F' R' F L2 F' R U'" }, // 	Orthogonals
    { "(URB LFU RUF)", "L' B U2 B' L B L' U2 L B'" }, // 	Cyclic Shift
    { "(URB LFU RFD)", "L' B L F2 L' B' L F2" }, // 	Drop and Catch
    { "(URB LFU RDB)", "R F2 R' B' R F2 R' B" }, // 	Drop and Catch
    { "(URB LFU BLU)", "L2 D L' U2 L D' L' U2 L'" }, // 	A9
    { "(URB LFU BRD)", "R' B L' B' R B L B'" }, // 	Drop and Catch
    { "(URB LFU BDL)", "U B L' F2 L B' L' F2 L U'" }, // 	Orthogonals
    { "(URB LFU LUB)", "U' F' U B U' F U B'" }, // 	Direct Insert
    { "(URB LFU LBD)", "L2 D' R D L2 D' R' D" }, // 	Drop and Catch
    { "(URB LFU LDF)", "L F' R2 F L' F' R2 F" }, // 	Drop and Catch
    { "(URB LFU DFL)", "F' D' B2 D F D' B2 D" }, // 	Drop and Catch
    { "(URB LFU DRF)", "R2 B L' B' R2 B L B'" }, // 	Drop and Catch
    { "(URB LFU DLB)", "D F' D' B2 D F D' B2" }, // 	Drop and Catch
    { "(URB LFU DBR)", "D2 F' D' B2 D F D' B2 D'" }, // 	A9
    { "(URB LBD UBL)", "U R D' R' U' R D R'" }, // 	Drop and Catch
    { "(URB LBD ULF)", "U2 R D' R' U2 R D R'" }, // 	Drop and Catch
    { "(URB LBD UFR)", "U' R D' R' U R D R'" }, // 	Drop and Catch
    { "(URB LBD FUL)", "U B R2 B' L' B R2 B' L U'" }, // 	Orthogonals
    { "(URB LBD FRU)", "U L2 U' R' U L2 U' R" }, // 	Drop and Catch
    { "(URB LBD FLD)", "D L' U2 L D' L' U2 L" }, // 	Toss Up
    { "(URB LBD FDR)", "U R F' L2 F R' F' L2 F U'" }, // 	Orthogonals
    { "(URB LBD RUF)", "F2 L' F' R2 F L F' R2 F'" }, // 	A9
    { "(URB LBD RFD)", "D2 F' U F D2 F' U' F" }, // 	Toss Up
    { "(URB LBD RDB)", "R' B2 R' F' R B2 R' F R2" }, // 	A9
    { "(URB LBD BLU)", "D' R B2 R' D R D' B2 D R'" }, // 	Cyclic Shift
    { "(URB LBD BRD)", "D' B U B' D B U' B'" }, // 	Direct Insert
    { "(URB LBD LUB)", "D' R D L' D' R' D L" }, // 	Drop and Catch
    { "(URB LBD LFU)", "D' R D L2 D' R' D L2" }, // 	Drop and Catch
    { "(URB LBD LDF)", "D' R D L D' R' D L'" }, // 	Drop and Catch
    { "(URB LBD DFL)", "F' D2 F' U F D2 F' U' F2" }, // 	A9
    { "(URB LBD DRF)", "F L' F' R2 F L F' R2" }, // 	Drop and Catch
    { "(URB LBD DBR)", "D2 F D' B2 D F' D' B2 D'" }, // 	A9
    { "(URB LDF UBL)", "U B' D2 B U' B' D2 B" }, // 	Drop and Catch
    { "(URB LDF ULF)", "U2 B' D2 B U2 B' D2 B" }, // 	Drop and Catch
    { "(URB LDF UFR)", "U' B' D2 B U B' D2 B" }, // 	Drop and Catch
    { "(URB LDF FUL)", "D2 B' D' F2 D B D' F2 D'" }, // 	A9
    { "(URB LDF FRU)", "U L' U' R' U L U' R" }, // 	Drop and Catch
    { "(URB LDF FDR)", "F U2 F' D' F U2 F' D" }, // 	Toss Up
    { "(URB LDF RUF)", "U2 B2 D2 B U2 B' D2 B U2 B U2" }, // 	Columns
    { "(URB LDF RFD)", "L2 B L F2 L' B' L F2 L" }, // 	A9
    { "(URB LDF RDB)", "F U2 F' D2 F U2 F' D2" }, // 	Toss Up
    { "(URB LDF BLU)", "R B2 R F' R' B2 R F R2" }, // 	A9
    { "(URB LDF BRD)", "R' B L2 B' R B L2 B'" }, // 	Drop and Catch
    { "(URB LDF BDL)", "F U2 F' D F U2 F' D'" }, // 	Toss Up
    { "(URB LDF LUB)", "F' R2 F L2 F' R2 F L2" }, // 	Toss Up
    { "(URB LDF LFU)", "F' R2 F L F' R2 F L'" }, // 	Drop and Catch
    { "(URB LDF LBD)", "F' R2 F L' F' R2 F L" }, // 	Drop and Catch
    { "(URB LDF DRF)", "R2 B L2 B' R2 B L2 B'" }, // 	Drop and Catch
    { "(URB LDF DLB)", "U B D2 B U' B' D2 B U B2 U'" }, // 	Columns
    { "(URB LDF DBR)", "U' B2 U' F U B2 U' F' U2" }, // 	A9
    { "(URB DFL UBL)", "D R2 D' L2 D R2 D' L2" }, // 	Toss Up
    { "(URB DFL ULF)", "R U L2 U R' U' L2 U R U2 R'" }, // 	Columns
    { "(URB DFL UFR)", "F2 R B R' F2 R B' R'" }, // 	Direct Insert
    { "(URB DFL FUL)", "D R2 D' L D R2 D' L'" }, // 	Drop and Catch
    { "(URB DFL FRU)", "R U' L2 U R' U' L2 U" }, // 	Drop and Catch
    { "(URB DFL FDR)", "L2 U' L' D2 L U L' D2 L'" }, // 	A9
    { "(URB DFL RUF)", "B U2 B D' B' U2 B D B2" }, // 	A9
    { "(URB DFL RFD)", "D' B2 D F D' B2 D F'" }, // 	Drop and Catch
    { "(URB DFL RDB)", "R F' R' B' R F R' B" }, // 	Drop and Catch
    { "(URB DFL BLU)", "R' U2 R' D R U2 R' D' R2" }, // 	A9
    { "(URB DFL BRD)", "R' U' L2 U R U' L2 U" }, // 	Drop and Catch
    { "(URB DFL BDL)", "D R2 D' L' D R2 D' L" }, // 	Drop and Catch
    { "(URB DFL LUB)", "B' U F2 U' B U F2 U'" }, // 	Drop and Catch
    { "(URB DFL LFU)", "F L' B L F' L' B' L" }, // 	Drop and Catch
    { "(URB DFL LBD)", "F2 U F D2 F' U' F D2 F" }, // 	A9
    { "(URB DFL DRF)", "R2 U' L2 U R2 U' L2 U" }, // 	Toss Up
    { "(URB DFL DLB)", "L' F' L B2 L' F L B2" }, // 	Direct Insert
    { "(URB DFL DBR)", "R2 U2 L2 U R2 U' L2 U R2 U R2" }, // 	Columns
    { "(URB DRF UBL)", "R2 D' L2 D R2 D' L2 D" }, // 	Toss Up
    { "(URB DRF ULF)", "U2 F R2 F' R2 F' U2 F R2 F R2 F'" }, // 	Per Special
    { "(URB DRF UFR)", "D2 B2 D F2 D' B2 D F2 D" }, // 	A9
    { "(URB DRF FUL)", "R2 D' L D R2 D' L' D" }, // 	Drop and Catch
    { "(URB DRF FRU)", "U' B U' F2 U B' U' F2 U2" }, // 	A9
    { "(URB DRF FLD)", "U2 R2 U2 R D2 R' U2 R D2 R U2" }, // 	Columns
    { "(URB DRF RUF)", "B U2 B D2 B' U2 B D2 B2" }, // 	A9
    { "(URB DRF RDB)", "F2 U2 F' D2 F U2 F' D2 F'" }, // 	A9
    { "(URB DRF BLU)", "U2 L' U2 L' D2 L U2 L' D2 L2 U2" }, // 	Columns
    { "(URB DRF BRD)", "D2 B2 D F D' B2 D F' D" }, // 	A9
    { "(URB DRF BDL)", "U' L U R2 U' L' U R2" }, // 	Drop and Catch
    { "(URB DRF LUB)", "U' L' U R2 U' L U R2" }, // 	Direct Insert
    { "(URB DRF LFU)", "B L' B' R2 B L B' R2" }, // 	Drop and Catch
    { "(URB DRF LBD)", "R2 F L' F' R2 F L F'" }, // 	Drop and Catch
    { "(URB DRF LDF)", "R2 D' L' D R2 D' L D" }, // 	Direct Insert
    { "(URB DRF DFL)", "U' L2 U R2 U' L2 U R2" }, // 	Toss Up
    { "(URB DRF DLB)", "U R2 U' B2 U' B2 U B2 D B2 D' R2" }, // 	Per Special
    { "(URB DRF DBR)", "U' B2 U' F2 U B2 U' F2 U2" }, // 	A9
    { "(URB DLB UBL)", "D2 R2 D' L2 D R2 D' L2 D'" }, // 	A9
    { "(URB DLB ULF)", "U B2 U' B2 U' L2 U B2 U B2 U' L2" }, // 	Per Special
    { "(URB DLB UFR)", "R' F' R B2 R' F R B2" }, // 	Direct Insert
    { "(URB DLB FUL)", "R' F R B2 R' F' R B2" }, // 	Drop and Catch
    { "(URB DLB FRU)", "U F U' B2 U F' U' B2" }, // 	Direct Insert
    { "(URB DLB FLD)", "R' F2 R B2 R' F2 R B2" }, // 	Toss Up
    { "(URB DLB FDR)", "B2 L' F L B2 L' F' L" }, // 	Drop and Catch
    { "(URB DLB RUF)", "U R U2 R D' R' U2 R D R2 U'" }, // 	Columns
    { "(URB DLB RFD)", "U F' U' B2 U F U' B2" }, // 	Drop and Catch
    { "(URB DLB RDB)", "R' U R' D2 R U' R' D2 R2" }, // 	A9
    { "(URB DLB BLU)", "R' U2 R' D2 R U2 R' D2 R2" }, // 	A9
    { "(URB DLB BRD)", "L2 U2 L D2 L' U2 L D2 L" }, // 	A9
    { "(URB DLB LUB)", "U R' U L2 U' R U L2 U2" }, // 	A9
    { "(URB DLB LFU)", "B2 D F' D' B2 D F D'" }, // 	Drop and Catch
    { "(URB DLB LDF)", "U B2 U' B' D2 B U B' D2 B' U'" }, // 	Columns
    { "(URB DLB DFL)", "U F2 U' B2 U F2 U' B2" }, // 	Toss Up
    { "(URB DLB DRF)", "U' B2 U R2 U R2 U' R2 D' R2 D B2" }, // 	Per Special
    { "(URB DLB DBR)", "U R2 U L2 U' R2 U L2 U2" }, // 	A9
    { "(URB DBR UBL)", "D' R2 D' L2 D R2 D' L2 D2" }, // 	A9
    { "(URB DBR ULF)", "R B2 L' B' R2 B L B' R2 B' R'" }, // 	Columns
    { "(URB DBR UFR)", "D B2 D F2 D' B2 D F2 D2" }, // 	A9
    { "(URB DBR FUL)", "D' R2 D' L D R2 D' L' D2" }, // 	A9
    { "(URB DBR FRU)", "U2 L U' R2 U L' U' R2 U'" }, // 	A9
    { "(URB DBR FLD)", "U2 L' U' R2 U L U' R2 U'" }, // 	A9
    { "(URB DBR FDR)", "D' R2 D' L' D R2 D' L D2" }, // 	A9
    { "(URB DBR RUF)", "F D' R2 D F' D' F R2 F' D" }, // 	Cyclic Shift
    { "(URB DBR RFD)", "U F' R2 F U' F' U R2 U' F" }, // 	Cyclic Shift
    { "(URB DBR BLU)", "L' D B2 D' L D L' B2 L D'" }, // 	Cyclic Shift
    { "(URB DBR BDL)", "U' L B2 L' U L U' B2 U L'" }, // 	Cyclic Shift
    { "(URB DBR LUB)", "U2 F' U B2 U' F U B2 U" }, // 	A9
    { "(URB DBR LFU)", "D B2 D F' D' B2 D F D2" }, // 	A9
    { "(URB DBR LBD)", "D B2 D F D' B2 D F' D2" }, // 	A9
    { "(URB DBR LDF)", "U2 F U B2 U' F' U B2 U" }, // 	A9
    { "(URB DBR DFL)", "R2 U' R2 U' L2 U R2 U' L2 U2 R2" }, // 	Columns
    { "(URB DBR DRF)", "U2 F2 U B2 U' F2 U B2 U" }, // 	A9
    { "(URB DBR DLB)", "U2 L2 U' R2 U L2 U' R2 U'" }, // 	A9    
  };
  const auto& algorithm = cycleAlgorithms.at(cycle);

  // Translate this algorithm back to our actual faces.
  auto moveSequence = ParseMoveSequence(algorithm);
  for (auto& move : moveSequence)
  {
    move.face = algorithmToReal[move.face];
  }

  return moveSequence;
}

Solution InsertCorner3CycleInSkeleton(const Solution& skeleton,
                                      const std::array<StickerPosition, 3>& skeletonCornerPositions)
{
  auto skeletonMoves = skeleton.GetMoves();
  auto inverseSkeleton = InvertMoveSequence(skeletonMoves);

  bool hasSolution = false;
  Solution bestSolution;

  for (size_t insertionIndex = 0; insertionIndex <= skeletonMoves.size(); ++insertionIndex)
  {
    Cube3x3x3 cube;
    cube += std::vector<CubeMove>(inverseSkeleton.begin(), inverseSkeleton.begin() + (skeletonMoves.size() - insertionIndex));

    // Where are our 3 corners now?
    std::array<StickerPosition, 3> corners
    {
      cube.Find(skeletonCornerPositions[0]),
      cube.Find(skeletonCornerPositions[1]),
      cube.Find(skeletonCornerPositions[2])
    };

    auto insertionSequence = SolveThreeCorners(corners);
    Solution candidateSolution = skeleton + SolutionStep{ "Corner cycle", insertionIndex, insertionSequence };
    if (hasSolution == false || candidateSolution.Length() < bestSolution.Length())
    {
      hasSolution = true;
      bestSolution = candidateSolution;
    };
  }

  return bestSolution;
}

Solution SolveCorner3Cycle(Cube3x3x3 scrambledCube, const Solution& skeleton)
{
  scrambledCube += skeleton.GetMoves();
  return InsertCorner3CycleInSkeleton(skeleton, FindCorner3Cycle(scrambledCube));
}

} // Corner3Cycle