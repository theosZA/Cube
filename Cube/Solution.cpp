#include "Solution.h"

#include <sstream>

Solution& Solution::operator+=(SolutionStep newStep)
{
  steps.push_back(std::move(newStep));
  length = -1;  // adding steps invalidates previously calculated length
  return *this;
}

Solution Solution::operator+(SolutionStep newStep) const
{
  return Solution{ *this } += newStep;
}

int Solution::Length() const
{
  std::vector<CubeMove> cumulativeSolutionOnForwardSolve;
  std::vector<CubeMove> cumulativeSolutionOnInverseSolve;
  for (const auto& step : steps)
  {
    step.AccumulateSolution(cumulativeSolutionOnForwardSolve, cumulativeSolutionOnInverseSolve);
  }
  if (!steps.empty() && steps.back().IsSkeletonComplete())
  {
    auto fullSolution = cumulativeSolutionOnForwardSolve + InvertMoveSequence(cumulativeSolutionOnInverseSolve);
    return length = static_cast<int>(fullSolution.size());
  }
  else
  {
    return static_cast<int>(cumulativeSolutionOnForwardSolve.size() + cumulativeSolutionOnInverseSolve.size());
  }
}

std::vector<CubeMove> Solution::GetMoves() const
{
  std::vector<CubeMove> cumulativeSolutionOnForwardSolve;
  std::vector<CubeMove> cumulativeSolutionOnInverseSolve;
  for (const auto& step : steps)
  {
    step.AccumulateSolution(cumulativeSolutionOnForwardSolve, cumulativeSolutionOnInverseSolve);
  }
  return cumulativeSolutionOnForwardSolve + InvertMoveSequence(cumulativeSolutionOnInverseSolve);
}

Solution Solution::Rotate(std::pair<Face, Face> oldFaces, std::pair<Face, Face> newFaces) const
{
  Solution rotatedSolution;
  rotatedSolution.steps.reserve(steps.size());
  for (auto& step : steps)
  {
    rotatedSolution += step.Rotate(oldFaces, newFaces);
  }
  return rotatedSolution;
}

Solution Solution::Invert() const
{
  Solution invertedSolution;
  invertedSolution.steps.reserve(steps.size());
  for (auto& step : steps)
  {
    invertedSolution += step.Invert();
  }
  return invertedSolution;
}

std::vector<CubeMove> Solution::CombineScrambleAndSolution(const std::vector<CubeMove>& scramble) const
{
  std::vector<CubeMove> cumulativeSolutionOnForwardSolve;
  std::vector<CubeMove> cumulativeSolutionOnInverseSolve;
  for (const auto& step : steps)
  {
    step.AccumulateSolution(cumulativeSolutionOnForwardSolve, cumulativeSolutionOnInverseSolve);
  }
  return InvertMoveSequence(cumulativeSolutionOnInverseSolve) + scramble + cumulativeSolutionOnForwardSolve;
}

std::string Solution::GetStepsDescription() const
{
  std::ostringstream stepsDescription;
  std::vector<CubeMove> cumulativeSolutionOnForwardSolve;
  std::vector<CubeMove> cumulativeSolutionOnInverseSolve;
  bool isSolutionComplete;
  GetStepsDescription(stepsDescription, cumulativeSolutionOnForwardSolve, cumulativeSolutionOnInverseSolve, isSolutionComplete);
  return stepsDescription.str();
}

std::ostream& operator<<(std::ostream& out, const Solution& solution)
{
  std::vector<CubeMove> cumulativeSolutionOnForwardSolve;
  std::vector<CubeMove> cumulativeSolutionOnInverseSolve;
  bool isSolutionComplete;
  solution.GetStepsDescription(out, cumulativeSolutionOnForwardSolve, cumulativeSolutionOnInverseSolve, isSolutionComplete);

  if (isSolutionComplete)
  {
    out << "\nFinal solution (" << solution.Length() << " moves): " << MoveSequenceToText(solution.GetMoves());
  }
  else
  {
    out << "\nPartial solution (" << cumulativeSolutionOnForwardSolve.size() + cumulativeSolutionOnInverseSolve.size() << " moves): "
        << MoveSequenceToText(cumulativeSolutionOnForwardSolve) << " ... " << MoveSequenceToText(InvertMoveSequence(cumulativeSolutionOnInverseSolve));
  }

  return out;
}

std::vector<SolutionStep>::size_type Solution::GetLastSkeletonStep() const
{
  if (steps.empty() || !steps.back().IsSkeletonComplete())
  {
    return -1;
  }

  auto lastNonSkeletonStep = steps.size() - 1;
  while (0 <= lastNonSkeletonStep && lastNonSkeletonStep < steps.size() && steps[lastNonSkeletonStep].IsSkeletonComplete())
  {
    --lastNonSkeletonStep;
  }
  auto lastStepOfSkeleton = lastNonSkeletonStep + 1;
  while (0 <= lastStepOfSkeleton && lastStepOfSkeleton < steps.size() && steps[lastStepOfSkeleton].IsEmpty())
  {
    --lastStepOfSkeleton;
  }
  return lastStepOfSkeleton;
}

void Solution::GetStepsDescription(std::ostream& out, std::vector<CubeMove>& cumulativeSolutionOnForwardSolve, std::vector<CubeMove>& cumulativeSolutionOnInverseSolve, bool& isSolutionComplete) const
{
  auto lastStepOfSkeleton = GetLastSkeletonStep();
  cumulativeSolutionOnForwardSolve.clear();
  cumulativeSolutionOnInverseSolve.clear();
  isSolutionComplete = false;

  for (std::vector<CubeMove>::size_type stepIndex = 0; stepIndex < steps.size(); ++stepIndex)
  {
    const auto& step = steps[stepIndex];
    if (!step.IsEmpty())
    {
      int oldMoveCount = static_cast<int>(cumulativeSolutionOnForwardSolve.size() + cumulativeSolutionOnInverseSolve.size());
      step.AccumulateSolution(cumulativeSolutionOnForwardSolve, cumulativeSolutionOnInverseSolve);

      if (stepIndex == lastStepOfSkeleton)
      { // Skeleton complete - combine forward and inverse solution.
        cumulativeSolutionOnForwardSolve += InvertMoveSequence(cumulativeSolutionOnInverseSolve);
        cumulativeSolutionOnForwardSolve = SimplifyMoveSequence(cumulativeSolutionOnForwardSolve);
        cumulativeSolutionOnInverseSolve.clear();
        isSolutionComplete = true;
      }

      int newMoveCount = static_cast<int>(cumulativeSolutionOnForwardSolve.size() + cumulativeSolutionOnInverseSolve.size());

      out << step.GetDescription() << ": " << step.GetMoveSequence(cumulativeSolutionOnForwardSolve)
          << " [" << (newMoveCount - oldMoveCount) << '/' << newMoveCount << "]\n";
    }
  }
}
