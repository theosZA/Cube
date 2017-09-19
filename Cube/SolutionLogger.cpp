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
  log << solution
      << "\n\n-----------------------------------------------\n\n";
  log.flush();
}
