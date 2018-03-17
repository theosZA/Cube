import { Component } from '@angular/core';
import { SolutionComponent } from './solution.component';

@Component({
  selector: 'fmc-solver-app',
  templateUrl: "./app.component.html",
  styleUrls: ['./app.component.css']
})
export class AppComponent  {
  onSolve(solutionComponent: SolutionComponent): void {
    solutionComponent.solution = "F R U R' U' F'";
    solutionComponent.solutionSteps = [
      {
          step: "Setup",
          moves: ["F"]
      },
      {
          step: "Apply alg",
          moves: ["R", "U", "R'", "U'"]
      },
      {
          step: "Undo setup",
          moves: ["F'"]
      }];
  }
}
