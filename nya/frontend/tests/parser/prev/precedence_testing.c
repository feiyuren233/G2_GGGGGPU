declarations -> epsilon
type -> int
expression -> false
variable -> identifier
expression -> variable
variable -> identifier
expression -> variable
variable -> identifier
expression -> variable
variable -> identifier
expression -> variable
expression -> integer_literal
expression -> expression / expression
expression -> expression - expression
expression -> integer_literal
unary_op -> -
expression -> integer_literal
expression -> unary_op expression
expression -> expression & expression
expression -> expression ^ expression
expression -> (expression)
expression -> expression == expression
expression -> expression && expression
expression -> expression || expression
declaration -> type ID = expression ;
declarations -> declarations declaration
statements -> epsilon
scope -> { declarations statements }
program -> int main() scope
