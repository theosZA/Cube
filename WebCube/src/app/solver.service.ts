import { Injectable } from '@angular/core';
import { Observable } from 'rxjs/Observable';
import { of } from 'rxjs/observable/of';

import { ISolution } from './ISolution';

@Injectable()
export class SolverService {

  solve(scramble: string) : Observable<ISolution> {
    return of({
      solution: "Just do " + scramble + " backwards",
      steps: [
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
        }]
    });
  }
}
