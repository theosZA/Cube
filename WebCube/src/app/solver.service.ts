import { Injectable } from '@angular/core';
import { Observable } from 'rxjs/Observable';
import { of } from 'rxjs/observable/of';
import 'rxjs/add/operator/map';

import { ISolution } from './ISolution';
import { HttpClient } from '@angular/common/http';

@Injectable()
export class SolverService {

  constructor(private http: HttpClient) {}

  solve(scramble: string) : Observable<ISolution> {
    return this.http.post('http://localhost:1337/solve', { scramble: scramble }).map(data => <ISolution>data);
  }
}
