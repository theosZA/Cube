import { NgModule }      from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { FormsModule } from '@angular/forms';
import { HttpClientModule } from '@angular/common/http';

import { AppComponent }  from './app.component';
import { SolutionComponent } from './solution.component';
import { SolverService } from './solver.service';

@NgModule({
  imports:      [ BrowserModule, FormsModule, HttpClientModule ],
  declarations: [ AppComponent, SolutionComponent ],
  bootstrap:    [ AppComponent ],
  providers:    [SolverService]
})
export class AppModule { }
