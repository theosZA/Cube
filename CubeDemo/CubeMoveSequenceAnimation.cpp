#include "CubeMoveSequenceAnimation.h"

#include "..\Cube\SolverFactory.h"

using namespace irr;

CubeMoveSequenceAnimation::CubeMoveSequenceAnimation(scene::ISceneManager& sceneManager, scene::ISceneNode* parent, f32 totalSize, size_t cubeSize)
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

const std::vector<CubeMove>& CubeMoveSequenceAnimation::SolveScramble(double quarterRotationsPerSecond, const std::function<void()>& onComplete)
{
  if (!solver)
    solver = SolverFactory::CreateSolver(cubeSize);

  Start(solver->Solve(moves), quarterRotationsPerSecond, onComplete);
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
