#include "Solution.h"

void Solution::InsertStep(const std::string& description, const std::vector<CubeMove>& moves)
{
  steps.push_front(Step{ description, moves });
}

Solution Solution::Rotate(std::pair<Face, Face> oldFaces, std::pair<Face, Face> newFaces) const
{
  Solution rotatedSolution;
  rotatedSolution.moves = ::Rotate(moves, oldFaces, newFaces);
  rotatedSolution.steps.resize(steps.size());
  for (size_t i = 0; i < steps.size(); ++i)
  {
    rotatedSolution.steps[i] = Step{
      steps[i].description,
      ::Rotate(steps[i].moves, oldFaces, newFaces),
      ::Rotate(steps[i].skeletonPreceedingMoves, oldFaces, newFaces),
      ::Rotate(steps[i].skeletonSucceedingMoves, oldFaces, newFaces)
    };
  }
  return rotatedSolution;
}