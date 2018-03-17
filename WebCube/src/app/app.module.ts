import { NgModule }      from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';

import { AppComponent }  from './app.component';
import { SolutionComponent } from './solution.component';

@NgModule({
  imports:      [ BrowserModule ],
  declarations: [ AppComponent, SolutionComponent ],
  bootstrap:    [ AppComponent ]
})
export class AppModule { }
