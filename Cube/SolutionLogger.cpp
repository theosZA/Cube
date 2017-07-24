#include "SolutionLogger.h"

SolutionLogger::SolutionLogger(const std::string& fileName)
: log(fileName, std::ios::trunc),
  count(0)
{}

void SolutionLogger::LogScramble(const std::vector<CubeMove>& scramble)
{
  ++count;
  log << "Scramble " << count << ": " << MoveSequenceToText(scramble) << "\n\n";
  log.flush();
}

void SolutionLogger::LogSolution(const Solution& solution)
{
  std::vector<CubeMove> cumulativeSolution;
  for (const auto& step : solution.steps)
  {
    if (!step.moves.empty())
    {
      int oldMoveCount = static_cast<int>(cumulativeSolution.size());
      if (step.skeletonPreceedingMoves.empty() && step.skeletonSucceedingMoves.empty())
      {
        cumulativeSolution = cumulativeSolution + step.moves;
        int newMoveCount = static_cast<int>(cumulativeSolution.size());
        log << step.description << ": " << MoveSequenceToText(step.moves) 
            << " (" << (newMoveCount - oldMoveCount) << '/' << newMoveCount << ")\n";
      }
      else
      {
        cumulativeSolution = step.skeletonPreceedingMoves + step.moves + step.skeletonSucceedingMoves;
        int newMoveCount = static_cast<int>(cumulativeSolution.size());
        log << step.description << ": Skeleton = " << MoveSequenceToText(step.skeletonPreceedingMoves) << " * " << MoveSequenceToText(step.skeletonSucceedingMoves)
            << "\n  Insert at * = " << MoveSequenceToText(step.moves)
            << " (" << (newMoveCount - oldMoveCount) << '/' << newMoveCount << ")\n";
      }
    }
  }

  log << "\nFinal solution (" << solution.moves.size() << " moves): " << MoveSequenceToText(solution.moves)
      << "\n\n-----------------------------------------------\n\n";
  log.flush();
}
