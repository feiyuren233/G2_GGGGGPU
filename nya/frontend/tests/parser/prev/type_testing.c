declarations -> epsilon
type -> int
declaration -> type ID ;
declarations -> declarations declaration
type -> bool
declaration -> type ID ;
declarations -> declarations declaration
type -> float
declaration -> type ID ;
declarations -> declarations declaration
statements -> epsilon
scope -> { declarations statements }
program -> int main() scope
