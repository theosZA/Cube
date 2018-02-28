#include <memory>
#include <random>
#include <fstream>
#include <vector>

#include "Cube\CubeMove.h"

#include "CubeDemo.h"

const char* const fixedScrambleFileName = "scramble.txt";

std::vector<std::vector<CubeMove>> ReadFixedScrambles(std::istream& in)
{
  std::vector<std::vector<CubeMove>> scrambles;
  char line[4096];
  while (in.good() && !in.eof())
  {
    in.getline(line, sizeof(line));
    std::string lineText = line;
    if (!lineText.empty() && !(lineText.size() >= 2 && lineText[0] == '/' && lineText[1] == '/'))
    {
      scrambles.push_back(ParseMoveSequence(lineText));
    }
  }
  return scrambles;
}

int main()
{
  std::unique_ptr<CubeDemo> demo;
  std::ifstream scrambleFile(fixedScrambleFileName);
  if (scrambleFile.good())
  {
    demo.reset(new CubeDemo(ReadFixedScrambles(scrambleFile)));
  }
  else
  {
    std::random_device randomSource;
    demo.reset(new CubeDemo(randomSource()));
  }
  demo->RunDemo();
}