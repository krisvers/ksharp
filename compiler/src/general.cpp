#include "general.hpp"
#include <cstring>

namespace ksharp::compiler {

void updateMetaInfo(MetaInfo& metaInfo, const char* source, unsigned int index) {
    metaInfo.index = index;
    metaInfo.column = 1;
    metaInfo.line = 1;
    metaInfo.length = (unsigned int) strlen(source);

    for (unsigned int i = 0; i < index; i++) {
        if (source[i] == '\n') {
            metaInfo.line++;
            metaInfo.column = 0;
        } else {
            metaInfo.column++;
        }
    }
}

} // namespace ksharp::compiler