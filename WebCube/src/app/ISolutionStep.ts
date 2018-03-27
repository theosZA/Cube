export interface ISolutionStep {
    step: string;
    moves: string[];
    stepMoveCount: number;
    cumulativeMoveCount: number;

    // For insertions.
    skeleton?: string[];
    insertionIndex?: number;
}