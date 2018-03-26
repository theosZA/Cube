export interface ISolutionStep {
    step: string;
    moves: string[];

    // For insertions.
    skeleton?: string[];
    insertionIndex?: number;
}