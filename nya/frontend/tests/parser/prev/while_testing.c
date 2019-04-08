declarations -> epsilon
statements -> epsilon
expression -> integer_literal
expression -> integer_literal
expression -> expression == expression
declarations -> epsilon
statements -> epsilon
unary_op -> !
expression -> false
expression -> unary_op expression
statement -> ;
statement -> while ( expression ) statement
statements -> statements statement
scope -> { declarations statements }
statement -> scope
statement -> while ( expression ) statement
statements -> statements statement
scope -> { declarations statements }
program -> int main() scope
