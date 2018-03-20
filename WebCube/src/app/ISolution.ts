import { ISolutionStep } from "./ISolutionStep";

export interface ISolution {
    solution: string;
    steps: ISolutionStep[];
}