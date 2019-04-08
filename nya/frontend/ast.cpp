#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cassert>

#include "ast.h"
#include "common.h"
#include "parser.tab.h"

node *ast = NULL;

//////////////////////////////////////////////////////////////////
//
// A Modern Object-Oriented Approach for AST
//
//////////////////////////////////////////////////////////////////

namespace AST
{ /* START NAMESPACE */

void Visitor::nodeVisit(ExpressionNode *expressionNode)
{
}

void Visitor::nodeVisit(ExpressionsNode *expressionsNode)
{
    for (ExpressionNode *expr : expressionsNode->getExpressionList())
    {
        expr->visit(*this);
    }
}

void Visitor::nodeVisit(UnaryExpressionNode *unaryExpressionNode)
{
    unaryExpressionNode->getExpression()->visit(*this);
}

void Visitor::nodeVisit(BinaryExpressionNode *binaryExpressionNode)
{
    binaryExpressionNode->getLeftExpression()->visit(*this);
    binaryExpressionNode->getRightExpression()->visit(*this);
}

void Visitor::nodeVisit(IntLiteralNode *intLiteralNode)
{
}

void Visitor::nodeVisit(FloatLiteralNode *floatLiteralNode)
{
}

void Visitor::nodeVisit(BooleanLiteralNode *booleanLiteralNode)
{
}

void Visitor::nodeVisit(VariableNode *variableNode)
{
}

void Visitor::nodeVisit(IdentifierNode *identifierNode)
{
}

void Visitor::nodeVisit(FunctionNode *functionNode)
{
    functionNode->getArgumentExpressions()->visit(*this);
}

void Visitor::nodeVisit(StatementNode *statementNode)
{
}

void Visitor::nodeVisit(StatementsNode *statementsNode)
{
    for (StatementNode *stmt : statementsNode->getStatementList())
    {
        stmt->visit(*this);
    }
}

void Visitor::nodeVisit(DeclarationNode *declarationNode)
{
    if (declarationNode->getExpression() != nullptr)
    {
        declarationNode->getExpression()->visit(*this);
    }
}

void Visitor::nodeVisit(DeclarationsNode *declarationsNode)
{
    for (DeclarationNode *decl : declarationsNode->getDeclarationList())
    {
        decl->visit(*this);
    }
}

void Visitor::nodeVisit(IfStatementNode *ifStatementNode)
{
    ifStatementNode->getConditionExpression()->visit(*this);
    ifStatementNode->getThenStatement()->visit(*this);
    if (ifStatementNode->getElseStatement() != nullptr)
    {
        ifStatementNode->getElseStatement()->visit(*this);
    }
}

void Visitor::nodeVisit(WhileStatementNode *whileStatementNode)
{
}

void Visitor::nodeVisit(AssignmentNode *assignmentNode)
{
    assignmentNode->getVariable()->visit(*this);
    assignmentNode->getExpression()->visit(*this);
}

void Visitor::nodeVisit(StallStatementNode *stallStatementNode)
{
}

void Visitor::nodeVisit(NestedScopeNode *nestedScopeNode)
{
    nestedScopeNode->getDeclarations()->visit(*this);
    nestedScopeNode->getStatements()->visit(*this);
}

void Visitor::nodeVisit(ScopeNode *scopeNode)
{
    scopeNode->getDeclarations()->visit(*this);
    scopeNode->getStatements()->visit(*this);
}

#define AST_VISITOR_VISIT    \
    {                        \
        preNodeVisit(node);  \
        nodeVisit(node);     \
        postNodeVisit(node); \
    }

void Visitor::visit(ExpressionNode *node) AST_VISITOR_VISIT;
void Visitor::visit(ExpressionsNode *node) AST_VISITOR_VISIT;
void Visitor::visit(UnaryExpressionNode *node) AST_VISITOR_VISIT;
void Visitor::visit(BinaryExpressionNode *node) AST_VISITOR_VISIT;
void Visitor::visit(IntLiteralNode *node) AST_VISITOR_VISIT;
void Visitor::visit(FloatLiteralNode *node) AST_VISITOR_VISIT;
void Visitor::visit(BooleanLiteralNode *node) AST_VISITOR_VISIT;
void Visitor::visit(VariableNode *node) AST_VISITOR_VISIT;
void Visitor::visit(IdentifierNode *node) AST_VISITOR_VISIT;
void Visitor::visit(FunctionNode *node) AST_VISITOR_VISIT;
void Visitor::visit(StatementNode *node) AST_VISITOR_VISIT;
void Visitor::visit(StatementsNode *node) AST_VISITOR_VISIT;
void Visitor::visit(DeclarationNode *node) AST_VISITOR_VISIT;
void Visitor::visit(DeclarationsNode *node) AST_VISITOR_VISIT;
void Visitor::visit(IfStatementNode *node) AST_VISITOR_VISIT;
void Visitor::visit(WhileStatementNode *node) AST_VISITOR_VISIT;
void Visitor::visit(AssignmentNode *node) AST_VISITOR_VISIT;
void Visitor::visit(StallStatementNode *node) AST_VISITOR_VISIT;
void Visitor::visit(NestedScopeNode *node) AST_VISITOR_VISIT;
void Visitor::visit(ScopeNode *node) AST_VISITOR_VISIT;

std::string DeclarationNode::getQualifierString() const
{
    return isConst() ? "const " : "";
}

int IntLiteralNode::getExpressionType() const
{
    return INT_T;
}

int FloatLiteralNode::getExpressionType() const
{
    return FLOAT_T;
}

int BooleanLiteralNode::getExpressionType() const
{
    return BOOL_T;
}

class Printer : public Visitor
{
  public:
    static std::string getIndentSpaceString(int indentSize)
    {
        return std::string(indentSize, ' ');
    }

  private:
    bool m_printSourceLocation = false;
    FILE *m_out = stdout;

  public:
    void setPrintSourceLocation(bool printSourceLocation) { m_printSourceLocation = printSourceLocation; }
    void setOutput(FILE *out) { m_out = out; }
    FILE *getOutput() const { return m_out; }

  private:
    void printSourceLocation(const ASTNode *node) const
    {
        if (m_printSourceLocation)
        {
            fprintf(m_out, "<%s>", node->getSourceLocationString().c_str());
        }
    }

  private:
    int m_indentSize = 0;
    static constexpr int m_indentIncr = 4;

  private:
    int getIndentSize() const { return m_indentSize; }
    void enterScope() { m_indentSize += m_indentIncr; }
    void exitScope() { m_indentSize -= m_indentIncr; }

  private:
    virtual void nodeVisit(ExpressionsNode *expressionsNode)
    {
        for (ExpressionNode *expr : expressionsNode->getExpressionList())
        {
            fprintf(m_out, " ");
            expr->visit(*this);
        }
    }

    virtual void nodeVisit(UnaryExpressionNode *unaryExpressionNode)
    {
        // (UNARY type op expr)
        fprintf(m_out, "(UNARY");
        printSourceLocation(unaryExpressionNode);
        fprintf(m_out, " ");
        fprintf(m_out, "%s ", unaryExpressionNode->getExpressionTypeString().c_str());
        fprintf(m_out, "%s ", getOperatorString(unaryExpressionNode->getOperator()).c_str());
        unaryExpressionNode->getExpression()->visit(*this);
        fprintf(m_out, ")");
    }

    virtual void nodeVisit(BinaryExpressionNode *binaryExpressionNode)
    {
        // (BINARY type op left right)
        fprintf(m_out, "(BINARY");
        printSourceLocation(binaryExpressionNode);
        fprintf(m_out, " ");
        fprintf(m_out, "%s ", binaryExpressionNode->getExpressionTypeString().c_str());
        fprintf(m_out, "%s ", getOperatorString(binaryExpressionNode->getOperator()).c_str());
        binaryExpressionNode->getLeftExpression()->visit(*this);
        fprintf(m_out, " ");
        binaryExpressionNode->getRightExpression()->visit(*this);
        fprintf(m_out, ")");
    }

    virtual void nodeVisit(IntLiteralNode *intLiteralNode)
    {
        // <literal>
        fprintf(m_out, "%d", intLiteralNode->getVal());
        printSourceLocation(intLiteralNode);
    }

    virtual void nodeVisit(FloatLiteralNode *floatLiteralNode)
    {
        // <literal>
        fprintf(m_out, "%f", floatLiteralNode->getVal());
        printSourceLocation(floatLiteralNode);
    }

    virtual void nodeVisit(BooleanLiteralNode *booleanLiteralNode)
    {
        // <literal>
        fprintf(m_out, "%s", (booleanLiteralNode->getVal() ? "true" : "false"));
        printSourceLocation(booleanLiteralNode);
    }

    virtual void nodeVisit(IdentifierNode *identifierNode)
    {
        // <identifier>
        fprintf(m_out, "%s", identifierNode->getName().c_str());
        printSourceLocation(identifierNode);
    }

    virtual void nodeVisit(FunctionNode *functionNode)
    {
        // (CALL name ...)
        fprintf(m_out, "(CALL");
        printSourceLocation(functionNode);
        fprintf(m_out, " ");
        fprintf(m_out, "%s", functionNode->getName().c_str());
        functionNode->getArgumentExpressions()->visit(*this);
        fprintf(m_out, ")");
    }

    virtual void nodeVisit(StatementsNode *statementsNode)
    {
        // (STATEMENTS ...)
        fprintf(m_out, "%s", getIndentSpaceString(getIndentSize()).c_str());
        fprintf(m_out, "(STATEMENTS");
        printSourceLocation(statementsNode);
        fprintf(m_out, "\n");
        enterScope();
        for (StatementNode *stmt : statementsNode->getStatementList())
        {
            fprintf(m_out, "%s", getIndentSpaceString(getIndentSize()).c_str());
            stmt->visit(*this);
            fprintf(m_out, "\n");
        }
        exitScope();
        fprintf(m_out, "%s", getIndentSpaceString(getIndentSize()).c_str());
        fprintf(m_out, ")\n");
    }

    virtual void nodeVisit(DeclarationNode *declarationNode)
    {
        // (DECLARATION variable-name type-name initial-value?)
        fprintf(m_out, "(DECLARATION");
        printSourceLocation(declarationNode);
        fprintf(m_out, " ");
        fprintf(m_out, "%s ", declarationNode->getName().c_str());
        fprintf(m_out, "%s", declarationNode->getQualifierString().c_str());
        fprintf(m_out, "%s", declarationNode->getTypeString().c_str());
        if (declarationNode->getInitValue() != nullptr)
        {
            fprintf(m_out, " ");
            declarationNode->getInitValue()->visit(*this);
        }
        else if (declarationNode->getExpression() != nullptr)
        {
            fprintf(m_out, " ");
            declarationNode->getExpression()->visit(*this);
        }
        fprintf(m_out, ")");
    }

    virtual void nodeVisit(DeclarationsNode *declarationsNode)
    {
        // (DECLARATIONS ...)
        fprintf(m_out, "%s", getIndentSpaceString(getIndentSize()).c_str());
        fprintf(m_out, "(DECLARATIONS");
        printSourceLocation(declarationsNode);
        fprintf(m_out, "\n");
        enterScope();
        for (DeclarationNode *decl : declarationsNode->getDeclarationList())
        {
            fprintf(m_out, "%s", getIndentSpaceString(getIndentSize()).c_str());
            decl->visit(*this);
            fprintf(m_out, "\n");
        }
        exitScope();
        fprintf(m_out, "%s", getIndentSpaceString(getIndentSize()).c_str());
        fprintf(m_out, ")\n");
    }

    virtual void nodeVisit(IfStatementNode *ifStatementNode)
    {
        // (IF cond then-stmt else-stmt?)
        fprintf(m_out, "(IF");
        printSourceLocation(ifStatementNode);
        fprintf(m_out, " ");
        ifStatementNode->getConditionExpression()->visit(*this);
        fprintf(m_out, " ");
        ifStatementNode->getThenStatement()->visit(*this);
        if (ifStatementNode->getElseStatement() != nullptr)
        {
            fprintf(m_out, " ");
            ifStatementNode->getElseStatement()->visit(*this);
        }
        fprintf(m_out, ")");
    }

    virtual void nodeVisit(AssignmentNode *assignmentNode)
    {
        // (ASSIGN type variable-name new-value)
        fprintf(m_out, "(ASSIGN");
        printSourceLocation(assignmentNode);
        fprintf(m_out, " ");
        fprintf(m_out, "%s ", assignmentNode->getExpressionTypeString().c_str());
        assignmentNode->getVariable()->visit(*this);
        fprintf(m_out, " ");
        assignmentNode->getExpression()->visit(*this);
        fprintf(m_out, ")");
    }

    virtual void nodeVisit(NestedScopeNode *nestedScopeNode)
    {
        // (SCOPE (DECLARATIONS ...) (STATEMENTS ...))
        fprintf(m_out, "(SCOPE");
        printSourceLocation(nestedScopeNode);
        fprintf(m_out, "\n");
        enterScope();
        nestedScopeNode->getDeclarations()->visit(*this);
        nestedScopeNode->getStatements()->visit(*this);
        exitScope();
        fprintf(m_out, "%s", getIndentSpaceString(getIndentSize()).c_str());
        fprintf(m_out, ")");
    }

    virtual void nodeVisit(ScopeNode *scopeNode)
    {
        // (SCOPE (DECLARATIONS ...) (STATEMENTS ...))
        fprintf(m_out, "(SCOPE");
        printSourceLocation(scopeNode);
        fprintf(m_out, "\n");
        enterScope();
        scopeNode->getDeclarations()->visit(*this);
        scopeNode->getStatements()->visit(*this);
        exitScope();
        fprintf(m_out, ")\n");
    }
};

std::string getTypeString(int type)
{
    switch (type)
    {
    case ANY_TYPE:
        return "ANY_TYPE";
    case BOOL_T:
        return "bool";
    case INT_T:
        return "int";
    case FLOAT_T:
        return "float";
    default:
        return "ERROR";
    }
}

std::string getOperatorString(int op)
{
    switch (op)
    {
    case NOT:
        return "!";
    case AND:
        return "&&";
    case OR:
        return "||";
    case PLUS:
        return "+";
    case MINUS:
        return "-";
    case TIMES:
        return "*";
    case SLASH:
        return "/";
    case EQL:
        return "==";
    case NEQ:
        return "!=";
    case LSS:
        return "<";
    case LEQ:
        return "<=";
    case GTR:
        return ">";
    case GEQ:
        return ">=";
    case BXOR:
        return "^";
    case BAND:
        return "&";
    case BOR:
        return "|";
    case BNOT:
        return "~";
    case BSL:
        return "<<";
    case BSR:
        return ">>";
    default:
        return "Error";
    }
}

std::string getSourceLocationString(const SourceLocation &srcLoc)
{
    return (std::string("Line ") +
            std::to_string(srcLoc.firstLine) +
            std::string(":") +
            std::to_string(srcLoc.firstColumn) +
            std::string(" to Line ") +
            std::to_string(srcLoc.lastLine) +
            std::string(":") +
            std::to_string(srcLoc.lastColumn));
}

} // namespace AST

//////////////////////////////////////////////////////////////////
//
// Interface Functions
//
//////////////////////////////////////////////////////////////////

node *ast_allocate(node_kind kind, ...)
{
    va_list args;
    va_start(args, kind);

    // make the node
    AST::ASTNode *astNode = nullptr;

    switch (kind)
    {
    case SCOPE_NODE:
    {
        AST::DeclarationsNode *decls = static_cast<AST::DeclarationsNode *>(va_arg(args, AST::ASTNode *));
        AST::StatementsNode *stmts = static_cast<AST::StatementsNode *>(va_arg(args, AST::ASTNode *));
        astNode = new AST::ScopeNode(decls, stmts);

        YYLTYPE *loc = va_arg(args, YYLTYPE *);
        astNode->setSourceLocation(AST::SourceLocation{loc->first_line, loc->first_column, loc->last_line, loc->last_column});

        break;
    }

    case EXPRESSION_NODE:
    {
        /* Virtual ASTNode, Does not Have Instance */
        astNode = nullptr;
        break;
    }

    case EXPRESSIONS_NODE:
    {
        AST::ExpressionsNode *exprs = static_cast<AST::ExpressionsNode *>(va_arg(args, AST::ASTNode *));
        AST::ExpressionNode *expr = static_cast<AST::ExpressionNode *>(va_arg(args, AST::ASTNode *));
        if (exprs == nullptr)
        {
            exprs = new AST::ExpressionsNode();
        }
        if (expr != nullptr)
        {
            exprs->pushBackExpression(expr);
        }
        astNode = exprs;

        YYLTYPE *loc = va_arg(args, YYLTYPE *);
        astNode->setSourceLocation(AST::SourceLocation{loc->first_line, loc->first_column, loc->last_line, loc->last_column});

        break;
    }

    case UNARY_EXPRESION_NODE:
    {
        int op = va_arg(args, int);
        AST::ExpressionNode *expr = static_cast<AST::ExpressionNode *>(va_arg(args, AST::ASTNode *));
        astNode = new AST::UnaryExpressionNode(op, expr);

        YYLTYPE *loc = va_arg(args, YYLTYPE *);
        astNode->setSourceLocation(AST::SourceLocation{loc->first_line, loc->first_column, loc->last_line, loc->last_column});

        break;
    }

    case BINARY_EXPRESSION_NODE:
    {
        int op = va_arg(args, int);
        AST::ExpressionNode *leftExpr = static_cast<AST::ExpressionNode *>(va_arg(args, AST::ASTNode *));
        AST::ExpressionNode *rightExpr = static_cast<AST::ExpressionNode *>(va_arg(args, AST::ASTNode *));
        astNode = new AST::BinaryExpressionNode(op, leftExpr, rightExpr);

        YYLTYPE *loc = va_arg(args, YYLTYPE *);
        astNode->setSourceLocation(AST::SourceLocation{loc->first_line, loc->first_column, loc->last_line, loc->last_column});

        break;
    }

    case INT_C_NODE:
    {
        int val = va_arg(args, int);
        astNode = new AST::IntLiteralNode(val);

        YYLTYPE *loc = va_arg(args, YYLTYPE *);
        astNode->setSourceLocation(AST::SourceLocation{loc->first_line, loc->first_column, loc->last_line, loc->last_column});

        break;
    }

    case FLOAT_C_NODE:
    {
        double val = va_arg(args, double);
        astNode = new AST::FloatLiteralNode(val);

        YYLTYPE *loc = va_arg(args, YYLTYPE *);
        astNode->setSourceLocation(AST::SourceLocation{loc->first_line, loc->first_column, loc->last_line, loc->last_column});

        break;
    }

    case BOOL_C_NODE:
    {
        bool val = static_cast<bool>(va_arg(args, int));
        astNode = new AST::BooleanLiteralNode(val);

        YYLTYPE *loc = va_arg(args, YYLTYPE *);
        astNode->setSourceLocation(AST::SourceLocation{loc->first_line, loc->first_column, loc->last_line, loc->last_column});

        break;
    }

    case ID_NODE:
    {
        const char *id = va_arg(args, char *);
        astNode = new AST::IdentifierNode(id);

        YYLTYPE *loc = va_arg(args, YYLTYPE *);
        astNode->setSourceLocation(AST::SourceLocation{loc->first_line, loc->first_column, loc->last_line, loc->last_column});

        break;
    }

    case FUNCTION_NODE:
    {
        const char *functionName = va_arg(args, char *);
        AST::ExpressionsNode *argExprs = static_cast<AST::ExpressionsNode *>(va_arg(args, AST::ASTNode *));
        astNode = new AST::FunctionNode(functionName, argExprs);

        YYLTYPE *loc = va_arg(args, YYLTYPE *);
        astNode->setSourceLocation(AST::SourceLocation{loc->first_line, loc->first_column, loc->last_line, loc->last_column});

        break;
    }

    case STATEMENT_NODE:
    {
        /* Virtual ASTNode, Does not Have Instance */
        astNode = nullptr;
        break;
    }

    case IF_STATEMENT_NODE:
    {
        AST::ExpressionNode *condExpr = static_cast<AST::ExpressionNode *>(va_arg(args, AST::ASTNode *));
        AST::StatementNode *thenStmt = static_cast<AST::StatementNode *>(va_arg(args, AST::ASTNode *));
        AST::StatementNode *elseStmt = static_cast<AST::StatementNode *>(va_arg(args, AST::ASTNode *));
        astNode = new AST::IfStatementNode(condExpr, thenStmt, elseStmt);

        YYLTYPE *loc = va_arg(args, YYLTYPE *);
        astNode->setSourceLocation(AST::SourceLocation{loc->first_line, loc->first_column, loc->last_line, loc->last_column});

        break;
    }

    case WHILE_STATEMENT_NODE:
    {
        AST::ExpressionNode *condExpr = static_cast<AST::ExpressionNode *>(va_arg(args, AST::ASTNode *));
        AST::StatementNode *bodyStmt = static_cast<AST::StatementNode *>(va_arg(args, AST::ASTNode *));
        astNode = new AST::WhileStatementNode(condExpr, bodyStmt);

        YYLTYPE *loc = va_arg(args, YYLTYPE *);
        astNode->setSourceLocation(AST::SourceLocation{loc->first_line, loc->first_column, loc->last_line, loc->last_column});

        break;
    }

    case ASSIGNMENT_NODE:
    {
        AST::VariableNode *var = static_cast<AST::VariableNode *>(va_arg(args, AST::ASTNode *));
        AST::ExpressionNode *newValExpr = static_cast<AST::ExpressionNode *>(va_arg(args, AST::ASTNode *));
        astNode = new AST::AssignmentNode(var, newValExpr);

        YYLTYPE *loc = va_arg(args, YYLTYPE *);
        astNode->setSourceLocation(AST::SourceLocation{loc->first_line, loc->first_column, loc->last_line, loc->last_column});

        break;
    }

    case NESTED_SCOPE_NODE:
    {
        /* Argument ScopeNode is destructed and converted to NestedScopeNode */
        AST::ScopeNode *scopeNode = static_cast<AST::ScopeNode *>(va_arg(args, AST::ASTNode *));
        astNode = AST::ScopeNode::convertToNestedScopeNode(scopeNode);

        YYLTYPE *loc = va_arg(args, YYLTYPE *);
        astNode->setSourceLocation(AST::SourceLocation{loc->first_line, loc->first_column, loc->last_line, loc->last_column});

        break;
    }

    case STALL_STATEMENT_NODE:
    {
        astNode = new AST::StallStatementNode();

        YYLTYPE *loc = va_arg(args, YYLTYPE *);
        astNode->setSourceLocation(AST::SourceLocation{loc->first_line, loc->first_column, loc->last_line, loc->last_column});

        break;
    }

    case STATEMENTS_NODE:
    {
        AST::StatementsNode *stmts = static_cast<AST::StatementsNode *>(va_arg(args, AST::ASTNode *));
        AST::StatementNode *stmt = static_cast<AST::StatementNode *>(va_arg(args, AST::ASTNode *));
        if (stmts == nullptr)
        {
            stmts = new AST::StatementsNode();
        }
        if (stmt != nullptr)
        {
            stmts->pushBackStatement(stmt);
        }
        astNode = stmts;

        YYLTYPE *loc = va_arg(args, YYLTYPE *);
        astNode->setSourceLocation(AST::SourceLocation{loc->first_line, loc->first_column, loc->last_line, loc->last_column});

        break;
    }

    case DECLARATION_NODE:
    {
        const char *varName = va_arg(args, char *);
        int isConst = va_arg(args, int);
        int type = va_arg(args, int);
        AST::ExpressionNode *initValExpr = static_cast<AST::ExpressionNode *>(va_arg(args, AST::ASTNode *));
        astNode = new AST::DeclarationNode(varName, static_cast<bool>(isConst), type, initValExpr);

        YYLTYPE *loc = va_arg(args, YYLTYPE *);
        astNode->setSourceLocation(AST::SourceLocation{loc->first_line, loc->first_column, loc->last_line, loc->last_column});

        break;
    }

    case DECLARATIONS_NODE:
    {
        AST::DeclarationsNode *decls = static_cast<AST::DeclarationsNode *>(va_arg(args, AST::ASTNode *));
        AST::DeclarationNode *decl = static_cast<AST::DeclarationNode *>(va_arg(args, AST::ASTNode *));
        if (decls == nullptr)
        {
            decls = new AST::DeclarationsNode();
        }
        if (decl != nullptr)
        {
            decls->pushBackDeclaration(decl);
        }
        astNode = decls;

        YYLTYPE *loc = va_arg(args, YYLTYPE *);
        astNode->setSourceLocation(AST::SourceLocation{loc->first_line, loc->first_column, loc->last_line, loc->last_column});

        break;
    }

    case UNKNOWN:
    default:
        astNode = nullptr;
        break;
    }

    va_end(args);

    return static_cast<node *>(astNode);
}

void ast_free(node *ast)
{
    if (ast != nullptr)
    {
        AST::ASTNode::destructNode(static_cast<AST::ASTNode *>(ast));
    }
}

void ast_print(node *ast)
{
    if (ast != nullptr)
    {
        AST::Printer printer;
        printer.setPrintSourceLocation(false);
        printer.setOutput(dumpFile);
        static_cast<AST::ASTNode *>(ast)->visit(printer);
    }
}
