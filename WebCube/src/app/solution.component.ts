import { Component } from '@angular/core';
import { ISolutionStep } from './ISolutionStep';

@Component({
  selector: 'fmc-solver-solution',
  templateUrl: "./solution.component.html",
  styleUrls: ['./solution.component.css']
})
export class SolutionComponent  {

    GetTotalMoveCount() : number {
        return this.GetCumulativeMoveCount(this.solutionSteps.length - 1);
    }

    GetCumulativeMoveCount(stepIndex: number): number {
        let cumulativeMoveCount: number = 0;
        for (let i: number = 0; i <= stepIndex; ++i) {
            cumulativeMoveCount += this.solutionSteps[i].moves.length;
        }
        return cumulativeMoveCount;
    }

    solution: string;
    solutionSteps: ISolutionStep[];
}
