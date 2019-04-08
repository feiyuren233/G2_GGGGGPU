#ifndef SEMANTIC_H_INCLUDED
#define SEMANTIC_H_INCLUDED

#include "ast.h"

int semantic_check(node *ast);

namespace SEMA
{

enum class DataTypeCategory
{
    Boolean,
    Arithmetic
};

DataTypeCategory getDataTypeCategory(int dataType);

} // namespace SEMA

#endif