import { Component } from '@angular/core';
import { SolutionComponent } from './solution.component';
import { SolverService } from './solver.service';

@Component({
  selector: 'fmc-solver-app',
  templateUrl: "./app.component.html",
  styleUrls: ['./app.component.css']
})
export class AppComponent  {

  scramble: string;

  constructor(private _solverService: SolverService) {
    this.scramble = "Enter the scramble here...";
  }

  onSolve(solutionComponent: SolutionComponent): void {
    this._solverService.solve(this.scramble)
        .subscribe(solution => solutionComponent.solution = solution);
  }
}
