#ifndef SOURCE_CONTEXT_H_INCLUDED
#define SOURCE_CONTEXT_H_INCLUDED

#include <vector>
#include <string>
#include <stdio.h>

namespace SEMA
{

class SourceContext
{
  private:
    std::vector<std::string> m_sourceFile;

  public:
    SourceContext(FILE *f);

  public:
    const std::vector<std::string> &getLines() const { return m_sourceFile; }
    const std::string &getLine(int line) const { return m_sourceFile.at(line - 1); }
};

} // namespace SEMA

#endif