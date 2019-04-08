#include "data_container.h"

#include "ast.h"
#include "semantic.h"

#include "common.h"
#include "parser.tab.h"

#include <cassert>

namespace SEMA
{ /* START NAMESPACE */

int &DataContainer::intVal()
{
    assert(m_type == INT_T);
    return m_value.intVal;
}

float &DataContainer::floatVal()
{
    assert(m_type == FLOAT_T);
    return m_value.floatVal;
}

bool &DataContainer::boolVal()
{
    assert(m_type == BOOL_T);
    return m_value.boolVal;
}

int DataContainer::getIntVal() const
{
    assert(m_type == INT_T);
    return m_value.intVal;
}

float DataContainer::getFloatVal() const
{
    assert(m_type == FLOAT_T);
    return m_value.floatVal;
}

bool DataContainer::getBoolVal() const
{
    assert(m_type == BOOL_T);
    return m_value.boolVal;
}

DataContainer operator+(const DataContainer &lhs, const DataContainer &rhs)
{
    /*
     * +, - Arithmetic
     */
    assert(lhs.getType() == rhs.getType());
    assert(getDataTypeCategory(lhs.getType()) == DataTypeCategory::Arithmetic);

    DataContainer result(lhs.getType());
    switch (lhs.getType())
    {
    case INT_T:
    {
        result.intVal() = lhs.getIntVal() + rhs.getIntVal();
        break;
    }
    case FLOAT_T:
    {
        result.floatVal() = lhs.getFloatVal() + rhs.getFloatVal();
        break;
    }
    case BOOL_T:
        assert(0);
    default:
        assert(0);
    }

    return result;
}

DataContainer operator-(const DataContainer &lhs, const DataContainer &rhs)
{
    /*
     * +, - Arithmetic
     */
    assert(lhs.getType() == rhs.getType());
    assert(getDataTypeCategory(lhs.getType()) == DataTypeCategory::Arithmetic);

    DataContainer result(lhs.getType());
    switch (lhs.getType())
    {
    case INT_T:
    {
        result.intVal() = lhs.getIntVal() - rhs.getIntVal();
        break;
    }
    case FLOAT_T:
    {
        result.floatVal() = lhs.getFloatVal() - rhs.getFloatVal();
        break;
    }
    case BOOL_T:
        assert(0);
    default:
        assert(0);
    }

    return result;
}

DataContainer operator*(const DataContainer &lhs, const DataContainer &rhs)
{
    /*
     * * Arithmetic
     */
    assert(lhs.getType() == rhs.getType());
    assert(getDataTypeCategory(lhs.getType()) == DataTypeCategory::Arithmetic);

    DataContainer result(lhs.getType());
    switch (lhs.getType())
    {
    case INT_T:
    {
        result.intVal() = lhs.getIntVal() * rhs.getIntVal();
        break;
    }
    case FLOAT_T:
    {
        result.floatVal() = lhs.getFloatVal() * rhs.getFloatVal();
        break;
    }
    case BOOL_T:
        assert(0);
    default:
        assert(0);
    }

    return result;
}

DataContainer operator/(const DataContainer &lhs, const DataContainer &rhs)
{
    /*
     * / Arithmetic
     */
    assert(lhs.getType() == rhs.getType());
    assert(getDataTypeCategory(lhs.getType()) == DataTypeCategory::Arithmetic);

    DataContainer result(lhs.getType());
    switch (lhs.getType())
    {
    case INT_T:
    {
        result.intVal() = lhs.getIntVal() / rhs.getIntVal();
        break;
    }
    case FLOAT_T:
    {
        result.floatVal() = lhs.getFloatVal() / rhs.getFloatVal();
        break;
    }
    case BOOL_T:
        assert(0);
    default:
        assert(0);
    }

    return result;
}

DataContainer operator^(const DataContainer &lhs, const DataContainer &rhs)
{
    /*
     * ^, &, | Arithmetic with lhs and rhs being int
     */
    assert(lhs.getType() == rhs.getType());
    assert(lhs.getType() == INT_T);

    DataContainer result(lhs.getType());
    switch (lhs.getType())
    {
    case INT_T:
    {
        result.intVal() = lhs.getIntVal() ^ rhs.getIntVal();
        break;
    }
    case FLOAT_T:
    {
        assert(0);
    }
    case BOOL_T:
        assert(0);
    default:
        assert(0);
    }

    return result;
}

DataContainer operator&(const DataContainer &lhs, const DataContainer &rhs)
{
    /*
     * ^, &, | Arithmetic with lhs and rhs being int
     */
    assert(lhs.getType() == rhs.getType());
    assert(lhs.getType() == INT_T);

    DataContainer result(lhs.getType());
    switch (lhs.getType())
    {
    case INT_T:
    {
        result.intVal() = lhs.getIntVal() & rhs.getIntVal();
        break;
    }
    case FLOAT_T:
    {
        assert(0);
    }
    case BOOL_T:
        assert(0);
    default:
        assert(0);
    }

    return result;
}

DataContainer operator|(const DataContainer &lhs, const DataContainer &rhs)
{
    /*
     * ^, &, | Arithmetic with lhs and rhs being int
     */
    assert(lhs.getType() == rhs.getType());
    assert(lhs.getType() == INT_T);

    DataContainer result(lhs.getType());
    switch (lhs.getType())
    {
    case INT_T:
    {
        result.intVal() = lhs.getIntVal() | rhs.getIntVal();
        break;
    }
    case FLOAT_T:
    {
        assert(0);
    }
    case BOOL_T:
        assert(0);
    default:
        assert(0);
    }

    return result;
}

DataContainer operator<<(const DataContainer &lhs, const DataContainer &rhs)
{
    /*
     * <<, >> Arithmetic with lhs and rhs being int
     */
    assert(lhs.getType() == rhs.getType());
    assert(lhs.getType() == INT_T);

    DataContainer result(lhs.getType());
    switch (lhs.getType())
    {
    case INT_T:
    {
        result.intVal() = lhs.getIntVal() << rhs.getIntVal();
        break;
    }
    case FLOAT_T:
    {
        assert(0);
    }
    case BOOL_T:
        assert(0);
    default:
        assert(0);
    }

    return result;
}

DataContainer operator>>(const DataContainer &lhs, const DataContainer &rhs)
{
    /*
     * <<, >> Arithmetic with lhs and rhs being int
     */
    assert(lhs.getType() == rhs.getType());
    assert(lhs.getType() == INT_T);

    DataContainer result(lhs.getType());
    switch (lhs.getType())
    {
    case INT_T:
    {
        result.intVal() = lhs.getIntVal() >> rhs.getIntVal();
        break;
    }
    case FLOAT_T:
    {
        assert(0);
    }
    case BOOL_T:
        assert(0);
    default:
        assert(0);
    }

    return result;
}

DataContainer operator&&(const DataContainer &lhs, const DataContainer &rhs)
{
    /*
     * &&, || Logical
     */
    assert(lhs.getType() == rhs.getType());
    assert(getDataTypeCategory(lhs.getType()) == DataTypeCategory::Boolean);

    DataContainer result(lhs.getType());
    switch (lhs.getType())
    {
    case BOOL_T:
    {
        result.boolVal() = lhs.getBoolVal() && rhs.getBoolVal();
        break;
    }
    case INT_T:
        assert(0);
    case FLOAT_T:
        assert(0);
    default:
        assert(0);
    }

    return result;
}

DataContainer operator||(const DataContainer &lhs, const DataContainer &rhs)
{
    /*
     * &&, || Logical
     */
    assert(lhs.getType() == rhs.getType());
    assert(getDataTypeCategory(lhs.getType()) == DataTypeCategory::Boolean);

    DataContainer result(lhs.getType());
    switch (lhs.getType())
    {
    case BOOL_T:
    {
        result.boolVal() = lhs.getBoolVal() || rhs.getBoolVal();
        break;
    }
    case INT_T:
        assert(0);
    case FLOAT_T:
        assert(0);
    default:
        assert(0);
    }

    return result;
}

DataContainer operator<(const DataContainer &lhs, const DataContainer &rhs)
{
    /*
     * <, <=, >, >= Comparison
     */
    assert(lhs.getType() == rhs.getType());
    assert(getDataTypeCategory(lhs.getType()) == DataTypeCategory::Arithmetic);

    DataContainer result(BOOL_T);
    switch (lhs.getType())
    {
    case INT_T:
    {
        result.boolVal() = lhs.getIntVal() < rhs.getIntVal();
        break;
    }
    case FLOAT_T:
    {
        result.boolVal() = lhs.getFloatVal() < rhs.getFloatVal();
        break;
    }
    case BOOL_T:
        assert(0);
    default:
        assert(0);
    }

    return result;
}

DataContainer operator<=(const DataContainer &lhs, const DataContainer &rhs)
{
    /*
     * <, <=, >, >= Comparison
     */
    assert(lhs.getType() == rhs.getType());
    assert(getDataTypeCategory(lhs.getType()) == DataTypeCategory::Arithmetic);

    DataContainer result(BOOL_T);
    switch (lhs.getType())
    {
    case INT_T:
    {
        result.boolVal() = lhs.getIntVal() <= rhs.getIntVal();
        break;
    }
    case FLOAT_T:
    {
        result.boolVal() = lhs.getFloatVal() <= rhs.getFloatVal();
        break;
    }
    case BOOL_T:
        assert(0);
    default:
        assert(0);
    }

    return result;
}

DataContainer operator>(const DataContainer &lhs, const DataContainer &rhs)
{
    /*
     * <, <=, >, >= Comparison
     */
    assert(lhs.getType() == rhs.getType());
    assert(getDataTypeCategory(lhs.getType()) == DataTypeCategory::Arithmetic);

    DataContainer result(BOOL_T);
    switch (lhs.getType())
    {
    case INT_T:
    {
        result.boolVal() = lhs.getIntVal() > rhs.getIntVal();
        break;
    }
    case FLOAT_T:
    {
        result.boolVal() = lhs.getFloatVal() > rhs.getFloatVal();
        break;
    }
    case BOOL_T:
        assert(0);
    default:
        assert(0);
    }

    return result;
}

DataContainer operator>=(const DataContainer &lhs, const DataContainer &rhs)
{
    /*
     * <, <=, >, >= Comparison
     */
    assert(lhs.getType() == rhs.getType());
    assert(getDataTypeCategory(lhs.getType()) == DataTypeCategory::Arithmetic);

    DataContainer result(BOOL_T);
    switch (lhs.getType())
    {
    case INT_T:
    {
        result.boolVal() = lhs.getIntVal() >= rhs.getIntVal();
        break;
    }
    case FLOAT_T:
    {
        result.boolVal() = lhs.getFloatVal() >= rhs.getFloatVal();
        break;
    }
    case BOOL_T:
        assert(0);
    default:
        assert(0);
    }

    return result;
}

DataContainer operator==(const DataContainer &lhs, const DataContainer &rhs)
{
    /*
     * ==, != Comparison
     */
    assert(lhs.getType() == rhs.getType());
    assert(getDataTypeCategory(lhs.getType()) == DataTypeCategory::Arithmetic);

    DataContainer result(BOOL_T);
    switch (lhs.getType())
    {
    case INT_T:
    {
        result.boolVal() = lhs.getIntVal() == rhs.getIntVal();
        break;
    }
    case FLOAT_T:
    {
        result.boolVal() = lhs.getFloatVal() == rhs.getFloatVal();
        break;
    }
    case BOOL_T:
        assert(0);
    default:
        assert(0);
    }

    return result;
}

DataContainer operator!=(const DataContainer &lhs, const DataContainer &rhs)
{
    /*
     * ==, != Comparison
     */
    assert(lhs.getType() == rhs.getType());
    assert(getDataTypeCategory(lhs.getType()) == DataTypeCategory::Arithmetic);

    DataContainer result(BOOL_T);
    switch (lhs.getType())
    {
    case INT_T:
    {
        result.boolVal() = lhs.getIntVal() != rhs.getIntVal();
        break;
    }
    case FLOAT_T:
    {
        result.boolVal() = lhs.getFloatVal() != rhs.getFloatVal();
        break;
    }
    case BOOL_T:
        assert(0);
    default:
        assert(0);
    }

    return result;
}

DataContainer operator-(const DataContainer &rhs)
{
    /*
     * - Arithmetic
     */
    assert(getDataTypeCategory(rhs.getType()) == DataTypeCategory::Arithmetic);

    DataContainer result(rhs.getType());
    switch (rhs.getType())
    {
    case INT_T:
    {
        result.intVal() = -rhs.getIntVal();
        break;
    }
    case FLOAT_T:
    {
        result.floatVal() = -rhs.getFloatVal();
        break;
    }
    case BOOL_T:
        assert(0);
    default:
        assert(0);
    }

    return result;
}

DataContainer operator!(const DataContainer &rhs)
{
    /*
     * ! Logical
     */
    assert(getDataTypeCategory(rhs.getType()) == DataTypeCategory::Boolean);

    DataContainer result(rhs.getType());
    switch (rhs.getType())
    {
    case BOOL_T:
    {
        result.boolVal() = !rhs.getBoolVal();
        break;
    }
    case INT_T:
        assert(0);
    case FLOAT_T:
        assert(0);
    default:
        assert(0);
    }

    return result;
}

DataContainer operator~(const DataContainer &rhs)
{
    /*
     * ~ Arithmetic with rhs being int
     */
    assert(rhs.getType() == INT_T);

    DataContainer result(rhs.getType());
    switch (rhs.getType())
    {
    case INT_T:
    {
        result.intVal() = ~rhs.getIntVal();
        break;
    }
    case BOOL_T:
        assert(0);
    case FLOAT_T:
        assert(0);
    default:
        assert(0);
    }

    return result;
}

DataContainer &DataContainer::operator=(const DataContainer &rhs)
{
    if (this == &rhs)
    {
        return *this;
    }

    assert((this->getType() == rhs.getType()));
    this->m_value = rhs.m_value;

    return *this;
}

AST::ExpressionNode *DataContainer::createASTExpr() const
{
    AST::ExpressionNode *resultExpr = nullptr;

    switch (m_type)
    {
    case INT_T:
    {
        AST::IntLiteralNode *intLit = new AST::IntLiteralNode(getIntVal());
        resultExpr = intLit;

        break;
    }

    case FLOAT_T:
    {
        AST::FloatLiteralNode *floatLit = new AST::FloatLiteralNode(getFloatVal());
        resultExpr = floatLit;

        break;
    }

    case BOOL_T:
    {
        AST::BooleanLiteralNode *boolLit = new AST::BooleanLiteralNode(getBoolVal());
        resultExpr = boolLit;

        break;
    }

    default:
        assert(0);
    }

    if (resultExpr != nullptr)
    {
        resultExpr->setConst(true);
        resultExpr->setExpressionType(m_type);
    }

    return resultExpr;
}

} // namespace SEMA