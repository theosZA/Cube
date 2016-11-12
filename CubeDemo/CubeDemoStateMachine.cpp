#include "CubeDemoStateMachine.h"

#include "CubeMoveSequenceAnimation.h"

CubeDemoStateMachine::CubeDemoStateMachine(
    CubeMoveSequenceAnimation& moveSequenceAnimator,
    std::chrono::milliseconds solveDelay,
    double scrambleSpeedQuarterRotationsPerSecond,
    double solveSpeedQuarterRotationsPerSecond,
    std::function<std::vector<CubeMove>()> getScramble,
    std::function<void(const std::vector<CubeMove>&)> onSolving)
: moveSequenceAnimator(moveSequenceAnimator),
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

    case State::Solving:
      StartSolve();
      break;
  }
}

void CubeDemoStateMachine::StartScramble()
{
  std::vector<CubeMove> scramble = getScramble();
  if (scramble.empty())
    state = State::Finished;
  else
    moveSequenceAnimator.Start(scramble, scrambleSpeedQuarterRotationsPerSecond, [=] { AdvanceState(); });
}

void CubeDemoStateMachine::StartSolve()
{
  auto solution = moveSequenceAnimator.SolveScramble(solveSpeedQuarterRotationsPerSecond, [=] { AdvanceState(); });
  onSolving(solution);
}
