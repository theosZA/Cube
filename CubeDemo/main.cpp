#include <memory>
#include <random>
#include <fstream>

#include "CubeDemo.h"

const char* const fixedScrambleFileName = "scramble.txt";

int main()
{
  std::unique_ptr<CubeDemo> demo;
  std::ifstream scrambleFile(fixedScrambleFileName);
  if (scrambleFile.good())
  {
    scrambleFile >> std::noskipws;
    std::string scrambleText;
    std::copy(std::istream_iterator<char>(scrambleFile), std::istream_iterator<char>(), std::back_inserter(scrambleText));
    demo.reset(new CubeDemo(ParseMoveSequence(scrambleText)));
  }
  else
  {
    std::random_device randomSource;
    demo.reset(new CubeDemo(randomSource()));
  }
  demo->RunDemo();
}