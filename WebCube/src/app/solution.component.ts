import { Component } from '@angular/core';
import { ISolutionStep } from './ISolutionStep';
import { ISolution } from './ISolution';

@Component({
  selector: 'fmc-solver-solution',
  templateUrl: "./solution.component.html",
  styleUrls: ['./solution.component.css']
})
export class SolutionComponent  {

    GetTotalMoveCount() : number {
        return this.solution.solution.split(" ").length;
    }

    GetPreInsertionMoves(stepIndex: number): string {
        let step: ISolutionStep = this.solution.steps[stepIndex];
        if (step.skeleton == null || step.insertionIndex == null) {
            // Normal (non-insertion) step.
            return "";
        }
        return step.skeleton.slice(0, step.insertionIndex).join(" ") + " ";
    }

    GetPostInsertionMoves(stepIndex: number): string {
        let step: ISolutionStep = this.solution.steps[stepIndex];
        if (step.skeleton == null || step.insertionIndex == null) {
            // Normal (non-insertion) step.
            return "";
        }
        return " " + step.skeleton.slice(step.insertionIndex).join(" ");
    }
    solution: ISolution;
}
