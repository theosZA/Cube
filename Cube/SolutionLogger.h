#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "CubeMove.h"

class SolutionLogger
{
public:
  SolutionLogger(const std::string& fileName);

  void LogScramble(const std::vector<CubeMove>& scramble);
  void LogStep(const std::string& stepDescription);
  void LogSolution(const std::vector<CubeMove>& solution);

private:
  std::ofstream log;
  int count;
};