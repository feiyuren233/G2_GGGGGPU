declarations -> epsilon
type -> bool
variable -> identifier
expression -> variable
unary_op -> !
unary_op -> !
unary_op -> !
expression -> false
expression -> unary_op expression
expression -> unary_op expression
expression -> unary_op expression
expression -> expression && expression
declaration -> type ID = expression ;
declarations -> declarations declaration
type -> int
variable -> identifier
expression -> variable
unary_op -> -
expression -> integer_literal
expression -> unary_op expression
expression -> expression - expression
declaration -> type ID = expression ;
declarations -> declarations declaration
type -> int
unary_op -> ~
expression -> integer_literal
expression -> unary_op expression
declaration -> type ID = expression ;
declarations -> declarations declaration
statements -> epsilon
variable -> identifier
expression -> integer_literal
unary_op -> !
unary_op -> !
expression -> false
expression -> unary_op expression
unary_op -> !
variable -> identifier
expression -> variable
expression -> unary_op expression
expression -> expression && expression
expression -> (expression)
expression -> unary_op expression
expression -> expression && expression
statement -> variable = expression ;
statements -> statements statement
scope -> { declarations statements }
program -> int main() scope
