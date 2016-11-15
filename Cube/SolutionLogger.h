#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "CubeMove.h"
#include "Solution.h"

class SolutionLogger
{
public:
  SolutionLogger(const std::string& fileName);

  void LogScramble(const std::vector<CubeMove>& scramble);
  void LogSolution(const Solution&);

private:
  std::ofstream log;
  int count;
};