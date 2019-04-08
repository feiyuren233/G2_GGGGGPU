#include "codegen.h"

#include "ast.h"
#include "semantic.h"
#include "codegen_instruction.h"
#include "source_context.h"

#include "common.h"
#include "parser.tab.h"

#include <cassert>
#include <unordered_map>
#include <vector>
#include <string>
#include <list>
#include <sstream>
#include <iomanip>
#include <memory>

namespace COGEN
{ /* START NAMESPACE */

/* Stores the Meow Assembly */
class MeowAssemblyDatabase
{
  private:
    std::unordered_map<std::string, Register> m_userDeclaredRegMapping;
    unsigned m_intRegCount = 0;
    unsigned m_floatRegCount = 0;

    std::list<unsigned> m_savedIntRegCount;
    std::list<unsigned> m_savedFloatRegCount;

  private:
    /* For assembly instructions */
    std::vector<std::unique_ptr<Instruction>> m_instructions;

  public:
    void saveRegisterContext();
    void loadRegisterContext();
    void discardRegisterContext();

  public:
    void declareUserRegister(const std::string &regName, RegType regType);
    Register createUserRegister(const std::string &regName);
    Register getUserRegister(const std::string &regName) const;

  public:
    Register requestTempRegister(RegType regType);
    Register requestTempIntRegister();
    Register requestTempFloatRegister();

    Register getTempRegister(RegType regType);
    Register getTempIntRegister();
    Register getTempFloatRegister();

  public:
    void insertInstruction(Instruction *instruction);
    void insertInstructionComment(const std::string &comment);

  public:
    std::vector<std::string> generateCode() const;
    void dump() const;
    void output(FILE *fd) const;
};

void MeowAssemblyDatabase::saveRegisterContext(){
    m_savedIntRegCount.push_front(m_intRegCount);
    m_savedFloatRegCount.push_front(m_floatRegCount);
}

void MeowAssemblyDatabase::loadRegisterContext(){
    m_intRegCount = m_savedIntRegCount.front();
    m_floatRegCount = m_savedFloatRegCount.front();
}

void MeowAssemblyDatabase::discardRegisterContext(){
    m_savedIntRegCount.pop_front();
    m_savedFloatRegCount.pop_front();
}

void MeowAssemblyDatabase::declareUserRegister(const std::string &regName, RegType regType)
{
    // No duplicate register declaration allowed
    assert(m_userDeclaredRegMapping.count(regName) == 0);

    unsigned regId = 0;
    assert(regType.getValue() != RegType::GENERAL);

    m_userDeclaredRegMapping.emplace(regName, Register{regType, regId});
}

Register MeowAssemblyDatabase::createUserRegister(const std::string &regName)
{
    auto fit = m_userDeclaredRegMapping.find(regName);
    assert(fit != m_userDeclaredRegMapping.end());

    RegType regType = fit->second.regType;
    if (regType.getValue() == RegType::INT)
    {
        fit->second.id = m_intRegCount;
        m_intRegCount++;
    }
    else if (regType.getValue() == RegType::FLOAT)
    {
        fit->second.id = m_floatRegCount;
        m_floatRegCount++;
    }
    else
    {
        assert(0);
    }

    return fit->second;
}

Register MeowAssemblyDatabase::getUserRegister(const std::string &regName) const
{
    auto fit = m_userDeclaredRegMapping.find(regName);
    assert(fit != m_userDeclaredRegMapping.end());

    return fit->second;
}

Register MeowAssemblyDatabase::requestTempRegister(RegType regType)
{
    switch (regType.getValue())
    {
    case RegType::INT:
        return requestTempIntRegister();
    case RegType::FLOAT:
        return requestTempFloatRegister();
    case RegType::GENERAL:
        assert(0);
    }

    return Register{};
}

Register MeowAssemblyDatabase::requestTempIntRegister()
{
    Register reg = {RegType(RegType::INT), m_intRegCount};
    m_intRegCount++;
    return reg;
}

Register MeowAssemblyDatabase::requestTempFloatRegister()
{
    Register reg = {RegType(RegType::FLOAT), m_floatRegCount};
    m_floatRegCount++;
    return reg;
}

Register MeowAssemblyDatabase::getTempRegister(RegType regType)
{
    switch (regType.getValue())
    {
    case RegType::INT:
        return getTempIntRegister();
    case RegType::FLOAT:
        return getTempFloatRegister();
    case RegType::GENERAL:
        assert(0);
    }

    return Register{};
}

Register MeowAssemblyDatabase::getTempIntRegister()
{
    if (m_intRegCount == 0)
    {
        return requestTempIntRegister();
    }
    else
    {
        Register reg = {RegType(RegType::INT), m_intRegCount - 1};
        return reg;
    }
}

Register MeowAssemblyDatabase::getTempFloatRegister()
{
    if (m_floatRegCount == 0)
    {
        return requestTempFloatRegister();
    }
    else
    {
        Register reg = {RegType(RegType::FLOAT), m_floatRegCount - 1};
        return reg;
    }
}

void MeowAssemblyDatabase::insertInstruction(Instruction *instruction)
{
    m_instructions.emplace_back(instruction);
}

void MeowAssemblyDatabase::insertInstructionComment(const std::string &comment)
{
    m_instructions.emplace_back(new MeowComment(comment));
}

std::vector<std::string> MeowAssemblyDatabase::generateCode() const
{
    std::vector<std::string> assemblyCode;

    assemblyCode.emplace_back("# Instructions");
    for (const auto &ins : m_instructions)
    {
        assemblyCode.push_back(ins->generateCode());
    }
    assemblyCode.emplace_back("");

    return assemblyCode;
}

void MeowAssemblyDatabase::dump() const
{
    std::vector<std::string> assemblyCode = generateCode();

    for (unsigned i = 0; i < assemblyCode.size(); i++)
    {
        printf("%-12u %s\n", i + 1, assemblyCode[i].c_str());
    }
}

void MeowAssemblyDatabase::output(FILE *fd) const
{
    std::vector<std::string> assemblyCode = generateCode();

    fprintf(fd, "# Meow Meow Meow GPU ASSEMBLY\n");
    fprintf(fd, "\n");

    for (const auto &line : assemblyCode)
    {
        fprintf(fd, "%s\n", line.c_str());
    }
}

/* Flattened symbol table with resolved symbol names */
class DeclaredSymbolRegisterTable
{
  public:
    using NameToDeclHashTable = std::unordered_map<std::string, const AST::DeclarationNode *>;
    using DeclToNameHashTable = std::unordered_map<const AST::DeclarationNode *, std::string>;

  private:
    NameToDeclHashTable m_regNameToDecl;
    DeclToNameHashTable m_declToregName;

  public:
    bool hasRegisterName(const std::string &regName) const
    {
        return (m_regNameToDecl.count(regName) == 1);
    }

    bool hasDeclaration(const AST::DeclarationNode *decl) const
    {
        return (m_declToregName.count(decl) == 1);
    }

    const std::string &getRegisterName(const AST::DeclarationNode *decl) const
    {
        auto fit = m_declToregName.find(decl);
        assert(fit != m_declToregName.end());
        return fit->second;
    }

    void insert(const std::string &regName, const AST::DeclarationNode *decl)
    {
        assert(!hasRegisterName(regName));
        assert(!hasDeclaration(decl));

        m_regNameToDecl.emplace(regName, decl);
        m_declToregName.emplace(decl, regName);

        assert(m_regNameToDecl.size() == m_declToregName.size());
    }

  public:
    const NameToDeclHashTable &getRegNameToDeclMapping() const { return m_regNameToDecl; }
    const DeclToNameHashTable &getDeclToregNameMapping() const { return m_declToregName; }

  public:
    void print() const
    {
        for (const auto &p : m_regNameToDecl)
        {
            printf("%s : ", p.first.c_str());
            ast_print(const_cast<AST::DeclarationNode *>(p.second));
            printf("\n");
        }
    }

    void sendToAssemblyDB(MeowAssemblyDatabase &assemblyDB) const
    {
        for (const auto &p : m_regNameToDecl)
        {
            RegType regType(p.second->getType());
            assemblyDB.declareUserRegister(p.first, regType);
        }
    }
};

DeclaredSymbolRegisterTable createDeclaredSymbolRegisterTable(AST::ASTNode *astNode)
{
    class SymbolDeclVisitor : public AST::Visitor
    {
      private:
        const char *m_symbolNamePrefix = "$";

      public:
        DeclaredSymbolRegisterTable m_declaredSymbolRegisterTable;

      private:
        virtual void preNodeVisit(AST::DeclarationNode *declarationNode)
        {
            std::string symbolDeclaredName = declarationNode->getName();
            std::string symbolRegisterName = m_symbolNamePrefix + symbolDeclaredName;

            std::string symbolRegisterResolvedName = symbolRegisterName;

            // avoid same symbol declaration name
            unsigned duplicate_count = 0;
            do
            {
                symbolRegisterResolvedName = symbolRegisterName + "_" + std::to_string(duplicate_count);
                duplicate_count++;
            } while (m_declaredSymbolRegisterTable.hasRegisterName(symbolRegisterResolvedName));

            m_declaredSymbolRegisterTable.insert(symbolRegisterResolvedName, declarationNode);
        }
    };

    SymbolDeclVisitor symbolDeclVisitor;
    astNode->visit(symbolDeclVisitor);

    return symbolDeclVisitor.m_declaredSymbolRegisterTable;
}

class ConstQualifiedExpressionReducer : public AST::Visitor
{
  private:
    const DeclaredSymbolRegisterTable &m_declaredSymbolRegisterTable;

    std::string m_assemblyValue; // for const qualifed values
    bool m_successful = true;

  private:
    ConstQualifiedExpressionReducer(const DeclaredSymbolRegisterTable &declaredSymbolRegisterTable) : m_declaredSymbolRegisterTable(declaredSymbolRegisterTable) {}

  private:
    virtual void postNodeVisit(AST::UnaryExpressionNode *unaryExpressionNode)
    {
        m_successful = false;
    }

    virtual void postNodeVisit(AST::BinaryExpressionNode *binaryExpressionNode)
    {
        m_successful = false;
    }

    virtual void postNodeVisit(AST::IntLiteralNode *intLiteralNode)
    {
        m_assemblyValue += std::to_string(intLiteralNode->getVal());
    }

    virtual void postNodeVisit(AST::FloatLiteralNode *floatLiteralNode)
    {
        m_assemblyValue += std::to_string(floatLiteralNode->getVal());
    }

    virtual void postNodeVisit(AST::BooleanLiteralNode *booleanLiteralNode)
    {
        m_assemblyValue += std::to_string(booleanLiteralNode->getVal() ? 1 : 0);
    }

    virtual void postNodeVisit(AST::IdentifierNode *identifierNode)
    {
        const AST::DeclarationNode *decl = identifierNode->getDeclaration();
        if (!decl->isConst())
        {
            m_successful = false;
            return;
        }

        // const type
        AST::ExpressionNode *initExpr = (decl->getInitValue()) ? decl->getInitValue() : decl->getExpression();
        initExpr->visit(*this);
    }

    virtual void postNodeVisit(AST::FunctionNode *functionNode)
    {
        m_successful = false;
    }

  private:
    virtual void nodeVisit(AST::ExpressionsNode *expressionsNode)
    {
        int counter = 0;
        for (AST::ExpressionNode *expr : expressionsNode->getExpressionList())
        {
            if (counter != 0)
            {
                m_assemblyValue += ",";
            }
            expr->visit(*this);
            counter++;
        }
    }
    virtual void nodeVisit(AST::FunctionNode *functionNode) {}

  public:
    /* Reduce the expression to value */
    static std::string reduceToValue(const DeclaredSymbolRegisterTable &declaredSymbolRegisterTable, AST::ExpressionNode *expr)
    {
        assert(expr);
        ConstQualifiedExpressionReducer reducer(declaredSymbolRegisterTable);
        expr->visit(reducer);
        if (reducer.m_successful)
        {
            return reducer.m_assemblyValue;
        }
        else
        {
            return "0";
        }
    }
};

class ExpressionReducer : public AST::Visitor
{
  private:
    const DeclaredSymbolRegisterTable &m_declaredSymbolRegisterTable;
    MeowAssemblyDatabase &m_assemblyDB;

    Register m_resultReg;

  private:
    ExpressionReducer(const DeclaredSymbolRegisterTable &declaredSymbolRegisterTable, MeowAssemblyDatabase &assemblyDB) : m_declaredSymbolRegisterTable(declaredSymbolRegisterTable), m_assemblyDB(assemblyDB) {}

  private:
    /* Disable traversal, we do reduction here */
    virtual void nodeVisit(AST::UnaryExpressionNode *unaryExpressionNode);
    virtual void nodeVisit(AST::BinaryExpressionNode *binaryExpressionNode);
    virtual void nodeVisit(AST::IntLiteralNode *intLiteralNode);
    virtual void nodeVisit(AST::FloatLiteralNode *floatLiteralNode);
    virtual void nodeVisit(AST::BooleanLiteralNode *booleanLiteralNode);
    virtual void nodeVisit(AST::IdentifierNode *identifierNode);
    virtual void nodeVisit(AST::FunctionNode *functionNode);

  public:
    /* Reduce the expression into a register containing the expression result */
    static Register reduce(const DeclaredSymbolRegisterTable &declaredSymbolRegisterTable,
                           MeowAssemblyDatabase &assemblyDB,
                           AST::ExpressionNode *expr);
};

void ExpressionReducer::nodeVisit(AST::UnaryExpressionNode *unaryExpressionNode)
{
    Register rhsReg = reduce(m_declaredSymbolRegisterTable,
                             m_assemblyDB,
                             unaryExpressionNode->getExpression());

    int exprType = unaryExpressionNode->getExpressionType();
    m_resultReg = m_assemblyDB.requestTempRegister(RegType(exprType));

    switch (unaryExpressionNode->getOperator())
    {
    case MINUS:
    {
        if (exprType == INT_T)
        {
            Register zeroReg = m_assemblyDB.requestTempIntRegister();
            m_assemblyDB.insertInstruction(new UInst(UInst::FnCode::OP_LLI_I, zeroReg, "0"));
            m_assemblyDB.insertInstruction(new RInst(RInst::FnCode::OP_SUB, m_resultReg, zeroReg, rhsReg));
        }
        else if (exprType == FLOAT_T)
        {
            Register zeroReg = m_assemblyDB.requestTempFloatRegister();
            m_assemblyDB.insertInstruction(new UInst(UInst::FnCode::OP_LLI_F, zeroReg, "0"));
            m_assemblyDB.insertInstruction(new RInst(RInst::FnCode::OP_SUB_F, m_resultReg, zeroReg, rhsReg));
        }

        break;
    }
    case NOT:
    {
        Register oneReg = m_assemblyDB.requestTempIntRegister();
        m_assemblyDB.insertInstruction(new UInst(UInst::FnCode::OP_LLI_I, oneReg, "1"));
        m_assemblyDB.insertInstruction(new RInst(RInst::FnCode::OP_CMPLT_I, m_resultReg, rhsReg, oneReg));

        break;
    }
    case BNOT:
    {
        Register zeroReg = m_assemblyDB.requestTempIntRegister();
        m_assemblyDB.insertInstruction(new UInst(UInst::FnCode::OP_LLI_I, zeroReg, "0"));
        Register oneReg = m_assemblyDB.requestTempIntRegister();
        m_assemblyDB.insertInstruction(new UInst(UInst::FnCode::OP_LLI_I, oneReg, "1"));
        Register negOneReg = m_assemblyDB.requestTempIntRegister();
        m_assemblyDB.insertInstruction(new RInst(RInst::FnCode::OP_SUB, negOneReg, zeroReg, oneReg));
        m_assemblyDB.insertInstruction(new RInst(RInst::FnCode::OP_XOR, m_resultReg, negOneReg, rhsReg));

        break;
    }
    }
}

void ExpressionReducer::nodeVisit(AST::BinaryExpressionNode *binaryExpressionNode)
{
    int op = binaryExpressionNode->getOperator();
    Register lhsReg = reduce(m_declaredSymbolRegisterTable,
                             m_assemblyDB,
                             binaryExpressionNode->getLeftExpression());

    Register rhsReg;
    // Reduce rhs to reg if and only if the operation is not BSL or BSR
    if (!(op == BSL || op == BSR))
    {
        rhsReg = reduce(m_declaredSymbolRegisterTable,
                        m_assemblyDB,
                        binaryExpressionNode->getRightExpression());
    }

    int exprType = binaryExpressionNode->getExpressionType();
    m_resultReg = m_assemblyDB.requestTempRegister(RegType(exprType));

    switch (op)
    {
    case AND:
        m_assemblyDB.insertInstruction(new RInst(RInst::FnCode::OP_AND, m_resultReg, lhsReg, rhsReg));
        break;
    case OR:
        m_assemblyDB.insertInstruction(new RInst(RInst::FnCode::OP_OR, m_resultReg, lhsReg, rhsReg));
        break;
    case PLUS:
        if (exprType == INT_T)
        {
            m_assemblyDB.insertInstruction(new RInst(RInst::FnCode::OP_ADD, m_resultReg, lhsReg, rhsReg));
        }
        else if (exprType == FLOAT_T)
        {
            m_assemblyDB.insertInstruction(new RInst(RInst::FnCode::OP_ADD_F, m_resultReg, lhsReg, rhsReg));
        }

        break;
    case MINUS:
        if (exprType == INT_T)
        {
            m_assemblyDB.insertInstruction(new RInst(RInst::FnCode::OP_SUB, m_resultReg, lhsReg, rhsReg));
        }
        else if (exprType == FLOAT_T)
        {
            m_assemblyDB.insertInstruction(new RInst(RInst::FnCode::OP_SUB_F, m_resultReg, lhsReg, rhsReg));
        }

        break;
    case TIMES:
        if (exprType == INT_T)
        {
            m_assemblyDB.insertInstruction(new RInst(RInst::FnCode::OP_MUL, m_resultReg, lhsReg, rhsReg));
        }
        else if (exprType == FLOAT_T)
        {
            m_assemblyDB.insertInstruction(new RInst(RInst::FnCode::OP_MUL_F, m_resultReg, lhsReg, rhsReg));
        }

        break;
    case SLASH:
    {
        assert(0 && "Division is not supported!");
        break;
    }
    case EQL:
    {
        int operandType = binaryExpressionNode->getLeftExpression()->getExpressionType();
        if (operandType == INT_T)
        {
            m_assemblyDB.insertInstruction(new RInst(RInst::FnCode::OP_CMPEQ_I, m_resultReg, lhsReg, rhsReg));
        }
        else if (operandType == FLOAT_T)
        {
            m_assemblyDB.insertInstruction(new RInst(RInst::FnCode::OP_CMPEQ_F, m_resultReg, lhsReg, rhsReg));
        }

        break;
    }
    case NEQ:
    {
        int operandType = binaryExpressionNode->getLeftExpression()->getExpressionType();
        if (operandType == INT_T)
        {
            m_assemblyDB.insertInstruction(new RInst(RInst::FnCode::OP_CMPNE_I, m_resultReg, lhsReg, rhsReg));
        }
        else if (operandType == FLOAT_T)
        {
            m_assemblyDB.insertInstruction(new RInst(RInst::FnCode::OP_CMPNE_F, m_resultReg, lhsReg, rhsReg));
        }

        break;
    }
    case LSS:
    {
        int operandType = binaryExpressionNode->getLeftExpression()->getExpressionType();
        if (operandType == INT_T)
        {
            m_assemblyDB.insertInstruction(new RInst(RInst::FnCode::OP_CMPLT_I, m_resultReg, lhsReg, rhsReg));
        }
        else if (operandType == FLOAT_T)
        {
            m_assemblyDB.insertInstruction(new RInst(RInst::FnCode::OP_CMPLT_F, m_resultReg, lhsReg, rhsReg));
        }

        break;
    }
    case GEQ:
    {
        int operandType = binaryExpressionNode->getLeftExpression()->getExpressionType();
        if (operandType == INT_T)
        {
            m_assemblyDB.insertInstruction(new RInst(RInst::FnCode::OP_CMPGE_I, m_resultReg, lhsReg, rhsReg));
        }
        else if (operandType == FLOAT_T)
        {
            m_assemblyDB.insertInstruction(new RInst(RInst::FnCode::OP_CMPGE_F, m_resultReg, lhsReg, rhsReg));
        }

        break;
    }
    case GTR:
    {
        int operandType = binaryExpressionNode->getLeftExpression()->getExpressionType();
        if (operandType == INT_T)
        {
            m_assemblyDB.insertInstruction(new RInst(RInst::FnCode::OP_CMPGT_I, m_resultReg, lhsReg, rhsReg));
        }
        else if (operandType == FLOAT_T)
        {
            m_assemblyDB.insertInstruction(new RInst(RInst::FnCode::OP_CMPGT_F, m_resultReg, lhsReg, rhsReg));
        }

        break;
    }
    case LEQ:
    {
        int operandType = binaryExpressionNode->getLeftExpression()->getExpressionType();
        if (operandType == INT_T)
        {
            m_assemblyDB.insertInstruction(new RInst(RInst::FnCode::OP_CMPLE_I, m_resultReg, lhsReg, rhsReg));
        }
        else if (operandType == FLOAT_T)
        {
            m_assemblyDB.insertInstruction(new RInst(RInst::FnCode::OP_CMPLE_F, m_resultReg, lhsReg, rhsReg));
        }

        break;
    }
    case BXOR:
    {
        m_assemblyDB.insertInstruction(new RInst(RInst::FnCode::OP_XOR, m_resultReg, lhsReg, rhsReg));
        break;
    }
    case BAND:
    {
        m_assemblyDB.insertInstruction(new RInst(RInst::FnCode::OP_AND, m_resultReg, lhsReg, rhsReg));
        break;
    }
    case BOR:
    {
        m_assemblyDB.insertInstruction(new RInst(RInst::FnCode::OP_OR, m_resultReg, lhsReg, rhsReg));
        break;
    }
    case BSL:
    {
        std::string reducedValString = ConstQualifiedExpressionReducer::reduceToValue(m_declaredSymbolRegisterTable, binaryExpressionNode->getRightExpression());
        m_assemblyDB.insertInstruction(new IInst(IInst::FnCode::OP_SHL, m_resultReg, lhsReg, reducedValString));
        break;
    }
    case BSR:
    {
        std::string reducedValString = ConstQualifiedExpressionReducer::reduceToValue(m_declaredSymbolRegisterTable, binaryExpressionNode->getRightExpression());
        m_assemblyDB.insertInstruction(new IInst(IInst::FnCode::OP_ASHR, m_resultReg, lhsReg, reducedValString));
        break;
    }
    default:
        assert(0);
    }
}

void ExpressionReducer::nodeVisit(AST::IntLiteralNode *intLiteralNode)
{
    m_resultReg = m_assemblyDB.requestTempIntRegister();
    std::string reducedValString = ConstQualifiedExpressionReducer::reduceToValue(m_declaredSymbolRegisterTable, intLiteralNode);
    m_assemblyDB.insertInstruction(new UInst(UInst::FnCode::OP_LLI_I, m_resultReg, reducedValString));
}

void ExpressionReducer::nodeVisit(AST::FloatLiteralNode *floatLiteralNode)
{
    m_resultReg = m_assemblyDB.requestTempFloatRegister();
    std::string reducedValString = ConstQualifiedExpressionReducer::reduceToValue(m_declaredSymbolRegisterTable, floatLiteralNode);
    m_assemblyDB.insertInstruction(new UInst(UInst::FnCode::OP_LLI_F, m_resultReg, reducedValString));
}

void ExpressionReducer::nodeVisit(AST::BooleanLiteralNode *booleanLiteralNode)
{
    m_resultReg = m_assemblyDB.requestTempIntRegister();
    std::string reducedValString = ConstQualifiedExpressionReducer::reduceToValue(m_declaredSymbolRegisterTable, booleanLiteralNode);
    m_assemblyDB.insertInstruction(new UInst(UInst::FnCode::OP_LLI_I, m_resultReg, reducedValString));
}

void ExpressionReducer::nodeVisit(AST::IdentifierNode *identifierNode)
{
    std::string regName = m_declaredSymbolRegisterTable.getRegisterName(identifierNode->getDeclaration());
    m_resultReg = m_assemblyDB.getUserRegister(regName);
}

void ExpressionReducer::nodeVisit(AST::FunctionNode *functionNode)
{
    const std::string &funcName = functionNode->getName();
    AST::ExpressionsNode *exprs = functionNode->getArgumentExpressions();
    if (funcName == "store")
    {
        assert(exprs->getNumberExpression() == 3);
        Register dataReg = reduce(m_declaredSymbolRegisterTable, m_assemblyDB, exprs->getExpressionAt(0));
        Register addrReg = reduce(m_declaredSymbolRegisterTable, m_assemblyDB, exprs->getExpressionAt(1));
        std::string reducedValString = ConstQualifiedExpressionReducer::reduceToValue(m_declaredSymbolRegisterTable, exprs->getExpressionAt(2));

        if (exprs->getExpressionAt(0)->getExpressionType() == INT_T)
        {
            m_assemblyDB.insertInstruction(new SInst(SInst::FnCode::MEMOP_STORE_I, addrReg, dataReg, reducedValString));
        }
        else if (exprs->getExpressionAt(0)->getExpressionType() == FLOAT_T)
        {
            m_assemblyDB.insertInstruction(new SInst(SInst::FnCode::MEMOP_STORE_F, addrReg, dataReg, reducedValString));
        }
        else
        {
            assert(0);
        }

        // Fake output register
        m_resultReg = m_assemblyDB.requestTempIntRegister();
        m_assemblyDB.insertInstruction(new UInst(UInst::FnCode::OP_LLI_I, m_resultReg, "0"));
    }
    else if (funcName == "blkdim")
    {
        Register dontCareReg = m_assemblyDB.getTempIntRegister();
        m_resultReg = m_assemblyDB.requestTempIntRegister();
        m_assemblyDB.insertInstruction(new IInst(IInst::FnCode::SYS_BLOCK_DIM, m_resultReg, dontCareReg, "233"));
    }
    else if (funcName == "blkidx")
    {
        Register dontCareReg = m_assemblyDB.getTempIntRegister();
        m_resultReg = m_assemblyDB.requestTempIntRegister();
        m_assemblyDB.insertInstruction(new IInst(IInst::FnCode::SYS_BLOCK_IDX, m_resultReg, dontCareReg, "233"));
    }
    else if (funcName == "tidx")
    {
        Register dontCareReg = m_assemblyDB.getTempIntRegister();
        m_resultReg = m_assemblyDB.requestTempIntRegister();
        m_assemblyDB.insertInstruction(new IInst(IInst::FnCode::SYS_THREAD_IDX, m_resultReg, dontCareReg, "233"));
    }
    else if (funcName == "exit")
    {
        Register dontCareReg = m_assemblyDB.getTempIntRegister();
        m_assemblyDB.insertInstruction(new SBInst(SBInst::FnCode::EXIT, dontCareReg, dontCareReg, "233"));

        // Fake output register
        m_resultReg = m_assemblyDB.requestTempIntRegister();
        m_assemblyDB.insertInstruction(new UInst(UInst::FnCode::OP_LLI_I, m_resultReg, "0"));
    }
    else
    {
        assert(0);
    }
}

Register ExpressionReducer::reduce(const DeclaredSymbolRegisterTable &declaredSymbolRegisterTable,
                                   MeowAssemblyDatabase &assemblyDB,
                                   AST::ExpressionNode *expr)
{
    assert(expr);
    ExpressionReducer reducer(declaredSymbolRegisterTable, assemblyDB);
    expr->visit(reducer);
    return reducer.m_resultReg;
}

void sendInstructionToAssemblyDB(MeowAssemblyDatabase &assemblyDB, const DeclaredSymbolRegisterTable &declaredSymbolRegisterTable, const SEMA::SourceContext &sourceContext, AST::ASTNode *ast)
{
    class StatementVisitor : public AST::Visitor
    {
      private:
        const DeclaredSymbolRegisterTable &m_declaredSymbolRegisterTable;
        const SEMA::SourceContext &m_sourceContext;
        MeowAssemblyDatabase &m_assemblyDB;

        std::string m_currentConditionReg;
        int m_ifScopeCount = 0;

      public:
        StatementVisitor(const DeclaredSymbolRegisterTable &declaredSymbolRegisterTable,
                         const SEMA::SourceContext &sourceContext,
                         MeowAssemblyDatabase &assemblyDB) : m_declaredSymbolRegisterTable(declaredSymbolRegisterTable), m_sourceContext(sourceContext), m_assemblyDB(assemblyDB) {}

      private:
        /* Disable traversal for expressions */
        virtual void nodeVisit(AST::ExpressionNode *expressionNode) {}
        virtual void nodeVisit(AST::ExpressionsNode *expressionsNode) {}
        virtual void nodeVisit(AST::UnaryExpressionNode *unaryExpressionNode) {}
        virtual void nodeVisit(AST::BinaryExpressionNode *binaryExpressionNode) {}
        virtual void nodeVisit(AST::IntLiteralNode *intLiteralNode) {}
        virtual void nodeVisit(AST::FloatLiteralNode *floatLiteralNode) {}
        virtual void nodeVisit(AST::BooleanLiteralNode *booleanLiteralNode) {}
        virtual void nodeVisit(AST::VariableNode *variableNode) {}
        virtual void nodeVisit(AST::IdentifierNode *identifierNode) {}
        virtual void nodeVisit(AST::FunctionNode *functionNode) {}

        virtual void nodeVisit(AST::IfStatementNode *ifStatementNode)
        {
            m_assemblyDB.insertInstructionComment("");
            m_assemblyDB.insertInstructionComment("Evaluate if statement condition");

            int firstline = ifStatementNode->getConditionExpression()->getSourceLocation().firstLine;
            int lastline = ifStatementNode->getConditionExpression()->getSourceLocation().lastLine;
            for (int line = firstline; line <= lastline; line++)
            {
                m_assemblyDB.insertInstructionComment(std::to_string(line) + ":" + m_sourceContext.getLine(line));
            }

            Register condReg = ExpressionReducer::reduce(m_declaredSymbolRegisterTable, m_assemblyDB,
                                                         ifStatementNode->getConditionExpression());
            m_assemblyDB.insertInstruction(new SBInst(SBInst::FnCode::IF, condReg, condReg, "1234"));

            m_assemblyDB.saveRegisterContext();
            m_assemblyDB.insertInstructionComment("");
            m_assemblyDB.insertInstructionComment("Into if statement then block");
            ifStatementNode->getThenStatement()->visit(*this);

            if (ifStatementNode->getElseStatement() != nullptr)
            {
                m_assemblyDB.loadRegisterContext();
                m_assemblyDB.insertInstructionComment("");
                m_assemblyDB.insertInstructionComment("Into if statement else block");
                Register fakeReg = m_assemblyDB.requestTempIntRegister();
                m_assemblyDB.insertInstruction(new SBInst(SBInst::FnCode::ELSE, fakeReg, fakeReg, "1234"));

                ifStatementNode->getElseStatement()->visit(*this);
            }

            m_assemblyDB.loadRegisterContext();
            m_assemblyDB.discardRegisterContext();
            m_assemblyDB.insertInstructionComment("");
            m_assemblyDB.insertInstructionComment("End if");
            Register fakeReg = m_assemblyDB.requestTempIntRegister();
            m_assemblyDB.insertInstruction(new SBInst(SBInst::FnCode::ENDIF, fakeReg, fakeReg, "1234"));
        }

        virtual void nodeVisit(AST::DeclarationNode *declarationNode)
        {
            AST::ExpressionNode *initExpr = declarationNode->getExpression();
            if (initExpr)
            {
                m_assemblyDB.insertInstructionComment("");

                int firstline = declarationNode->getSourceLocation().firstLine;
                int lastline = declarationNode->getSourceLocation().lastLine;
                for (int line = firstline; line <= lastline; line++)
                {
                    m_assemblyDB.insertInstructionComment(std::to_string(line) + ":" + m_sourceContext.getLine(line));
                }

                AST::ExpressionNode *optimizedInitExpr = declarationNode->getInitValue();
                Register rhsReg = ExpressionReducer::reduce(m_declaredSymbolRegisterTable, m_assemblyDB, optimizedInitExpr ? optimizedInitExpr : initExpr);
                Register lhsReg = m_assemblyDB.createUserRegister(m_declaredSymbolRegisterTable.getRegisterName(declarationNode));
                m_assemblyDB.insertInstruction(new IInst(IInst::FnCode::OP_ADDI, lhsReg, rhsReg, "0"));
            } else {
                m_assemblyDB.createUserRegister(m_declaredSymbolRegisterTable.getRegisterName(declarationNode));
            }
        }

        virtual void nodeVisit(AST::AssignmentNode *assignmentNode)
        {
            m_assemblyDB.insertInstructionComment("");

            int firstline = assignmentNode->getSourceLocation().firstLine;
            int lastline = assignmentNode->getSourceLocation().lastLine;
            for (int line = firstline; line <= lastline; line++)
            {
                m_assemblyDB.insertInstructionComment(std::to_string(line) + ":" + m_sourceContext.getLine(line));
            }

            AST::VariableNode *lhsVar = assignmentNode->getVariable();
            AST::ExpressionNode *rhsExpr = assignmentNode->getExpression();

            Register rhsReg = ExpressionReducer::reduce(m_declaredSymbolRegisterTable, m_assemblyDB, rhsExpr);
            Register lhsReg = m_assemblyDB.getUserRegister(m_declaredSymbolRegisterTable.getRegisterName(lhsVar->getDeclaration()));
            m_assemblyDB.insertInstruction(new IInst(IInst::FnCode::OP_ADDI, lhsReg, rhsReg, "0"));
        }
    };

    StatementVisitor visitor(declaredSymbolRegisterTable, sourceContext, assemblyDB);
    ast->visit(visitor);
}

} // namespace COGEN

int genCode(node *ast)
{
    SEMA::SourceContext sourceContext(inputFile);
    COGEN::MeowAssemblyDatabase assemblyDB;
    COGEN::DeclaredSymbolRegisterTable declaredSymbolRegisterTable =
        COGEN::createDeclaredSymbolRegisterTable(static_cast<AST::ASTNode *>(ast));

    declaredSymbolRegisterTable.sendToAssemblyDB(assemblyDB);
    COGEN::sendInstructionToAssemblyDB(assemblyDB, declaredSymbolRegisterTable, sourceContext, ast);

    // printf("\n");
    // assemblyDB.dump();
    if (dumpInstructions)
    {
        assemblyDB.output(dumpFile);
    }

    return 0;
}