#include "CubeDemoStateMachine.h"

#include "CubeMoveSequenceAnimation.h"

CubeDemoStateMachine::CubeDemoStateMachine(
    size_t cubeSize,
    CubeMoveSequenceAnimation& moveSequenceAnimator,
    std::chrono::milliseconds solveDelay,
    double scrambleSpeedQuarterRotationsPerSecond,
    double solveSpeedQuarterRotationsPerSecond,
    std::function<std::vector<CubeMove>()> getScramble,
    std::function<void(const std::vector<CubeMove>&)> onSolving)
: cubeSize(cubeSize),
  moveSequenceAnimator(moveSequenceAnimator),
  scrambleSpeedQuarterRotationsPerSecond(scrambleSpeedQuarterRotationsPerSecond),
  solveSpeedQuarterRotationsPerSecond(solveSpeedQuarterRotationsPerSecond),
  solveDelay(solveDelay),
  getScramble(getScramble),
  onSolving(onSolving)
{}

void CubeDemoStateMachine::Start()
{
  state = State::Solved;
  stateStartTime = std::chrono::high_resolution_clock::now();
}

void CubeDemoStateMachine::Step()
{
  switch (state)
  {
    case State::Scrambling:
    case State::Solving:
      moveSequenceAnimator.Update();
      break;

    case State::Scrambled:
      if (IsReadyForNextState() && hasSolution)
      {
        solvingThread.join();
        AdvanceState();
      }
      break;

    case State::Solved:
      if (IsReadyForNextState())
        AdvanceState();
      break;
  }
}

bool CubeDemoStateMachine::IsReadyForNextState()
{
  return (std::chrono::high_resolution_clock::now() - stateStartTime >= solveDelay);
}

void CubeDemoStateMachine::AdvanceState()
{
  stateStartTime = std::chrono::high_resolution_clock::now();
  state = static_cast<State>((static_cast<int>(state) + 1) % 4);
  switch (state)
  {
    case State::Scrambling:
      StartScramble();
      break;

    case State::Scrambled:
      Solve();
      break;

    case State::Solving:
      PlaySolve();
      break;
  }
}

void CubeDemoStateMachine::StartScramble()
{
  hasSolution = false;
  solution.clear();
  scramble = getScramble();
  if (scramble.empty())
    state = State::Finished;
  else
    moveSequenceAnimator.Start(scramble, scrambleSpeedQuarterRotationsPerSecond, [=] { AdvanceState(); });
}

void CubeDemoStateMachine::Solve()
{
  switch (cubeSize)
  {
    case 2:
      if (!solver2x2x2)
      {
        solver2x2x2.reset(new Solver2x2x2(20, "solution.2x2"));
      }
      solvingThread = std::thread{
        [this]
        {
          solution = solver2x2x2->Solve(scramble);
          hasSolution = true;
        }};
      break;

    case 3:
      if (!solver3x3x3)
      {
        solver3x3x3.reset(new Solver3x3x3);
      }
      solvingThread = std::thread{
        [this]
        {
          solution = solver3x3x3->Solve(scramble).GetMoves();
          hasSolution = true;
        }};
      break;
    }
}

void CubeDemoStateMachine::PlaySolve()
{
  moveSequenceAnimator.Start(solution, solveSpeedQuarterRotationsPerSecond, [=] { AdvanceState(); });
  onSolving(solution);
}
