#pragma once

#include <atomic>
#include <chrono>
#include <functional>
#include <memory>
#include <thread>
#include <vector>

#include "Cube\CubeMove.h"
#include "Solver\Solver3x3x3.h"
#include "Solver\Solver2x2x2.h"

class CubeMoveSequenceAnimation;

// A state machine for the animation of a cube between scrambled and solved states
// with a time delay in between.
class CubeDemoStateMachine
{
public:
  CubeDemoStateMachine(size_t cubeSize, CubeMoveSequenceAnimation& moveSequenceAnimator, std::chrono::milliseconds solveDelay,
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
  // Calculates the solution for the current scramble in a seperate thread.
  void Solve();
  // Begins the animation of the solve and provides the solution to the callback in the constructor.
  void PlaySolve();

  size_t cubeSize;

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

  std::unique_ptr<Solver2x2x2> solver2x2x2;
  std::unique_ptr<Solver3x3x3> solver3x3x3;
  std::thread solvingThread;
  std::vector<CubeMove> scramble;
  std::vector<CubeMove> solution;
  std::atomic<bool> hasSolution;
};