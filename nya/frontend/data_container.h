#ifndef DATA_CONTAINER_H_INCLUDED
#define DATA_CONTAINER_H_INCLUDED

// Forward declaration
namespace AST
{
class ExpressionNode;
}

namespace SEMA
{

class DataContainer
{
  private:
    const int m_type;

  private:
    union ValueUnion {
        int intVal;
        float floatVal;
        bool boolVal;
    } m_value = {0};

  public:
    DataContainer(int type) : m_type(type) {}
    DataContainer(const DataContainer &other) : m_type(other.m_type), m_value(other.m_value) {}

  public:
    int getType() const { return m_type; }

  public:
    int &intVal();
    float &floatVal();
    bool &boolVal();
    int getIntVal() const;
    float getFloatVal() const;
    bool getBoolVal() const;

  public:
    DataContainer &operator=(const DataContainer &rhs);

  public:
    AST::ExpressionNode *createASTExpr() const;
};

DataContainer operator+(const DataContainer &lhs, const DataContainer &rhs);
DataContainer operator-(const DataContainer &lhs, const DataContainer &rhs);
DataContainer operator*(const DataContainer &lhs, const DataContainer &rhs);
DataContainer operator/(const DataContainer &lhs, const DataContainer &rhs);
DataContainer operator^(const DataContainer &lhs, const DataContainer &rhs);
DataContainer operator&(const DataContainer &lhs, const DataContainer &rhs);
DataContainer operator|(const DataContainer &lhs, const DataContainer &rhs);
DataContainer operator<<(const DataContainer &lhs, const DataContainer &rhs);
DataContainer operator>>(const DataContainer &lhs, const DataContainer &rhs);
DataContainer operator&&(const DataContainer &lhs, const DataContainer &rhs);
DataContainer operator||(const DataContainer &lhs, const DataContainer &rhs);
DataContainer operator<(const DataContainer &lhs, const DataContainer &rhs);
DataContainer operator<=(const DataContainer &lhs, const DataContainer &rhs);
DataContainer operator>(const DataContainer &lhs, const DataContainer &rhs);
DataContainer operator>=(const DataContainer &lhs, const DataContainer &rhs);
DataContainer operator==(const DataContainer &lhs, const DataContainer &rhs);
DataContainer operator!=(const DataContainer &lhs, const DataContainer &rhs);
DataContainer operator-(const DataContainer &rhs);
DataContainer operator!(const DataContainer &rhs);
DataContainer operator~(const DataContainer &rhs);

} // namespace SEMA

#endif