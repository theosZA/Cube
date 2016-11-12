#pragma once

#include <chrono>
#include <functional>
#include <vector>

#include "..\Cube\CubeMove.h"

class CubeMoveSequenceAnimation;

// A state machine for the animation of a cube between scrambled and solved states
// with a time delay in between.
class CubeDemoStateMachine
{
public:
  CubeDemoStateMachine(CubeMoveSequenceAnimation& moveSequenceAnimator, std::chrono::milliseconds solveDelay,
                       double scrambleSpeedQuarterRotationsPerSecond, double solveSpeedQuarterRotationsPerSecond,
                       std::function<std::vector<CubeMove>()> getScramble,
                       std::function<void(const std::vector<CubeMove>&)> onSolving);

  // Call Start() when the animations begin before the first Step().
  void Start();
  // Advances the animation and advances to the next state if necessary.
  void Step();

private:
  // Returns true if the time has come to advance to the next state.
  bool IsReadyForNextState();
  // Advances to the next state in sequence. Never advances to the Finished state as that
  // has to be entered explicitly.
  void AdvanceState();

  // Begins the animation of a scramble using a scramble from the callback in the constructor.
  void StartScramble();
  // Begins the animation of the solve and provides the solution to the callback in the constructor.
  void StartSolve();

  CubeMoveSequenceAnimation& moveSequenceAnimator;
  double scrambleSpeedQuarterRotationsPerSecond;
  double solveSpeedQuarterRotationsPerSecond;

  enum State
  {
    Scrambling = 0,
    Scrambled,
    Solving,
    Solved,
    Finished
  } state;
  decltype(std::chrono::high_resolution_clock::now()) stateStartTime;
  std::chrono::milliseconds solveDelay;

  std::function<std::vector<CubeMove>()> getScramble;
  std::function<void(const std::vector<CubeMove>&)> onSolving;
};