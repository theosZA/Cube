#include "SolverStep_F2LMinus1.h"

#include <array>

SolverStep_F2LMinus1::SolverStep_F2LMinus1(const std::string& cacheFileNameFrontSlot, const std::string& cacheFileNameBackSlot)
: solverF2LMinusBackSlot(20, cacheFileNameBackSlot),
  solverF2LMinusFrontSlot(20, cacheFileNameFrontSlot)
{}

std::vector<PartialSolution> SolverStep_F2LMinus1::Solve(const std::vector<CubeMove>& scramble, const Solution& solutionSoFar)
{
  std::vector<PartialSolution> partialSolutions;

  // There are 4 possible slots - FDR, BDR, FUL, BUL.
  // These consist of 2 possibilities on the R face (that can't be rotated to each other) and their rotations on the U face.
  // The rotation in both cases is the same - so 2 possible orientations.
  std::array<Face, 2> newUpFaces{ Face::Up, Face::Right };
  std::array<Face, 2> newRightFaces{ Face::Right, Face::Up };
  for (size_t i = 0; i < 2; ++i)
  {
    auto rotatedScramble = Rotate(scramble, std::make_pair(Face::Up, Face::Right), std::make_pair(newUpFaces[i], newRightFaces[i]));
    auto rotatedSolutionSoFar = solutionSoFar.Rotate(std::make_pair(Face::Up, Face::Right), std::make_pair(newUpFaces[i], newRightFaces[i]));
    auto rotatedScramblePlusSolution = rotatedSolutionSoFar.CombineScrambleAndSolution(rotatedScramble);

    // Front slot
    {
      auto rotatedStep = SolutionStep{ "F2L-1", solverF2LMinusFrontSlot.Solve(rotatedScramblePlusSolution) };
      auto rotatedPartialSolution = rotatedSolutionSoFar + rotatedStep;
      auto partialSolution = rotatedPartialSolution.Rotate(std::make_pair(newUpFaces[i], newRightFaces[i]), std::make_pair(Face::Up, Face::Right));
      partialSolutions.push_back(PartialSolution{ partialSolution, CubeGroup::F2L_FSlot_EO, std::make_pair(Face::Up, Face::Right), std::make_pair(newUpFaces[i], newRightFaces[i]) });
    }

    // Back slot
    {
      auto rotatedStep = SolutionStep{ "F2L-1", solverF2LMinusBackSlot.Solve(rotatedScramblePlusSolution) };
      auto rotatedPartialSolution = rotatedSolutionSoFar + rotatedStep;
      auto partialSolution = rotatedPartialSolution.Rotate(std::make_pair(newUpFaces[i], newRightFaces[i]), std::make_pair(Face::Up, Face::Right));
      partialSolutions.push_back(PartialSolution{ partialSolution, CubeGroup::F2L_BSlot_EO, std::make_pair(Face::Up, Face::Right), std::make_pair(newUpFaces[i], newRightFaces[i]) });
    }
  }

  return partialSolutions;
}
