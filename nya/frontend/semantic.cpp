#include "semantic.h"

#include "ast.h"
#include "symbol.h"
#include "data_container.h"
#include "source_context.h"

#include "common.h"
#include "parser.tab.h"

#include <algorithm>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <array>
#include <algorithm>
#include <sstream>

#include <cmath>
#include <cassert>

///////////////////////////////////////
// #define NORMAL_MODE
///////////////////////////////////////

namespace SEMA
{ /* START NAMESPACE */

class SemanticAnalyzer
{
  public:
    enum class EventType
    {
        Warning,
        Error,
        Unknown
    };

  public: /* Forward declarations for friend declaration */
    void setTempEventASTNode(const AST::ASTNode *astNode);
    void setTempEventEventType(EventType eventType);

  public:
    class Event
    {
      private:
        const AST::ASTNode *m_astNode = nullptr;
        EventType m_eventType = EventType::Unknown;

        AST::SourceLocation m_eventLoc;
        std::string m_message;

        bool m_useRef = false;
        AST::SourceLocation m_refLoc;
        std::string m_refMessage;

      public:
        Event(const AST::ASTNode *astNode, EventType eventType) : m_astNode(astNode), m_eventType(eventType) {}

      public:
        friend void SemanticAnalyzer::setTempEventASTNode(const AST::ASTNode *astNode);
        friend void SemanticAnalyzer::setTempEventEventType(EventType eventType);

      public:
        const AST::ASTNode *getASTNode() const { return m_astNode; }
        EventType getEventType() const { return m_eventType; }

        std::string &Message() { return m_message; }
        const std::string &Message() const { return m_message; }
        AST::SourceLocation &EventLoc() { return m_eventLoc; }
        const AST::SourceLocation &EventLoc() const { return m_eventLoc; }

        bool isUsingReference() const { return m_useRef; }
        void setUsingReference(bool useRef) { m_useRef = useRef; }
        std::string &RefMessage() { return m_refMessage; }
        const std::string &RefMessage() const { return m_refMessage; }
        AST::SourceLocation &RefLoc() { return m_refLoc; }
        const AST::SourceLocation &RefLoc() const { return m_refLoc; }
    };

    using EventID = size_t;

  private:
    std::vector<std::unique_ptr<Event>> m_eventList;
    std::unordered_map<const AST::ASTNode *, std::vector<EventID>> m_astEventLU;

    std::vector<EventID> m_errorEventList;
    std::vector<EventID> m_warningEventList;

  private:
    std::unique_ptr<Event> m_tempEvent = nullptr;
    bool m_tempEventValid = false;

  private:
    bool m_colorPrintEnabled = true;
    FILE *m_out = stdout;

  public:
    void resetAnalyzer()
    {
        m_eventList.clear();
        m_astEventLU.clear();
        m_errorEventList.clear();
        m_warningEventList.clear();
        m_tempEvent.reset();
        m_tempEventValid = false;
    }

  public:
    int getNumberEvents() const { return m_eventList.size(); }
    int getNumberErrors() const { return m_errorEventList.size(); }
    int getNumberWarnings() const { return m_warningEventList.size(); }

  public:
    EventID createEvent(const AST::ASTNode *astNode, EventType eventType);

    Event &getEvent(EventID eventID);
    const Event &getEventC(EventID eventID) const;
    const Event &getEvent(EventID eventID) const;

  private:
    void printEventNoColor(EventID eventID, const SourceContext &sourceContext) const;
    void printEventColor(EventID eventID, const SourceContext &sourceContext) const;

  public:
    bool isColorPrintEnabled() const { return m_colorPrintEnabled; }
    void setColorPrintEnabled(bool flag) { m_colorPrintEnabled = flag; }
    void setOutput(FILE *out) { m_out = out; }
    FILE *getOutput() const { return m_out; }
    void printEvent(EventID eventID, const SourceContext &sourceContext) const;

  public:
    void createTempEvent(const AST::ASTNode *astNode = nullptr, EventType eventType = EventType::Unknown);
    void dropTempEvent();
    EventID promoteTempEvent();
    Event &getTempEvent();
    const Event &getTempEventC() const;
    const Event &getTempEvent() const;

    bool isTempEventCreated() const { return m_tempEventValid; }
};

SemanticAnalyzer::EventID SemanticAnalyzer::createEvent(const AST::ASTNode *astNode, EventType eventType)
{
    assert(astNode != nullptr);
    assert(eventType != EventType::Unknown);

    m_eventList.emplace_back(new Event(astNode, eventType));
    EventID id = m_eventList.size() - 1;
    m_astEventLU[astNode].push_back(id);
    if (eventType == EventType::Error)
    {
        m_errorEventList.push_back(id);
    }
    else
    {
        m_warningEventList.push_back(id);
    }

    return id;
}

SemanticAnalyzer::Event &SemanticAnalyzer::getEvent(EventID eventID)
{
    return *(m_eventList.at(eventID));
}

const SemanticAnalyzer::Event &SemanticAnalyzer::getEventC(EventID eventID) const
{
    return *(m_eventList.at(eventID));
}

const SemanticAnalyzer::Event &SemanticAnalyzer::getEvent(EventID eventID) const
{
    return *(m_eventList.at(eventID));
}

void SemanticAnalyzer::printEventNoColor(EventID eventID, const SourceContext &sourceContext) const
{
    static const std::string tenSpace(10, ' ');
    static const std::string sevenSpace(7, ' ');

    const Event &event = getEvent(eventID);
    const AST::SourceLocation &eventLoc = event.EventLoc();

    fprintf(m_out, "--------------------------------------------------------------------------\n");

    // Print event message
    std::string header;
    if (event.getEventType() == EventType::Error)
    {
        header = "Error";
    }
    else
    {
        header = "Warning";
    }
    fprintf(m_out, "%s-%lu", header.c_str(), eventID);
    fprintf(m_out, ": %s\n", event.Message().c_str());

    // Print event location info
    if (AST::SourceLocation() != eventLoc)
    {
        if (eventLoc.firstLine == eventLoc.lastLine)
        {
            const std::string &line = sourceContext.getLine(eventLoc.firstLine);
            fprintf(m_out, "%7d:%s", eventLoc.firstLine, tenSpace.c_str());
            fprintf(m_out, "%s\n", line.c_str());
            fprintf(m_out, "%s %s", sevenSpace.c_str(), tenSpace.c_str());
            for (unsigned colNumber = 0; colNumber < line.length(); colNumber++)
            {
                if (static_cast<int>(colNumber) >= eventLoc.firstColumn - 1 && static_cast<int>(colNumber) < eventLoc.lastColumn - 1)
                {
                    fprintf(m_out, "^");
                }
                else
                {
                    fprintf(m_out, " ");
                }
            }
            fprintf(m_out, "\n");
        }
        else
        {
            for (int lineNumber = eventLoc.firstLine; lineNumber <= eventLoc.lastLine; lineNumber++)
            {
                const std::string &line = sourceContext.getLine(lineNumber);
                fprintf(m_out, "%7d:%s", lineNumber, tenSpace.c_str());
                fprintf(m_out, "%s\n", line.c_str());
            }
        }
    }

    // Print reference location info and reference message
    if (event.isUsingReference())
    {
        const AST::SourceLocation &refLoc = event.RefLoc();

        fprintf(m_out, "\n");

        // Print reference message
        fprintf(m_out, "Info");
        fprintf(m_out, ": %s\n", event.RefMessage().c_str());

        // Print reference location info
        if (AST::SourceLocation() != refLoc)
        {
            if (refLoc.firstLine == refLoc.lastLine)
            {
                const std::string &line = sourceContext.getLine(refLoc.firstLine);
                fprintf(m_out, "%7d:%s", refLoc.firstLine, tenSpace.c_str());
                fprintf(m_out, "%s\n", line.c_str());
                fprintf(m_out, "%s %s", sevenSpace.c_str(), tenSpace.c_str());
                for (unsigned colNumber = 0; colNumber < line.length(); colNumber++)
                {
                    if (static_cast<int>(colNumber) >= refLoc.firstColumn - 1 && static_cast<int>(colNumber) < refLoc.lastColumn - 1)
                    {
                        fprintf(m_out, "~");
                    }
                    else
                    {
                        fprintf(m_out, " ");
                    }
                }
                fprintf(m_out, "\n");
            }
            else
            {
                for (int lineNumber = refLoc.firstLine; lineNumber <= refLoc.lastLine; lineNumber++)
                {
                    const std::string &line = sourceContext.getLine(lineNumber);
                    fprintf(m_out, "%7d:%s", lineNumber, tenSpace.c_str());
                    fprintf(m_out, "%s\n", line.c_str());
                }
            }
        }
    }
}

void SemanticAnalyzer::printEventColor(EventID eventID, const SourceContext &sourceContext) const
{
    static const std::string tenSpace(10, ' ');
    static const std::string sevenSpace(7, ' ');

    const Event &event = getEvent(eventID);
    const AST::SourceLocation &eventLoc = event.EventLoc();

    printf("--------------------------------------------------------------------------\n");

    // Print event message
    if (event.getEventType() == EventType::Error)
    {
        printf("\033[1;31mError-%lu\033[0m", eventID);
    }
    else
    {
        printf("\033[1;33mWarning-%lu\033[0m", eventID);
    }
    printf("\033[1;39m: %s\033[0m\n", event.Message().c_str());

    // Print event location info
    if (AST::SourceLocation() != eventLoc)
    {
        if (eventLoc.firstLine == eventLoc.lastLine)
        {
            const std::string &line = sourceContext.getLine(eventLoc.firstLine);
            printf("\033[0;32m%7d:%s\033[0m", eventLoc.firstLine, tenSpace.c_str());
            printf("\033[1;37m%s\033[0m\n", line.c_str());
            printf("%s %s", sevenSpace.c_str(), tenSpace.c_str());
            for (unsigned colNumber = 0; colNumber < line.length(); colNumber++)
            {
                if (static_cast<int>(colNumber) >= eventLoc.firstColumn - 1 && static_cast<int>(colNumber) < eventLoc.lastColumn - 1)
                {
                    printf("\033[1;31m^\033[0m");
                }
                else
                {
                    printf(" ");
                }
            }
            printf("\n");
        }
        else
        {
            for (int lineNumber = eventLoc.firstLine; lineNumber <= eventLoc.lastLine; lineNumber++)
            {
                const std::string &line = sourceContext.getLine(lineNumber);
                printf("\033[0;32m%7d:%s\033[0m", lineNumber, tenSpace.c_str());
                printf("\033[1;37m%s\033[0m\n", line.c_str());
            }
        }
    }

    // Print reference location info and reference message
    if (event.isUsingReference())
    {
        const AST::SourceLocation &refLoc = event.RefLoc();

        printf("\n");

        // Print reference message
        printf("\033[1;37mInfo\033[0m");
        printf("\033[1;39m: %s\033[0m\n", event.RefMessage().c_str());

        // Print reference location info
        if (AST::SourceLocation() != refLoc)
        {
            if (refLoc.firstLine == refLoc.lastLine)
            {
                const std::string &line = sourceContext.getLine(refLoc.firstLine);
                printf("\033[0;32m%7d:%s\033[0m", refLoc.firstLine, tenSpace.c_str());
                printf("\033[1;37m%s\033[0m\n", line.c_str());
                printf("%s %s", sevenSpace.c_str(), tenSpace.c_str());
                for (unsigned colNumber = 0; colNumber < line.length(); colNumber++)
                {
                    if (static_cast<int>(colNumber) >= refLoc.firstColumn - 1 && static_cast<int>(colNumber) < refLoc.lastColumn - 1)
                    {
                        printf("\033[1;33m~\033[0m");
                    }
                    else
                    {
                        printf(" ");
                    }
                }
                printf("\n");
            }
            else
            {
                for (int lineNumber = refLoc.firstLine; lineNumber <= refLoc.lastLine; lineNumber++)
                {
                    const std::string &line = sourceContext.getLine(lineNumber);
                    printf("\033[0;32m%7d:%s\033[0m", lineNumber, tenSpace.c_str());
                    printf("\033[1;37m%s\033[0m\n", line.c_str());
                }
            }
        }
    }
}

void SemanticAnalyzer::printEvent(EventID eventID, const SourceContext &sourceContext) const
{
    if (isColorPrintEnabled())
    {
        printEventColor(eventID, sourceContext);
    }
    else
    {
        printEventNoColor(eventID, sourceContext);
    }
}

void SemanticAnalyzer::createTempEvent(const AST::ASTNode *astNode, EventType eventType)
{
    assert(m_tempEventValid == false);
    assert(m_tempEvent == nullptr);
    m_tempEventValid = true;
    m_tempEvent.reset(new Event(astNode, eventType));
}

void SemanticAnalyzer::setTempEventASTNode(const AST::ASTNode *astNode)
{
    assert(m_tempEventValid == true);
    assert(m_tempEvent != nullptr);
    m_tempEvent->m_astNode = astNode;
}

void SemanticAnalyzer::setTempEventEventType(EventType eventType)
{
    assert(m_tempEventValid == true);
    assert(m_tempEvent != nullptr);
    m_tempEvent->m_eventType = eventType;
}

void SemanticAnalyzer::dropTempEvent()
{
    assert(m_tempEventValid == true);
    assert(m_tempEvent != nullptr);
    m_tempEventValid = false;
    m_tempEvent.reset(nullptr);
}

SemanticAnalyzer::EventID SemanticAnalyzer::promoteTempEvent()
{
    assert(m_tempEventValid == true);
    assert(m_tempEvent != nullptr);

    EventType eventType = m_tempEvent->getEventType();
    const AST::ASTNode *astNode = m_tempEvent->getASTNode();
    assert(eventType != EventType::Unknown);
    assert(astNode != nullptr);

    m_tempEventValid = false;
    m_eventList.push_back(std::move(m_tempEvent));
    assert(m_tempEvent == nullptr);

    EventID id = m_eventList.size() - 1;

    m_astEventLU[astNode].push_back(id);
    if (eventType == EventType::Error)
    {
        m_errorEventList.push_back(id);
    }
    else
    {
        m_warningEventList.push_back(id);
    }

    return id;
}

SemanticAnalyzer::Event &SemanticAnalyzer::getTempEvent()
{
    assert(m_tempEventValid == true);
    assert(m_tempEvent != nullptr);
    return *m_tempEvent;
}

const SemanticAnalyzer::Event &SemanticAnalyzer::getTempEventC() const
{
    assert(m_tempEventValid == true);
    assert(m_tempEvent != nullptr);
    return *m_tempEvent;
}

const SemanticAnalyzer::Event &SemanticAnalyzer::getTempEvent() const
{
    assert(m_tempEventValid == true);
    assert(m_tempEvent != nullptr);
    return *m_tempEvent;
}

class SymbolDeclVisitor : public AST::Visitor
{
  private:
    ST::SymbolTable &m_symbolTable;
    SEMA::SemanticAnalyzer &m_semaAnalyzer;

  public:
    SymbolDeclVisitor(ST::SymbolTable &symbolTable, SEMA::SemanticAnalyzer &semaAnalyzer) : m_symbolTable(symbolTable), m_semaAnalyzer(semaAnalyzer) {}

  private:
    virtual void preNodeVisit(AST::IdentifierNode *identifierNode)
    {
        m_symbolTable.markSymbolRefPos(identifierNode);
    }

    virtual void preNodeVisit(AST::DeclarationNode *declarationNode)
    {
        AST::DeclarationNode *redecl = nullptr;

        // Declare the symbol before traversing the possible evaluation on rhs
        redecl = m_symbolTable.declareSymbol(declarationNode);

        if (redecl != nullptr)
        {
            std::stringstream ss;
            ss << "Duplicate declaration of '" << declarationNode->getQualifierString() << declarationNode->getTypeString() << " " << declarationNode->getName() << "' at " << declarationNode->getSourceLocationString() << ".";
            ss << " Previously declared at " << redecl->getSourceLocationString() << ".";

            auto id = m_semaAnalyzer.createEvent(declarationNode, SemanticAnalyzer::EventType::Error);
            m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
            m_semaAnalyzer.getEvent(id).EventLoc() = declarationNode->getSourceLocation();

            m_semaAnalyzer.getEvent(id).setUsingReference(true);
            m_semaAnalyzer.getEvent(id).RefMessage() = "Previously declared here:";
            m_semaAnalyzer.getEvent(id).RefLoc() = redecl->getSourceLocation();

            return;
        }
    }

    virtual void preNodeVisit(AST::NestedScopeNode *nestedScopeNode)
    {
        m_symbolTable.enterScope();
    }
    virtual void postNodeVisit(AST::NestedScopeNode *nestedScopeNode)
    {
        m_symbolTable.exitScope();
    }

    virtual void preNodeVisit(AST::ScopeNode *scopeNode)
    {
        m_symbolTable.enterScope();
    }
    virtual void postNodeVisit(AST::ScopeNode *scopeNode)
    {
        m_symbolTable.exitScope();
    }
};

DataTypeCategory getDataTypeCategory(int dataType)
{
    switch (dataType)
    {
    case BOOL_T:
        return DataTypeCategory::Boolean;
    case INT_T:
    case FLOAT_T:
        return DataTypeCategory::Arithmetic;
    default:
        assert(0);
    }
}

class TypeChecker : public AST::Visitor
{
  private:
    ST::SymbolTable &m_symbolTable;
    SEMA::SemanticAnalyzer &m_semaAnalyzer;

    int m_ifScopeCount = 0;

  public:
    TypeChecker(ST::SymbolTable &symbolTable, SEMA::SemanticAnalyzer &semaAnalyzer) : m_symbolTable(symbolTable), m_semaAnalyzer(semaAnalyzer) {}

  private:
    virtual void preNodeVisit(AST::IdentifierNode *identifierNode);
    virtual void preNodeVisit(AST::IfStatementNode *ifStatementNode);

  private:
    virtual void postNodeVisit(AST::UnaryExpressionNode *unaryExpressionNode);
    virtual void postNodeVisit(AST::BinaryExpressionNode *binaryExpressionNode);
    virtual void postNodeVisit(AST::FunctionNode *functionNode);
    virtual void postNodeVisit(AST::DeclarationNode *declarationNode);
    virtual void postNodeVisit(AST::IfStatementNode *ifStatementNode);
    virtual void postNodeVisit(AST::AssignmentNode *assignmentNode);

  private:
    int inferDataType(int op, int rhsDataType);
    int inferDataType(int op, int lhsDataType, int rhsDataType);
};

void TypeChecker::preNodeVisit(AST::IdentifierNode *identifierNode)
{
    AST::DeclarationNode *decl = m_symbolTable.getSymbolDecl(identifierNode);

    if (decl == nullptr)
    {
        // Update info in identifierNode
        identifierNode->setExpressionType(ANY_TYPE);
        identifierNode->setDeclaration(nullptr);

        std::stringstream ss;
        ss << "Missing declaration for symbol '" << identifierNode->getName() << "' at " << identifierNode->getSourceLocationString() << ".";

        auto id = m_semaAnalyzer.createEvent(identifierNode, SemanticAnalyzer::EventType::Error);
        m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
        m_semaAnalyzer.getEvent(id).EventLoc() = identifierNode->getSourceLocation();

        return;
    }

    assert(decl->getName() == identifierNode->getName());

    // Update info in identifierNode
    identifierNode->setExpressionType(decl->getType());
    identifierNode->setDeclaration(decl);
}

void TypeChecker::preNodeVisit(AST::IfStatementNode *ifStatementNode)
{
    assert(m_ifScopeCount >= 0);
    m_ifScopeCount++;
}

void TypeChecker::postNodeVisit(AST::UnaryExpressionNode *unaryExpressionNode)
{
    const AST::ExpressionNode *rhsExpr = unaryExpressionNode->getExpression();
    int rhsDataType = rhsExpr->getExpressionType();
    bool rhsIsConst = rhsExpr->isConst();
    int op = unaryExpressionNode->getOperator();

    int resultDataType = ANY_TYPE;
    if (rhsDataType != ANY_TYPE)
    {
        bool isLegal = true;

        // Type check
        m_semaAnalyzer.createTempEvent(unaryExpressionNode, SemanticAnalyzer::EventType::Error);

        resultDataType = inferDataType(op, rhsDataType);

        if (resultDataType == ANY_TYPE)
        {
            isLegal = false;

            std::stringstream ss;
            ss << "Operand in unary expression at " << unaryExpressionNode->getSourceLocationString() << " has non-compatible type at " << rhsExpr->getSourceLocationString() << ".";

            m_semaAnalyzer.getTempEvent().Message() = std::move(ss.str());
            m_semaAnalyzer.getTempEvent().EventLoc() = unaryExpressionNode->getSourceLocation();

            m_semaAnalyzer.promoteTempEvent();
        }
        else
        {
            m_semaAnalyzer.dropTempEvent();
        }

        if (!isLegal)
        {
            resultDataType = ANY_TYPE;
        }
    }

    unaryExpressionNode->setExpressionType(resultDataType);
    unaryExpressionNode->setConst(rhsIsConst);
}

void TypeChecker::postNodeVisit(AST::BinaryExpressionNode *binaryExpressionNode)
{
    const AST::ExpressionNode *lhsExpr = binaryExpressionNode->getLeftExpression();
    const AST::ExpressionNode *rhsExpr = binaryExpressionNode->getRightExpression();

    int lhsDataType = lhsExpr->getExpressionType();
    int rhsDataType = rhsExpr->getExpressionType();

    bool lhsIsConst = lhsExpr->isConst();
    bool rhsIsConst = rhsExpr->isConst();

    int op = binaryExpressionNode->getOperator();

    int resultDataType = ANY_TYPE;
    if (lhsDataType != ANY_TYPE && rhsDataType != ANY_TYPE)
    {
        bool isLegal = true;

        // Type check
        m_semaAnalyzer.createTempEvent(binaryExpressionNode, SemanticAnalyzer::EventType::Error);

        resultDataType = inferDataType(op, lhsDataType, rhsDataType);

        if (resultDataType != ANY_TYPE && (op == BSL || op == BSR))
        {
            // For BSL and BSR, rhs needs to be const.
            if (!rhsIsConst)
            {
                resultDataType = ANY_TYPE;

                m_semaAnalyzer.getTempEvent().setUsingReference(true);
                std::stringstream ss;
                ss << "Expression at rhs of operator '" << AST::getOperatorString(op) << "' at " << rhsExpr->getSourceLocationString() << " needs to be of constant int type:";

                m_semaAnalyzer.getTempEvent().RefMessage() = std::move(ss.str());
                m_semaAnalyzer.getTempEvent().RefLoc() = rhsExpr->getSourceLocation();
            }
        }

        if (resultDataType == ANY_TYPE)
        {
            isLegal = false;

            std::stringstream ss;
            ss << "Operands in binary expression at " << AST::getSourceLocationString(binaryExpressionNode->getSourceLocation()) << " have non-compatible type.";

            m_semaAnalyzer.getTempEvent().Message() = std::move(ss.str());
            m_semaAnalyzer.getTempEvent().EventLoc() = binaryExpressionNode->getSourceLocation();

            m_semaAnalyzer.promoteTempEvent();
        }
        else
        {
            m_semaAnalyzer.dropTempEvent();
        }

        if (!isLegal)
        {
            resultDataType = ANY_TYPE;
        }
    }

    binaryExpressionNode->setExpressionType(resultDataType);
    binaryExpressionNode->setConst(lhsIsConst && rhsIsConst);
}

void TypeChecker::postNodeVisit(AST::FunctionNode *functionNode)
{
    int resultDataType = ANY_TYPE;

    // Check for argument type
    const std::string &funcName = functionNode->getName();
    AST::ExpressionsNode *exprs = functionNode->getArgumentExpressions();
    const std::vector<AST::ExpressionNode *> &args = exprs->getExpressionList();

    if (funcName == "store")
    {
        // int store(float, int_addr, const_int_addr_offset)
        // int store(int, int_addr, const_int_addr_offset)

        bool isLegal = false;

        if (args.size() == 3)
        {
            int arg0Type = args[0]->getExpressionType();
            int arg1Type = args[1]->getExpressionType();
            int arg2Type = args[2]->getExpressionType();
            bool arg2IsConst = args[2]->isConst();
            if ((arg0Type == INT_T || arg0Type == FLOAT_T) && arg1Type == INT_T && arg2Type == INT_T && arg2IsConst)
            {
                resultDataType = INT_T;
                isLegal = true;
            }
        }

        if (!isLegal)
        {
            std::stringstream ss;
            ss << "Unmatched function parameters when calling function '" << funcName << "' at " << functionNode->getSourceLocationString() << ".";

            auto id = m_semaAnalyzer.createEvent(functionNode, SemanticAnalyzer::EventType::Error);
            m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
            m_semaAnalyzer.getEvent(id).EventLoc() = functionNode->getSourceLocation();

            m_semaAnalyzer.getEvent(id).setUsingReference(true);
            m_semaAnalyzer.getEvent(id).RefMessage() = "Expecting function arguments 'int/float, int, const int'.";
        }
    }
    else if (funcName == "blkdim" || funcName == "blkidx" || funcName == "tidx" || funcName == "exit")
    {
        // int blkdim()
        // int blkidx()
        // int tidx()
        // int exit()

        bool isLegal = false;

        if (args.size() == 0)
        {
            resultDataType = INT_T;
            isLegal = true;
        }

        if (!isLegal)
        {
            std::stringstream ss;
            ss << "Unmatched function parameters when calling function '" << funcName << "' at " << functionNode->getSourceLocationString() << ".";

            auto id = m_semaAnalyzer.createEvent(functionNode, SemanticAnalyzer::EventType::Error);
            m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
            m_semaAnalyzer.getEvent(id).EventLoc() = functionNode->getSourceLocation();

            m_semaAnalyzer.getEvent(id).setUsingReference(true);
            m_semaAnalyzer.getEvent(id).RefMessage() = "Expecting 0 function argument.";
        }
    }
    else
    {
        assert(0);
    }

    functionNode->setExpressionType(resultDataType);
}

void TypeChecker::postNodeVisit(AST::DeclarationNode *declarationNode)
{
    /*
     * Initialization
     * 
     * - const qualified variables must be initialized with a literal value or with a uniform
     * variable, not an expression.
     * - The value assigned to a variable (this includes variables declared with an initial value)
     * must have the same type as that variable, or a type that can be widened to the correct type.
     */
    AST::ExpressionNode *initExpr = declarationNode->getExpression();
    if (declarationNode->isConst())
    {
        if (initExpr == nullptr)
        {
            std::stringstream ss;
            ss << "Const qualified variable 'const " << declarationNode->getTypeString() << " " << declarationNode->getName() << "' is missing initialization at " << declarationNode->getSourceLocationString() << ".";

            auto id = m_semaAnalyzer.createEvent(declarationNode, SemanticAnalyzer::EventType::Error);
            m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
            m_semaAnalyzer.getEvent(id).EventLoc() = declarationNode->getSourceLocation();
        }
        else if (!initExpr->isConst())
        {
            std::stringstream ss;
            ss << "Const qualified variable 'const " << declarationNode->getTypeString() << " " << declarationNode->getName() << "' is initialized to a non-const qualified expression at " << initExpr->getSourceLocationString() << ".";

            auto id = m_semaAnalyzer.createEvent(declarationNode, SemanticAnalyzer::EventType::Error);
            m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
            m_semaAnalyzer.getEvent(id).EventLoc() = declarationNode->getSourceLocation();
        }
    }

    if (initExpr != nullptr)
    {
        int rhsDataType = initExpr->getExpressionType();
        int lhsDataType = declarationNode->getType();

        if (rhsDataType == ANY_TYPE)
        {
            std::stringstream ss;
            ss << "Variable declaration of '" << declarationNode->getQualifierString() << AST::getTypeString(lhsDataType) << " " << declarationNode->getName() << "' at " << declarationNode->getSourceLocationString() << ", is initialized to an unknown type at " << initExpr->getSourceLocationString() << " due to previous error(s).";

            auto id = m_semaAnalyzer.createEvent(declarationNode, SemanticAnalyzer::EventType::Error);
            m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
            m_semaAnalyzer.getEvent(id).EventLoc() = declarationNode->getSourceLocation();
        }
        else
        {
            // Type check
            if (lhsDataType != rhsDataType)
            {
                std::stringstream ss;
                ss << "Variable declaration of '" << declarationNode->getQualifierString() << AST::getTypeString(lhsDataType) << " " << declarationNode->getName() << "' at " << declarationNode->getSourceLocationString() << ", is initialized to a noncompatible type '" << AST::getTypeString(rhsDataType) << "' at " << initExpr->getSourceLocationString() << ".";

                auto id = m_semaAnalyzer.createEvent(declarationNode, SemanticAnalyzer::EventType::Error);
                m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
                m_semaAnalyzer.getEvent(id).EventLoc() = declarationNode->getSourceLocation();
            }
        }
    }
}

void TypeChecker::postNodeVisit(AST::IfStatementNode *ifStatementNode)
{
    /*
     * The expression that determines which branch of an if statement should be taken must
     * have the type bool (not bvec).
     */
    AST::ExpressionNode *cond = ifStatementNode->getConditionExpression();
    int condExprType = cond->getExpressionType();

    if (condExprType != BOOL_T)
    {
        std::stringstream ss;
        ss << "If-statement condition expression has ";
        if (condExprType == ANY_TYPE)
        {
            ss << "unknown type ";
        }
        else
        {
            ss << "type '" << AST::getTypeString(condExprType) << "' ";
        }
        ss << "at " << cond->getSourceLocationString() << ". Expecting type 'bool'.";

        auto id = m_semaAnalyzer.createEvent(ifStatementNode, SemanticAnalyzer::EventType::Error);
        m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
        m_semaAnalyzer.getEvent(id).EventLoc() = cond->getSourceLocation();
    }

    m_ifScopeCount--;
    assert(m_ifScopeCount >= 0);
}

void TypeChecker::postNodeVisit(AST::AssignmentNode *assignmentNode)
{
    /*
     * The value assigned to a variable (this includes variables declared with an initial value)
     * must have the same type as that variable, or a type that can be widened to the correct type.
     */
    const AST::ExpressionNode *rhsExpr = assignmentNode->getExpression();
    const AST::VariableNode *lhsVar = assignmentNode->getVariable();
    int rhsDataType = rhsExpr->getExpressionType();
    int lhsDataType = lhsVar->getExpressionType();

    int resultDataType = ANY_TYPE;
    bool assignmentLegal = true;

    if (lhsDataType == ANY_TYPE && rhsDataType == ANY_TYPE)
    {
        assignmentLegal = false;

        std::stringstream ss;
        ss << "Variable assignment for '" << lhsVar->getName() << "' at " << assignmentNode->getSourceLocationString() << ", has unknown type on both sides of assignment operator due to pervious errors.";

        auto id = m_semaAnalyzer.createEvent(assignmentNode, SemanticAnalyzer::EventType::Error);
        m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
        m_semaAnalyzer.getEvent(id).EventLoc() = assignmentNode->getSourceLocation();
    }
    else if (lhsDataType == ANY_TYPE && rhsDataType != ANY_TYPE)
    {
        assignmentLegal = false;

        std::stringstream ss;
        ss << "Variable assignment for '" << lhsVar->getName() << "' at " << assignmentNode->getSourceLocationString() << ", has variable of unknown type at " << lhsVar->getSourceLocationString() << " due to previous error.";

        auto id = m_semaAnalyzer.createEvent(assignmentNode, SemanticAnalyzer::EventType::Error);
        m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
        m_semaAnalyzer.getEvent(id).EventLoc() = assignmentNode->getSourceLocation();
    }
    else if (lhsDataType != ANY_TYPE && rhsDataType == ANY_TYPE)
    {
        assignmentLegal = false;

        std::stringstream ss;
        ss << "Variable assignment for '" << lhsVar->getName() << "' at " << assignmentNode->getSourceLocationString() << ", has expression of unknown type at " << rhsExpr->getSourceLocationString() << " due to previous error(s).";

        auto id = m_semaAnalyzer.createEvent(assignmentNode, SemanticAnalyzer::EventType::Error);
        m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
        m_semaAnalyzer.getEvent(id).EventLoc() = assignmentNode->getSourceLocation();
    }
    else
    {
        assert(rhsDataType != ANY_TYPE);
        assert(lhsDataType != ANY_TYPE);

        // Check for type
        if (lhsDataType != rhsDataType)
        {
            assignmentLegal = false;

            std::stringstream ss;
            ss << "Invalid variable assignment for '" << lhsVar->getName() << "' at " << assignmentNode->getSourceLocationString() << ", has expression of non-compatible type '" << rhsExpr->getExpressionQualifierString() << AST::getTypeString(rhsDataType) << "' at " << rhsExpr->getSourceLocationString() << ".";

            auto id = m_semaAnalyzer.createEvent(assignmentNode, SemanticAnalyzer::EventType::Error);
            m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
            m_semaAnalyzer.getEvent(id).EventLoc() = assignmentNode->getSourceLocation();

            const AST::DeclarationNode *decl = lhsVar->getDeclaration();
            assert(decl != nullptr);
            ss = std::stringstream();
            m_semaAnalyzer.getEvent(id).setUsingReference(true);
            ss << "Variable '" << lhsVar->getName() << "' at " << lhsVar->getSourceLocationString() << " has type '" << decl->getQualifierString() << AST::getTypeString(lhsDataType) << "', and is declared at " << decl->getSourceLocationString() << ":";

            m_semaAnalyzer.getEvent(id).RefMessage() = std::move(ss.str());
            m_semaAnalyzer.getEvent(id).RefLoc() = decl->getSourceLocation();
        }
    }

    if (lhsVar->isConst())
    {
        assignmentLegal = false;

        std::stringstream ss;
        ss << "Invalid variable assignment for const-qualified variable '" << lhsVar->getName() << "' at " << assignmentNode->getSourceLocationString() << ".";

        auto id = m_semaAnalyzer.createEvent(assignmentNode, SemanticAnalyzer::EventType::Error);
        m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
        m_semaAnalyzer.getEvent(id).EventLoc() = lhsVar->getSourceLocation();

        const AST::DeclarationNode *decl = lhsVar->getDeclaration();
        assert(decl != nullptr);
        ss.str(std::string());
        ss << "Variable '" << lhsVar->getName() << "' at " << lhsVar->getSourceLocationString() << " has type '" << decl->getQualifierString() << AST::getTypeString(lhsDataType) << "', and is declared at " << decl->getSourceLocationString() << ":";

        m_semaAnalyzer.getEvent(id).setUsingReference(true);
        m_semaAnalyzer.getEvent(id).RefMessage() = std::move(ss.str());
        m_semaAnalyzer.getEvent(id).RefLoc() = decl->getSourceLocation();
    }

    if (assignmentLegal)
    {
        assert(rhsDataType != ANY_TYPE);
        assert(lhsDataType != ANY_TYPE);
        assert(lhsDataType == rhsDataType);

        resultDataType = lhsDataType;
    }

    assignmentNode->setExpressionType(resultDataType);
}

int TypeChecker::inferDataType(int op, int rhsDataType)
{
    /* Unary Operator Type Inference */
    /*
     * - Arithmetic
     * ! Logical
     * ~ Arithmetic
     */

    if (rhsDataType == ANY_TYPE)
    {
        return ANY_TYPE;
    }

    DataTypeCategory typeCateg = getDataTypeCategory(rhsDataType);
    switch (op)
    {
    case MINUS:
    {
        if (typeCateg == DataTypeCategory::Arithmetic)
        {
            return rhsDataType;
        }

        if (m_semaAnalyzer.isTempEventCreated())
        {
            m_semaAnalyzer.getTempEvent().setUsingReference(true);
            m_semaAnalyzer.getTempEvent().RefMessage() = "Expecting arithmetic type on right-hand side of operator '-'";
        }

        break;
    }
    case NOT:
    {
        if (typeCateg == DataTypeCategory::Boolean)
        {
            return rhsDataType;
        }

        if (m_semaAnalyzer.isTempEventCreated())
        {
            m_semaAnalyzer.getTempEvent().setUsingReference(true);
            m_semaAnalyzer.getTempEvent().RefMessage() = "Expecting boolean type on right-hand side of operator '!'";
        }

        break;
    }
    case BNOT:
    {
        if (typeCateg == DataTypeCategory::Arithmetic)
        {
            return rhsDataType;
        }

        if (m_semaAnalyzer.isTempEventCreated())
        {
            m_semaAnalyzer.getTempEvent().setUsingReference(true);
            m_semaAnalyzer.getTempEvent().RefMessage() = "Expecting arithmetic type on right-hand side of operator '~'";
        }

        break;
    }
    default:
        assert(0);
    }

    return ANY_TYPE;
}

int TypeChecker::inferDataType(int op, int lhsDataType, int rhsDataType)
{
    /* Binary Operator Type Inference */
    /*
     * +, -                 Arithmetic
     * *, /                 Arithmetic
     * &&, ||               Logical
     * <, <=, >, >=         Comparison
     * ==, !=               Comparison
     * 
     * ^, &, |              Arithmetic with lhs and rhs being int
     * <<, >>               Arithmetic with lhs and rhs being int
     */

    if (lhsDataType == ANY_TYPE || rhsDataType == ANY_TYPE)
    {
        return ANY_TYPE;
    }

    if (lhsDataType != rhsDataType)
    {
        /*
         * Both operands of a binary operator must have exactly the same type
         */

        if (m_semaAnalyzer.isTempEventCreated())
        {
            m_semaAnalyzer.getTempEvent().setUsingReference(true);
            std::stringstream ss;
            ss << "Expecting operands on both sides of operator '";
            ss << AST::getOperatorString(op);
            ss << "' to have same type, but they are '";
            ss << AST::getTypeString(lhsDataType);
            ss << "' and '";
            ss << AST::getTypeString(rhsDataType);
            ss << "'.";
            m_semaAnalyzer.getTempEvent().RefMessage() = std::move(ss.str());
        }

        return ANY_TYPE;
    }

    DataTypeCategory lhsTypeCateg = getDataTypeCategory(lhsDataType);
    DataTypeCategory rhsTypeCateg = getDataTypeCategory(rhsDataType);
    switch (op)
    {
    case PLUS:
    case MINUS:
    case TIMES:
    case SLASH:
    {
        if (lhsTypeCateg == DataTypeCategory::Arithmetic && rhsTypeCateg == DataTypeCategory::Arithmetic)
        {
            // Both arithmetic type
            assert(lhsDataType == rhsDataType);
            return lhsDataType;
        }

        if (m_semaAnalyzer.isTempEventCreated())
        {
            m_semaAnalyzer.getTempEvent().setUsingReference(true);
            std::stringstream ss;
            ss << "Expecting operands on both sides of operator '";
            ss << AST::getOperatorString(op);
            ss << "' to have arithmetic type, but they are '";
            ss << AST::getTypeString(lhsDataType);
            ss << "' and '";
            ss << AST::getTypeString(rhsDataType);
            ss << "'.";
            m_semaAnalyzer.getTempEvent().RefMessage() = std::move(ss.str());
        }

        break;
    }

    case AND:
    case OR:
    {
        if (lhsTypeCateg == DataTypeCategory::Boolean && rhsTypeCateg == DataTypeCategory::Boolean)
        {
            // Both boolean type
            assert(lhsDataType == rhsDataType);
            return lhsDataType;
        }

        if (m_semaAnalyzer.isTempEventCreated())
        {
            m_semaAnalyzer.getTempEvent().setUsingReference(true);
            std::stringstream ss;
            ss << "Expecting operands on both sides of operator '";
            ss << AST::getOperatorString(op);
            ss << "' to have boolean type, but they are '";
            ss << AST::getTypeString(lhsDataType);
            ss << "' and '";
            ss << AST::getTypeString(rhsDataType);
            ss << "'.";
            m_semaAnalyzer.getTempEvent().RefMessage() = std::move(ss.str());
        }

        break;
    }

    case LSS:
    case LEQ:
    case GTR:
    case GEQ:
    case EQL:
    case NEQ:
    {
        if (lhsTypeCateg == DataTypeCategory::Arithmetic && rhsTypeCateg == DataTypeCategory::Arithmetic)
        {
            // Both arithmetic type
            assert(lhsDataType == rhsDataType);
            return BOOL_T;
        }

        if (m_semaAnalyzer.isTempEventCreated())
        {
            m_semaAnalyzer.getTempEvent().setUsingReference(true);
            std::stringstream ss;
            ss << "Expecting operands on both sides of operator '";
            ss << AST::getOperatorString(op);
            ss << "' to have arithmetic type, but they are '";
            ss << AST::getTypeString(lhsDataType);
            ss << "' and '";
            ss << AST::getTypeString(rhsDataType);
            ss << "'.";
            m_semaAnalyzer.getTempEvent().RefMessage() = std::move(ss.str());
        }

        break;
    }

    case BXOR:
    case BAND:
    case BOR:
    case BSL:
    case BSR:
    {
        if (lhsDataType == INT_T && rhsDataType == INT_T)
        {
            return lhsDataType;
        }

        if (m_semaAnalyzer.isTempEventCreated())
        {
            m_semaAnalyzer.getTempEvent().setUsingReference(true);
            std::stringstream ss;
            ss << "Expecting operands on both sides of operator '";
            ss << AST::getOperatorString(op);
            ss << "' to have int type, but they are '";
            ss << AST::getTypeString(lhsDataType);
            ss << "' and '";
            ss << AST::getTypeString(rhsDataType);
            ss << "'.";
            m_semaAnalyzer.getTempEvent().RefMessage() = std::move(ss.str());
        }

        break;
    }

    default:
        assert(0);
    }

    return ANY_TYPE;
}

class ConstantExpressionEvaluator : public AST::Visitor
{
  private:
    bool m_evaluationSuccessful = true;
    DataContainer &m_data;

  private:
    ConstantExpressionEvaluator(DataContainer &data) : m_data(data) {}

  private:
    virtual void nodeVisit(AST::UnaryExpressionNode *unaryExpressionNode)
    {
        assert(m_data.getType() == unaryExpressionNode->getExpressionType());

        AST::ExpressionNode *rhsExpr = unaryExpressionNode->getExpression();
        DataContainer rhsData(rhsExpr->getExpressionType());
        m_evaluationSuccessful = evaluateValue(rhsExpr, rhsData);

        if (m_evaluationSuccessful)
        {
            switch (unaryExpressionNode->getOperator())
            {
            case MINUS:
                m_data = -rhsData;
                break;
            case NOT:
                m_data = !rhsData;
                break;
            case BNOT:
                m_data = ~rhsData;
                break;
            default:
                assert(0);
            }
        }
    }

    virtual void nodeVisit(AST::BinaryExpressionNode *binaryExpressionNode)
    {
        assert(m_data.getType() == binaryExpressionNode->getExpressionType());

        AST::ExpressionNode *lhsExpr = binaryExpressionNode->getLeftExpression();
        DataContainer lhsData(lhsExpr->getExpressionType());
        m_evaluationSuccessful = evaluateValue(lhsExpr, lhsData);

        AST::ExpressionNode *rhsExpr = binaryExpressionNode->getRightExpression();
        DataContainer rhsData(rhsExpr->getExpressionType());
        m_evaluationSuccessful &= evaluateValue(rhsExpr, rhsData);

        if (m_evaluationSuccessful)
        {
            switch (binaryExpressionNode->getOperator())
            {
            case AND:
                m_data = lhsData && rhsData;
                break;
            case OR:
                m_data = lhsData || rhsData;
                break;
            case PLUS:
                m_data = lhsData + rhsData;
                break;
            case MINUS:
                m_data = lhsData - rhsData;
                break;
            case TIMES:
                m_data = lhsData * rhsData;
                break;
            case SLASH:
                m_data = lhsData / rhsData;
                break;
            case EQL:
                m_data = lhsData == rhsData;
                break;
            case NEQ:
                m_data = lhsData != rhsData;
                break;
            case LSS:
                m_data = lhsData < rhsData;
                break;
            case LEQ:
                m_data = lhsData <= rhsData;
                break;
            case GTR:
                m_data = lhsData > rhsData;
                break;
            case GEQ:
                m_data = lhsData >= rhsData;
                break;
            case BXOR:
                m_data = lhsData ^ rhsData;
                break;
            case BAND:
                m_data = lhsData & rhsData;
                break;
            case BOR:
                m_data = lhsData | rhsData;
                break;
            case BSL:
                m_data = lhsData << rhsData;
                break;
            case BSR:
                m_data = lhsData >> rhsData;
                break;
            default:
                assert(0);
            }
        }
    }

    virtual void nodeVisit(AST::IntLiteralNode *intLiteralNode)
    {
        assert(m_data.getType() == INT_T);

        m_data.intVal() = intLiteralNode->getVal();
    }

    virtual void nodeVisit(AST::FloatLiteralNode *floatLiteralNode)
    {
        assert(m_data.getType() == FLOAT_T);

        m_data.floatVal() = floatLiteralNode->getVal();
    }

    virtual void nodeVisit(AST::BooleanLiteralNode *booleanLiteralNode)
    {
        assert(m_data.getType() == BOOL_T);

        m_data.boolVal() = booleanLiteralNode->getVal();
    }

    virtual void nodeVisit(AST::IdentifierNode *identifierNode)
    {
        assert(m_data.getType() == identifierNode->getExpressionType());

        const AST::DeclarationNode *decl = identifierNode->getDeclaration();
        assert(decl != nullptr);

        AST::ExpressionNode *identVal = decl->getInitValue();
        if (identVal == nullptr)
        {
            /* If somehow this variable does not have a evaluated initialization expression, abort */
            m_evaluationSuccessful = false;
            return;
        }

        m_evaluationSuccessful = evaluateValue(identVal, m_data);
    }

    virtual void nodeVisit(AST::FunctionNode *functionNode)
    {
        m_evaluationSuccessful = false;
    }

  public:
    static bool evaluateValue(AST::ExpressionNode *expr, DataContainer &data)
    {
        int dataType = expr->getExpressionType();
        assert(dataType != ANY_TYPE);
        assert(dataType == data.getType());

        ConstantExpressionEvaluator ev(data);
        expr->visit(ev);

        return ev.m_evaluationSuccessful;
    }
};

class ConstantDeclarationOptimizer : public AST::Visitor
{
  private:
    virtual void preNodeVisit(AST::DeclarationNode *declarationNode);

  private:
    /* Do not traverse into these nodes */
    virtual void nodeVisit(AST::DeclarationNode *declarationNode) {}

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
};

void ConstantDeclarationOptimizer::preNodeVisit(AST::DeclarationNode *declarationNode)
{
    /* Skip if declaration is not for a const-qualified variable */
    if (!declarationNode->isConst())
    {
        return;
    }

    AST::ExpressionNode *initExpr = declarationNode->getExpression();

    /* Skip if declaration does not have initialization */
    if (initExpr == nullptr)
    {
        return;
    }

    /* Skip if initialization is not well-defined */
    if (initExpr->getExpressionType() == ANY_TYPE)
    {
        return;
    }

    /* Skip if initialization is not a const-qualifed expression */
    if (!initExpr->isConst())
    {
        return;
    }

    /* Skip if type does not match */
    if (declarationNode->getType() != initExpr->getExpressionType())
    {
        return;
    }

    /* 
     * We have skipped a lot of invalid conditions that are not suitable for
     * constant folding and propagation, even some combinations of them
     * are not valid in terms of the lanague grammar rule.
     * However, it is okay to simply skip them, because these errors are caught
     * in other stages of semantic analysis.
     */
    DataContainer initData(initExpr->getExpressionType());
    bool constOptSuccessful = ConstantExpressionEvaluator::evaluateValue(initExpr, initData);
    if (constOptSuccessful)
    {
        fprintf(outputFile, "Info: Optimization for declaration of const-qualified symbol '%s' of type '%s%s' successful at %s.\n",
                declarationNode->getName().c_str(), declarationNode->getQualifierString().c_str(), declarationNode->getTypeString().c_str(),
                declarationNode->getSourceLocationString().c_str());

        AST::ExpressionNode *resultExpr = initData.createASTExpr();
        assert(resultExpr != nullptr);
        declarationNode->setInitValue(resultExpr);
    }
}

class VariableAssignmentLog
{
  private:
    VariableAssignmentLog *m_prev;
    std::unordered_set<const AST::DeclarationNode *> m_assignmentLU;

  public:
    VariableAssignmentLog(VariableAssignmentLog *prev) : m_prev(prev) {}

  public:
    void markAssigned(const AST::DeclarationNode *decl);
    bool checkAssigned(const AST::DeclarationNode *decl) const; // Checks recursively
    void merge(const VariableAssignmentLog &log1, const VariableAssignmentLog &log2);
    void merge(const VariableAssignmentLog &log);
};

void VariableAssignmentLog::markAssigned(const AST::DeclarationNode *decl)
{
    assert(decl);
    m_assignmentLU.insert(decl);
}

bool VariableAssignmentLog::checkAssigned(const AST::DeclarationNode *decl) const
{
    assert(decl);

    // Checks recursively
    if (m_assignmentLU.count(decl) == 1)
    {
        // If found, return immediately
        return true;
    }

    // If not found, check previous node
    if (m_prev != nullptr)
    {
        return m_prev->checkAssigned(decl);
    }
    else
    {
        // If not found and reaches the end
        return false;
    }
}

void VariableAssignmentLog::merge(const VariableAssignmentLog &log1, const VariableAssignmentLog &log2)
{
    std::unordered_set<const AST::DeclarationNode *> intersection;
    for (const AST::DeclarationNode *node : log1.m_assignmentLU)
    {
        if (log2.m_assignmentLU.count(node) == 1)
        {
            intersection.insert(node);
        }
    }

    this->m_assignmentLU.insert(intersection.begin(), intersection.end());
}

void VariableAssignmentLog::merge(const VariableAssignmentLog &log)
{
    this->m_assignmentLU.insert(log.m_assignmentLU.begin(), log.m_assignmentLU.end());
}

class VariableAssignmentChecker : public AST::Visitor
{
  private:
    SemanticAnalyzer &m_semanticAnalyzer;

  private:
    VariableAssignmentLog *m_currentFlowEdge = nullptr;

    const AST::DeclarationNode *m_currentDeclaration = nullptr;
    bool m_currentDeclarationRecursiveInit = false;

  public:
    VariableAssignmentChecker(SemanticAnalyzer &semanticAnalyzer) : m_semanticAnalyzer(semanticAnalyzer) {}

  private:
    virtual void preNodeVisit(AST::DeclarationNode *declarationNode);

  private:
    virtual void postNodeVisit(AST::AssignmentNode *assignmentNode);
    virtual void postNodeVisit(AST::IdentifierNode *identifierNode);
    virtual void postNodeVisit(AST::DeclarationNode *declarationNode);

  private:
    virtual void nodeVisit(AST::AssignmentNode *assignmentNode);
    virtual void nodeVisit(AST::IfStatementNode *ifStatementNode);
    virtual void nodeVisit(AST::ScopeNode *scopeNode);
};

void VariableAssignmentChecker::preNodeVisit(AST::DeclarationNode *declarationNode)
{
    assert(m_currentDeclaration == nullptr);
    m_currentDeclaration = declarationNode;
    m_currentDeclarationRecursiveInit = false;
}

void VariableAssignmentChecker::postNodeVisit(AST::AssignmentNode *assignmentNode)
{
    if (assignmentNode->getExpressionType() == ANY_TYPE)
    {
        return;
    }

    AST::VariableNode *var = assignmentNode->getVariable();
    const AST::DeclarationNode *decl = var->getDeclaration();
    assert(decl != nullptr);

    m_currentFlowEdge->markAssigned(decl);
}

void VariableAssignmentChecker::postNodeVisit(AST::IdentifierNode *identifierNode)
{
    if (identifierNode->getExpressionType() == ANY_TYPE)
    {
        return;
    }

    const AST::DeclarationNode *decl = identifierNode->getDeclaration();
    assert(decl != nullptr);

    if (!m_currentFlowEdge->checkAssigned(decl))
    {
        auto id = m_semanticAnalyzer.createEvent(identifierNode, SemanticAnalyzer::EventType::Warning);

        std::stringstream ss;
        ss << "Read of potentially unassigned variable '" << identifierNode->getName() << "' of type '" << identifierNode->getExpressionQualifierString() << identifierNode->getExpressionTypeString() << "' at " << identifierNode->getSourceLocationString() << ".";

        m_semanticAnalyzer.getEvent(id).Message() = std::move(ss.str());
        m_semanticAnalyzer.getEvent(id).EventLoc() = identifierNode->getSourceLocation();

        if (decl == m_currentDeclaration)
        {
            /* use of a variable within its own initialization */
            m_currentDeclarationRecursiveInit = true;
        }
    }
}

void VariableAssignmentChecker::postNodeVisit(AST::DeclarationNode *declarationNode)
{
    assert(m_currentDeclaration == declarationNode);
    m_currentDeclaration = nullptr;

    AST::ExpressionNode *initExpr = declarationNode->getExpression();
    if (initExpr == nullptr)
    {
        // Declaration without initialization
        return;
    }

    if (initExpr->getExpressionType() != declarationNode->getType())
    {
        return;
    }

    if (m_currentDeclarationRecursiveInit)
    {
        auto id = m_semanticAnalyzer.createEvent(declarationNode, SemanticAnalyzer::EventType::Warning);

        std::stringstream ss;
        ss << "Self-initialization involves read of potentially unassigned variable '" << declarationNode->getName() << "' of type '" << declarationNode->getQualifierString() << declarationNode->getTypeString() << "' at " << declarationNode->getSourceLocationString() << ".";

        m_semanticAnalyzer.getEvent(id).Message() = std::move(ss.str());
        m_semanticAnalyzer.getEvent(id).EventLoc() = declarationNode->getSourceLocation();

        m_currentDeclarationRecursiveInit = false;
    }
    else
    {
        m_currentFlowEdge->markAssigned(declarationNode);
    }
}

void VariableAssignmentChecker::nodeVisit(AST::AssignmentNode *assignmentNode)
{
    // Do not traverse into variable on lhs
    assignmentNode->getExpression()->visit(*this);
}

void VariableAssignmentChecker::nodeVisit(AST::IfStatementNode *ifStatementNode)
{
    AST::ExpressionNode *cond = ifStatementNode->getConditionExpression();
    cond->visit(*this);

    VariableAssignmentLog *commonParentEdge = m_currentFlowEdge;

    // Branch
    std::unique_ptr<VariableAssignmentLog> thenStmtEdge(new VariableAssignmentLog(commonParentEdge));
    m_currentFlowEdge = thenStmtEdge.get();
    ifStatementNode->getThenStatement()->visit(*this);

    // Branch
    std::unique_ptr<VariableAssignmentLog> elseStmtEdge(new VariableAssignmentLog(commonParentEdge));
    m_currentFlowEdge = elseStmtEdge.get();
    if (ifStatementNode->getElseStatement() != nullptr)
    {
        ifStatementNode->getElseStatement()->visit(*this);
    }

    // If there is a always true branch
    VariableAssignmentLog *alwaysTrueBranch = nullptr;
    if (cond->isConst() && cond->getExpressionType() == BOOL_T)
    {
        DataContainer condData(BOOL_T);
        bool successful = ConstantExpressionEvaluator::evaluateValue(cond, condData);

        if (successful)
        {
            alwaysTrueBranch = condData.getBoolVal() ? thenStmtEdge.get() : elseStmtEdge.get();
        }
    }

    // Merge
    if (alwaysTrueBranch == nullptr)
    {
        commonParentEdge->merge(*thenStmtEdge, *elseStmtEdge);
    }
    else
    {
        commonParentEdge->merge(*alwaysTrueBranch);
    }

    m_currentFlowEdge = commonParentEdge;
}

void VariableAssignmentChecker::nodeVisit(AST::ScopeNode *scopeNode)
{
    std::unique_ptr<VariableAssignmentLog> rootEdge(new VariableAssignmentLog(nullptr));
    m_currentFlowEdge = rootEdge.get();

    scopeNode->getDeclarations()->visit(*this);
    scopeNode->getStatements()->visit(*this);

    m_currentFlowEdge = nullptr;
}

} // namespace SEMA

int semantic_check(node *ast)
{
    ST::SymbolTable symbolTable;
    SEMA::SemanticAnalyzer semaAnalyzer;
    SEMA::SourceContext sourceContext(inputFile);

    /* Construct Symbol Tree */
    SEMA::SymbolDeclVisitor symbolDeclVisitor(symbolTable, semaAnalyzer);
    static_cast<AST::ASTNode *>(ast)->visit(symbolDeclVisitor);
    // symbolTable.printScopeLeaves();

    /* Type Checker */
    SEMA::TypeChecker typeChecker(symbolTable, semaAnalyzer);
    static_cast<AST::ASTNode *>(ast)->visit(typeChecker);
    // symbolTable.printSymbolReference();
    // printf("***************************************\n");
    // printf("AST DUMP POST TYPE CHECK\n");
    // ast_print(ast);

    /* Evaluate initialization for const-qualified declaration */
    SEMA::ConstantDeclarationOptimizer constDeclOptimizer;
    static_cast<AST::ASTNode *>(ast)->visit(constDeclOptimizer);
    // printf("***************************************\n");
    // printf("AST DUMP POST CONST DECL OPT\n");
    // ast_print(ast);

    /* Ensure that every variable has been assigned a value before being read */
    SEMA::VariableAssignmentChecker varAssignmentChecker(semaAnalyzer);
    static_cast<AST::ASTNode *>(ast)->visit(varAssignmentChecker);

    /* Check Semantic Analysis Result */
    int numEvents = semaAnalyzer.getNumberEvents();
#ifdef NORMAL_MODE
    semaAnalyzer.setColorPrintEnabled(true);
    semaAnalyzer.setOutput(stdout);
#else
    semaAnalyzer.setColorPrintEnabled(false);
    semaAnalyzer.setOutput(errorFile);
#endif
    if (numEvents != 0)
    {
        fprintf(semaAnalyzer.getOutput(), "\n");
    }
    for (int id = 0; id < numEvents; id++)
    {
        semaAnalyzer.printEvent(id, sourceContext);
    }
    if (numEvents != 0)
    {
        fprintf(semaAnalyzer.getOutput(), "--------------------------------------------------------------------------\n");
    }

    int numErrorEvents = semaAnalyzer.getNumberErrors();
    if (numErrorEvents > 0)
    {
        // If error occurs, return 0
        errorOccurred = 1;
        return 0;
    }
    else
    {
        // if no error, return 1
        return 1;
    }
}