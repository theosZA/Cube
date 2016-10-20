#include "CubeMoveSequenceAnimation.h"

#include "..\Cube\SolverFactory.h"

using namespace irr;

CubeMoveSequenceAnimation::CubeMoveSequenceAnimation(scene::ISceneManager* sceneManager, scene::ISceneNode* parent, f32 totalSize, size_t cubeSize)
: cubeSize(cubeSize),
  cube(sceneManager, parent, totalSize, cubeSize),
  currentMove(0)
{}

void CubeMoveSequenceAnimation::Start(std::vector<CubeMove> moves, double quarterRotationsPerSecond, std::function<void()> onComplete)
{
  if (IsAnimating())
    JumpToEnd();

  this->quarterRotationsPerSecond = quarterRotationsPerSecond;
  this->moves = std::move(moves);
  this->onComplete = std::move(onComplete);
  StartMove(0);
}

void CubeMoveSequenceAnimation::Update()
{
  if (cube.IsAnimating())
    cube.UpdateAnimate();

  if (!cube.IsAnimating() && currentMove < moves.size())
    StartMove(currentMove + 1);
}

void CubeMoveSequenceAnimation::JumpToEnd()
{
  while (IsAnimating())
  {
    if (cube.IsAnimating())
      cube.UpdateAnimate(true);
    StartMove(currentMove + 1);
  }
}

bool CubeMoveSequenceAnimation::IsAnimating() const
{
  return currentMove < moves.size();
}

const std::vector<CubeMove>& CubeMoveSequenceAnimation::RandomScramble(int seed, size_t scrambleLength, double quarterRotationsPerSecond, const std::function<void()>& onComplete)
{
  std::mt19937 generator(seed);
  std::uniform_int_distribution<> faceDistribution(0, 5);
  std::uniform_int_distribution<> rotationDistribution(0, 2);
  std::vector<CubeMove> scramble(scrambleLength);
  for (size_t i = 0; i < scrambleLength; ++i)
  {
    do
      scramble[i].face = static_cast<Face>(faceDistribution(generator));
    while (i > 0 && scramble[i].face == scramble[i - 1].face);

    scramble[i].quarterRotationsClockwise = rotationDistribution(generator);
    if (scramble[i].quarterRotationsClockwise == 0)
      scramble[i].quarterRotationsClockwise = -1;
  }
  
  Start(scramble, quarterRotationsPerSecond, onComplete);
  return moves;
}

const std::vector<CubeMove>& CubeMoveSequenceAnimation::SolveScramble(double quarterRotationsPerSecond, const std::function<void()>& onComplete)
{
  Start(SolverFactory::CreateSolver(cubeSize)->Solve(moves), quarterRotationsPerSecond, onComplete);
  return moves;
}

void CubeMoveSequenceAnimation::StartMove(size_t index)
{
  currentMove = index;
  if (currentMove < moves.size())
    cube.AnimateMove(moves[currentMove].face, moves[currentMove].quarterRotationsClockwise, 1, quarterRotationsPerSecond);
  else
    // Move sequence complete
    onComplete();
}
