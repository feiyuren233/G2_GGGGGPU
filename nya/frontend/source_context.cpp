#include "source_context.h"

#include <algorithm>
#include <cassert>

namespace SEMA
{ /* START NAMESPACE */
SourceContext::SourceContext(FILE *f)
{
    assert(f != nullptr);

    char *line = NULL;
    size_t len = 0;

    rewind(f);
    while (getline(&line, &len, f) != -1)
    {
        std::string str(line);
        str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
        m_sourceFile.push_back(std::move(str));
    }

    if (line)
    {
        free(line);
    }
}
} // namespace SEMA