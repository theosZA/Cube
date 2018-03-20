import { NgModule }      from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { FormsModule } from '@angular/forms';

import { AppComponent }  from './app.component';
import { SolutionComponent } from './solution.component';
import { SolverService } from './solver.service';

@NgModule({
  imports:      [ BrowserModule, FormsModule ],
  declarations: [ AppComponent, SolutionComponent ],
  bootstrap:    [ AppComponent ],
  providers:    [SolverService]
})
export class AppModule { }
